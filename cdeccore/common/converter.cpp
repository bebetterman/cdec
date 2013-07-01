#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void ConverterToHexStringL(const BYTE* bytes, UINT length, std::wstring16& s)
{
	s.reserve(s.size() + length + length);
	for (UINT i = 0; i < length; ++i)
	{
		BYTE a = bytes[i];
		s += Converter::ToDigit(a >> 4, 16);
		s += Converter::ToDigit(a & 0xF, 16);
	}
}

UINT ConverterFromHexStringL(PCWSTR str, UINT cch, BYTE* bytes, UINT capacity)
{
	if ((cch & 1) != 0)
		cdec_throw(Exception(EC_InvalidArg));
	if (capacity < (cch >> 1))
		cdec_throw(Exception(EC_OutOfBuffer));

	for (UINT i = 0; i < cch; i += 2)
	{
		*bytes++ = (Converter::ParseDigit(str[i], 16) << 4) | Converter::ParseDigit(str[i + 1], 16);
	}
	return cch >> 1;
}

stringx	ConverterToHexString(ref<ByteArray> bytes, UINT offset, UINT length)
{
	if (offset > bytes->Count() || offset + length > bytes->Count())
		cdec_throw(Exception(EC_OutOfRange));
	
	std::wstring16 s;
	pin_ptr<BYTE> pinBytes = bytes->GetBuffer();
	ConverterToHexStringL(pinBytes.ptr() + offset, length, s);

	return stringx(s);
}

UINT ConverterFromHexString(stringx str, ref<ByteArray> bytes, UINT offset)
{
	UINT count = bytes->Count();
	if (offset > count)
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pinBytes = bytes->GetBuffer();
	return ConverterFromHexStringL(str.c_str(), str.Length(), pinBytes.ptr() + offset, count - offset);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
