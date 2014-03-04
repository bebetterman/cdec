#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

interface ICurlContentWriter: public Object
{
	DECLARE_REF_CLASS(ICurlContentWriter)

	virtual void Reset() = 0;
	virtual void Write(const void* buffer, int size) = 0;
};

class StreamCurlContentWriter : public ICurlContentWriter
{
	DECLARE_REF_CLASS(StreamCurlContentWriter)

protected:
	ref<Stream>	m_stream;

public:
	StreamCurlContentWriter(ref<Stream> stream): m_stream(stream) {}

	void Reset()
	{
		m_stream->SetLength(0);
		m_stream->Seek(0);
	}

	void Write(const void* buffer, int size)
	{
		if (m_stream->Write(buffer, size) != size)
			cdec_throw(IOException(EC_IO_ReadWriteFail));
	}
};

// -------------------------------------------------------------------------- //

interface ICurlResponseWriter : public Object
{
	DECLARE_REF_CLASS(ICurlResponseWriter)

	virtual void SetResponseCode(int code) = 0;
	virtual void SetHttpState(stringx value) = 0;
	virtual void AddHeader(stringx key, stringx value) = 0;
	virtual ref<ICurlContentWriter> GetContentWriter() = 0;
};

class CurlResponse: public ICurlResponseWriter
{
	DECLARE_REF_CLASS(Response)

protected:
	typedef SortedMapVV<stringx, stringx> HeaderMap;

	int					m_responseCode;		// 200
	stringx				m_httpState;			// HTTP/1.1 200 OK	
	ref<HeaderMap>		m_headers;			// CCO_ResponseHeaders required
	ref<MemoryStream>	m_stream;

public:
	inline CurlResponse(CurlOption ops): m_responseCode(0)
	{
		if (ops & CCO_ResponseHeaders)
			m_headers = gc_new<HeaderMap>();
		m_stream = gc_new<MemoryStream>();
	}

	inline ~CurlResponse() { Close(); }

	void SetResponseCode(int code)
	{
		m_responseCode = code;
	}

	void SetHttpState(stringx value)
	{
		ASSERT(m_httpState == NULL);
		m_httpState = value;
	}

	void AddHeader(stringx key, stringx value)
	{
		m_headers->Insert(key, value);
	}

	ref<ICurlContentWriter> GetContentWriter()
	{
		return gc_new<StreamCurlContentWriter>(m_stream);
	}

	inline int GetResponseCode() { return m_responseCode; }

	inline ref<MemoryStream> GetStream() { return m_stream; }

	inline ref<ByteArray> GetBytes() { return m_stream->GetBytes(); }

	inline stringx GetString()
	{
		ref<ByteArray> data = GetBytes();
		return Encoding::get_UTF8()->GetString(data);
	}

	inline ref<HeaderMap> GetHeaders() { return m_headers; }

	void Close()
	{
		if (m_stream != NULL)
		{
			m_stream->Close();
			m_stream = NULL;
		}
		m_headers = NULL;
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
