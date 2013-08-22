#include "stdafx.h"

#define VERIFY_CURL_CODE(code)	if (code != 0) throw CurlException((code) + EC_CURL_ST);

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class CurlByteBufferContentWriter: public ICurlContentWriter
{
	DECLARE_REF_CLASS(CurlByteBufferContentWriter)

	typedef std::vector<unsigned char> ByteBuffer;
	ByteBuffer	m_buffer;

public:
	void	Reserve(int size) { m_buffer.reserve(size); }

	void	OnCurlReset() { m_buffer.clear(); }
	void	OnCurlReceive(const void* buffer, int size);

	int		GetDataLength() { return m_buffer.size(); }
	const void*	GetData() { return m_buffer.size() != 0 ? &m_buffer[0] : NULL; }

};

void CurlByteBufferContentWriter::OnCurlReceive(const void* buffer, int size)
{
	size_t rcOld = m_buffer.size();
	size_t rcNew = rcOld + size;
	m_buffer.resize(rcNew);
	memcpy(&m_buffer[rcOld], buffer, size);
}

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

CurlEasy::CurlEasy()
{
	if ((m_curl = curl_easy_init()) == NULL)
		throw CurlException(EC_CURL_FAILED_INIT);

	m_cWriter = gc_new<CurlByteBufferContentWriter>();

	// Default time-out values
	SetConnectionTimeOut(60);
	SetTimeOut(120);
}

CurlEasy::~CurlEasy()
{
	curl_easy_cleanup(m_curl);
	m_curl = NULL;
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

//size_t xxx(size_t, void*, void*, size_t) { return 0; }
void CurlEasy::Request()
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
	m_cWriter->OnCurlReset();

	code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlDataReceiveCallback);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_cWriter.__GetPointer());
	VERIFY_CURL_CODE(code);

	// Perform action
	code = curl_easy_perform(m_curl);
	VERIFY_CURL_CODE(code);

	// Clean up
	if (headers != NULL)
		curl_slist_free_all(headers); // free the header list
}

long CurlEasy::GetResponseCode()
{
	long responseCode = 0;
	int code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &responseCode);
	VERIFY_CURL_CODE(code);
	return responseCode;
}

size_t CurlEasy::CurlDataReceiveCallback(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	ICurlContentWriter*	pWriter = (ICurlContentWriter*)user_p;
	size_t cbTotal = size * nmemb;
	pWriter->OnCurlReceive(buffer, cbTotal);
	return cbTotal;
}

ref<ByteArray> CurlEasy::ReadResponseData()
{
	ref<CurlByteBufferContentWriter> cwr = ref_cast<CurlByteBufferContentWriter>(m_cWriter);
	ref<ByteArray> data = gc_new<ByteArray>((const BYTE*)cwr->GetData(), cwr->GetDataLength());
	return data;
}

stringx CurlEasy::ReadResponseText()
{
	ref<CurlByteBufferContentWriter> cwr = ref_cast<CurlByteBufferContentWriter>(m_cWriter);
	stringx text = Encoding::get_UTF8()->GetString((const BYTE*)cwr->GetData(), cwr->GetDataLength());
	return text;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
