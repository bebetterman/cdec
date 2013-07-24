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
	const void*		GetData() { return &m_buffer[0]; }
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
}

CurlEasy::~CurlEasy()
{
	curl_easy_cleanup(m_curl);
	m_curl = NULL;
}

void CurlEasy::SetUrl(const char* url)
{
	int code = curl_easy_setopt(m_curl, CURLOPT_URL, url);;
	VERIFY_CURL_CODE(code);
}

//size_t xxx(size_t, void*, void*, size_t) { return 0; }
void CurlEasy::Request()
{
	m_cWriter->OnCurlReset();

	int code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, CurlDataReceiveCallback);
	VERIFY_CURL_CODE(code);

	code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_cWriter.__GetPointer());
	VERIFY_CURL_CODE(code);

	code = curl_easy_perform(m_curl);
	VERIFY_CURL_CODE(code);
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

// -------------------------------------------------------------------------- //
CDEC_NS_END
