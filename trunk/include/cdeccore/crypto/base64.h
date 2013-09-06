#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

CDECCOREEXPORT stringx Base64Encode(const BYTE* data, int len);

CDECCOREEXPORT ref<ByteArray> Base64Decode(stringx data);

class Base64
{
public:
	static inline stringx Encode(const BYTE* data, int len) { return Base64Encode(data, len); }

	static inline stringx Encode(ref<ByteArray> data, int offset, int count)
	{
		if (CheckOutOfRange(offset, count, data->Count()))
			cdec_throw(Exception(EC_OutOfRange));
		pin_ptr<BYTE> pin = data->GetBuffer();
		return Base64Encode(pin.ptr() + offset, count);
	}

	static inline stringx Encode(ref<ByteArray> data) { return Encode(data, 0, data->Count()); }

	static inline ref<ByteArray> Decode(stringx data) { return Base64Decode(data); }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
