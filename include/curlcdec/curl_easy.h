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

	void	SetProgressCallback(ref<ICurlProgress> prog);

	void	Request(ref<ICurlResponseWriter> wr);
	ref<CurlResponse>	Request();
};

// -------------------------------------------------------------------------- //

inline void CurlEasy::SetPutBytes(ref<ByteArray> data, int offset, int count)
{
	ref<MemoryStream> stream = gc_new<MemoryStream>(data, offset, count);
	SetPutStream(stream);
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
