#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class CURLCDECEXPORT CurlEasy: public Object
{
	DECLARE_REF_CLASS(CurlEasy)

	CurlOption					m_ops;
	void*						m_curl;
	std::vector<std::string>	m_headers;

	ref<Stream>					m_istream;	// PUT

public:
	static void GlobalInit();
	static void GlobalTerm();

	CurlEasy(CurlOption ops = CCO_None);
	inline ~CurlEasy() { CleanUp(); }

	void	CleanUp();

	void	SetUrl(const char* url);
	void	SetUrl(stringx url);

	void	SetConnectionTimeOut(int seconds);
	void	SetTimeOut(int seconds);

	// SetOutputStream will be supported as an alternative
//	void	SetContentWriter(ref<ICurlContentWriter> cWriter) { m_cWriter = cWriter; }

	void	AddHeader(stringx key, stringx value);

	void	SetPostBytes(const void* data, int length);
	void	SetPostBytes(ref<ByteArray> data, int offset, int length);
	void	SetPostBytes(ref<ByteArray> data) { SetPostBytes(data, 0, data->Count()); }
	void	SetPostText(const char* text, int length) { SetPostBytes(text, length); }
	void	SetPostText(stringx str, int offset, int length);
	void	SetPostText(stringx str) { SetPostText(str, 0, str.Length()); }

	void	SetPutStream(ref<Stream> istream);
	void	SetPutBytes(ref<ByteArray> data, int offset, int length);

	void	SetCustomRequest(PCSTR method);
	void	SetCustomRequest(stringx method);

	void	Request(ref<ICurlResponseWriter> wr);
	ref<CurlResponse>	Request();

private:
	static size_t CurlDataReadCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

	// callback for writing response headers
	static size_t CurlHeaderWriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

	// callback for writing response body data (stream)
	static size_t CurlDataWriteCallback(void *buffer, size_t size, size_t nmemb, void *user_p);
};

// -------------------------------------------------------------------------- //

inline void CurlEasy::SetPutBytes(ref<ByteArray> data, int offset, int count)
{
	ref<MemoryStream> stream = gc_new<MemoryStream>(data, offset, count);
	SetPutStream(stream);
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
