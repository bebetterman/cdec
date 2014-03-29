#include "stdafx.h"

#define VERIFY_CURL_CODE(code)	if (code != 0) throw CurlException((code) + EC_CURL_ST);

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Callback functions
// -------------------------------------------------------------------------- //

size_t CurlDataReadCallback(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	Stream* pStream = (Stream*)user_p;
	size_t cbop = size * nmemb;
	cbop = pStream->Read(buffer, cbop);
	return cbop;
}

size_t CurlDataWriteCallback(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	ICurlContentWriter* contentw = (ICurlContentWriter*)user_p;
	size_t cbop = size * nmemb;
	contentw->Write(buffer, cbop);
	return cbop;
}

size_t CurlHeaderWriteCallback(void* buffer, size_t size, size_t nmemb, void* user_p)
{
	ICurlResponseWriter* response = (ICurlResponseWriter*)user_p;
	size_t cbop = size * nmemb;
	stringx s = Encoding::get_UTF8()->GetString((BYTE*)buffer, cbop);
	s = s.TrimRight();

	if (s.StartsWith(__X("HTTP")))
	{
		// Console::WriteLine(__X("[HTTP State] ") + s);
		response->SetHttpState(s);
	}
	else if (s.Length() != 0)
	{
		int pos = s.IndexOf(':');
		ASSERT(pos > 0);
		stringx key = s.Substring(0, pos).TrimRight();
		stringx value = s.Substring(pos + 1).TrimLeft();

		// Console::WriteLine(__X("[Header] Key=\"") + key + __X("\" Value=\"") + value + '\"');
		response->AddHeader(key, value);
	}

	return cbop;
}

int CurlProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	ICurlProgress* prog = (ICurlProgress*)clientp;

	if (dltotal != 0.0)
		return prog->OnDownload(dltotal, dlnow);
	else if (ultotal != 0.0)
		return prog->OnUpload(ultotal, ulnow);
	else
		return 0;
}

// -------------------------------------------------------------------------- //
// Curl Easy
// -------------------------------------------------------------------------- //

void CurlEasy::GlobalInit()
{
#if defined(X_OS_WINDOWS)
	long flags = CURL_GLOBAL_WIN32;	
#else
	long flags = CURL_GLOBAL_NOTHING;
#endif

	int code = curl_global_init(flags);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::GlobalTerm()
{
	curl_global_cleanup();
}

CurlEasy::CurlEasy(CurlOption ops): m_ops(ops)
{
	if ((m_curl = curl_easy_init()) == NULL)
		throw CurlException(EC_CURL_FAILED_INIT);

	// Default time-out values
	SetConnectionTimeOut(60);
	SetTimeOut(120);
}

void CurlEasy::CleanUp()
{
	if (m_curl != NULL)
	{
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
}

void CurlEasy::SetProxy(stringx proxyHost, UINT proxyPort, CurlProxyType proxyType /* = CPT_SOCKS4 */)
{
	stringx hostPort = proxyHost + __X(":") + Converter::ToString(proxyPort);

	CURLcode code = curl_easy_setopt(
		m_curl, 
		CURLOPT_PROXY, 
		Encoding::get_UTF8()->FromUnicode(hostPort).c_str()
	);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(
		m_curl,
		CURLOPT_PROXYTYPE,
		proxyType == CPT_SOCKS4 ? CURLPROXY_SOCKS4 : CURLPROXY_SOCKS5
	);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetUrl(const char* url)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_URL, url);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetUrl(stringx url)
{
	SetUrl(Encoding::get_UTF8()->FromUnicode(url).c_str());
}

void CurlEasy::SetConnectionTimeOut(int seconds)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, seconds);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetTimeOut(int seconds)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, seconds);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::AddHeader(stringx key, stringx value)
{
	stringx line = key + __X(": ") + value;
	m_headers.push_back(Encoding::get_UTF8()->FromUnicode(line));
}

void CurlEasy::SetPostBytes(const void* data, int length)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, length);		// Set post data length
	VERIFY_CURL_CODE(code);

	// CURLOPT_COPYPOSTFIELDS to copy the data
	// CURLOPT_POSTFIELDS for static resource
	code = curl_easy_setopt(m_curl, CURLOPT_COPYPOSTFIELDS, data);	// Set post data
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetPostBytes(ref<ByteArray> data, int offset, int length)
{
	if (CheckOutOfRange(offset, length, data->Count()))
		cdec_throw(Exception(EC_OutOfRange));

	SetPostBytes(data->GetBuffer().ptr() + offset, length);
}

void CurlEasy::SetPostText(stringx s, int offset, int length)
{
	std::string ss = Encoding::get_UTF8()->FromUnicode(s, offset, length);
	SetPostBytes(ss.c_str(), ss.size());
	ss.clear();
}

void CurlEasy::SetPutStream(ref<Stream> istream)
{
	m_istream = istream;
	m_istream->Seek(0, Stream::SeekBegin);

	int code = curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, CurlDataReadCallback);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_READDATA, m_istream.__GetPointer());
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetCustomRequest(PCSTR method)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, method);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::SetCustomRequest(stringx method)
{
	std::string method_cs = Encoding::get_UTF8()->FromUnicode(method);
	SetCustomRequest(method_cs.c_str());
}

void CurlEasy::SetProgressCallback(ref<ICurlProgress> prog)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, CurlProgressCallback);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, prog.__GetPointer());
	VERIFY_CURL_CODE(code);
}

void CurlEasy::Request(ref<ICurlResponseWriter> response)
{
	int code = 0;

	// Write headers
	struct curl_slist *headers = NULL;
	if (m_headers.size() != 0)
	{
		typedef std::vector<std::string>::iterator headers_it;
		foreach_it (headers_it, it, m_headers.begin(), m_headers.end())
			headers = curl_slist_append(headers, it->c_str());
	
		code = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);
		VERIFY_CURL_CODE(code);
	}

	// Set response receiver
	ref<ICurlContentWriter> cntwr = response->GetContentWriter();
	cntwr->Reset();

	code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlDataWriteCallback);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, cntwr.__GetPointer());
	VERIFY_CURL_CODE(code);

	// Set response header receiver
	if (m_ops & CCO_ResponseHeaders)
	{
		int code = curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, CurlHeaderWriteCallback);
		VERIFY_CURL_CODE(code);

		code = curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, response.__GetPointer());
		VERIFY_CURL_CODE(code);
	}

	// Perform action
	code = curl_easy_perform(m_curl);
	VERIFY_CURL_CODE(code);

	// Read response information
	long responseCode = 0;
	code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &responseCode);
	VERIFY_CURL_CODE(code);

	response->SetResponseCode((int)responseCode);

	// Clean up
	if (headers != NULL)
		curl_slist_free_all(headers); // free the header list
}

ref<CurlResponse> CurlEasy::Request()
{
	ref<CurlResponse> response = gc_new<CurlResponse>(m_ops);
	Request(response);
	return response;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
