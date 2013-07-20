#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

stringx ConverterToHexStringL(const BYTE* bytes, int length)
{
	if (length < 0)
		cdec_throw(Exception(EC_OutOfRange));

	std::wstring16 s;
	s.reserve(s.size() + length + length);
	for (int i = 0; i < length; ++i)
	{
		BYTE a = bytes[i];
		s += Converter::ToDigit(a >> 4, 16);
		s += Converter::ToDigit(a & 0xF, 16);
	}
	return stringx(s);
}

stringx ConverterToHexStringA(ref<ByteArray> bytes, int offset, int length)
{
	if (CheckOutOfRange(offset, length, bytes->Count()))
		cdec_throw(Exception(EC_OutOfRange));
	
	pin_ptr<BYTE> pinBytes = bytes->GetBuffer();
	return ConverterToHexStringL(pinBytes.ptr() + offset, length);
}

int ConverterFromHexStringL(PCWSTR str, int cch, BYTE* bytes, int capacity)
{
	if ((cch & 1) != 0)
		cdec_throw(Exception(EC_InvalidArg));
	if (capacity < (cch >> 1))
		cdec_throw(Exception(EC_OutOfBuffer));

	for (int i = 0; i < cch; i += 2)
	{
		*bytes++ = (Converter::ParseDigit(str[i], 16) << 4) | Converter::ParseDigit(str[i + 1], 16);
	}
	return cch >> 1;
}

int ConverterFromHexStringA(PCWSTR str, int cch, ref<ByteArray> bytes, int offset)
{
	int capacity = bytes->Count();
	if (CheckOutOfRange(offset, cch >> 1, capacity))
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pinBytes = bytes->GetBuffer();
	return ConverterFromHexStringL(str, cch, pinBytes.ptr() + offset, capacity - offset);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
