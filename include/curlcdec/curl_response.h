#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

interface ICurlContentWriter: public Object
{
	DECLARE_REF_CLASS(ICurlContentWriter)

	virtual void OnCurlReset() = 0;
	virtual void OnCurlReceive(const void* buffer, int size) = 0;
};

class CurlResponse: public Object
{
	DECLARE_REF_CLASS(Response)

public:
	typedef SortedMapVV<stringx, stringx> Map;

	int					HttpCode;		// 200
	stringx				HttpState;		// HTTP/1.1 200 OK	
	ref<Map>			Headers;		// CCO_ResponseHeaders required
	ref<MemoryStream>	Stream;

	inline CurlResponse(): HttpCode(0)
	{
		Stream = gc_new<MemoryStream>();
	}

	inline ref<ByteArray> GetBytes()
	{
		return Stream->GetBytes();
	}

	inline stringx GetString()
	{
		ref<ByteArray> data = GetBytes();
		return Encoding::get_UTF8()->GetString(data);
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
