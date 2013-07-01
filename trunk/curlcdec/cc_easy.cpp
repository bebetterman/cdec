#include "stdafx.h"
#include "../include/cdeccore.h"
#include "../include/curlcdec.h"
#include "adapt_define.h"
#include "adapt_easy.h"

#define VERIFY_CURL_CODE(code)	if (code != 0) throw CurlException((code) + EC_CURL_ST);

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

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

	int code = curl_adapt_global_init(flags);
	VERIFY_CURL_CODE(code);
}

void CurlEasy::GlobalTerm()
{
	curl_adapt_global_cleanup();
}

CurlEasy::CurlEasy(): m_cWriter(NULL)
{
	if ((m_curl = curl_adapt_easy_init()) == NULL)
		throw CurlException(EC_CURL_FAILED_INIT);
}

CurlEasy::~CurlEasy()
{
	curl_adapt_easy_cleanup(m_curl);
	m_curl = NULL;
}

void CurlEasy::SetUrl(const char* url)
{
	int code = curl_adapt_easy_seturl(m_curl, url);
	VERIFY_CURL_CODE(code);
}

//size_t xxx(size_t, void*, void*, size_t) { return 0; }
void CurlEasy::Request()
{
	m_cWriter->OnCurlReset();

	int code = curl_adapt_easy_setwritefunction(m_curl, CurlDataReceiveCallback);
	VERIFY_CURL_CODE(code);

	code = curl_adapt_easy_setwritedata(m_curl, m_cWriter.__GetPointer());
	VERIFY_CURL_CODE(code);

	code = curl_adapt_easy_perform(m_curl);
	VERIFY_CURL_CODE(code);
}

long CurlEasy::GetResponseCode()
{
	long responseCode = 0;
	int code = curl_adapt_easy_getresponsecode(m_curl, &responseCode);
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

// -------------------------------------------------------------------------- //
CDEC_NS_END
