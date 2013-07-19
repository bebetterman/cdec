#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

// Internal usage

template<class _Ty> class ArrayV;

typedef ArrayV<BYTE> ByteArray;

CDECCOREEXPORT void		ConverterToHexStringL(const BYTE* bytes, UINT length, std::wstring16& s);
CDECCOREEXPORT stringx	ConverterToHexString(ref<ByteArray> bytes, UINT offset, UINT length);

CDECCOREEXPORT UINT		ConverterFromHexStringL(PCWSTR str, UINT cch, BYTE* bytes, UINT capacity);
CDECCOREEXPORT UINT		ConverterFromHexStringA(PCWSTR str, UINT cch, ref<ByteArray> bytes, UINT offset);

// -------------------------------------------------------------------------- //

class Converter: public Object
{
	DECLARE_REF_CLASS(Converter)

public:
	static INT8		ToInt8(stringx value, int n = 10) { return ParseNumberPositive<INT8>(value, n, (INT8)0); }
	static UINT8	ToUInt8(stringx value, int n = 10) { return ParseNumberPositive<UINT8>(value, n, (UINT8)0); }

	static INT16	ToInt16(stringx value, int n = 10) { return ParseNumberPositive<INT16>(value, n, (INT16)0); }
	static UINT16	ToUInt16(stringx value, int n = 10) { return ParseNumberPositive<UINT16>(value, n, (UINT16)0); }

	static int		ToInt32(stringx value, int n = 10) { return ParseNumberPositive<INT32>(value, n, (INT32)0); }
	static UINT		ToUInt32(stringx value, int n = 10) { return ParseNumberPositive<UINT32>(value, n, (UINT32)0); }

	static INT64	ToInt64(stringx value, int n = 10) { return ParseNumberPositive<INT64>(value, n, (INT64)0); }
	static UINT64	ToUInt64(stringx value, int n = 10) { return ParseNumberPositive<UINT64>(value, n, (UINT64)0); }

	static stringx	ToString(int value, UINT n = 10) { return FormatNumber<int>(value, n); }
	static stringx	ToString(UINT value, UINT n = 10) { return FormatNumberPositive<UINT>(value, n); }

	static stringx	ToString(INT64 value, UINT n = 10) { return FormatNumber<INT64>(value, n); }
	static stringx	ToString(UINT64 value, UINT n = 10) { return FormatNumberPositive<UINT64>(value, n); }

	static void		ToHexString(const BYTE* bytes, UINT length, std::wstring16& s) { ConverterToHexStringL(bytes, length, s); }
	static stringx	ToHexString(const BYTE* bytes, UINT length);
	static stringx	ToHexString(ref<ByteArray> bytes, UINT offset, UINT length) { return ConverterToHexString(bytes, offset, length); }
	static stringx	ToHexString(ref<ByteArray> bytes);

	static UINT		FromHexString(PCWSTR str, UINT cch, BYTE* bytes, UINT length) { return ConverterFromHexStringL(str, cch, bytes, length); }
	static UINT		FromHexString(stringx str, ref<ByteArray> bytes, UINT offset) { return ConverterFromHexStringA(str.c_str(), str.Length(), bytes, offset); }
	static ref<ByteArray>	FromHexString(stringx str);
	static ref<ByteArray>	FromHexString(PCWSTR str, UINT cch);

protected:
	template<typename T>
	static T ParseNumberPositive(stringx s, UINT n, T);

	template<typename T>
	static stringx FormatNumber(T value, UINT n);

	template<typename T>
	static stringx FormatNumberPositive(T value, UINT n);

	template<typename T>
	static stringx FormatNumberNegative(T value, UINT n);

public:	// Internal usage
	static UINT ParseDigit(WCHAR ch);
	static UINT ParseDigit(WCHAR ch, UINT n);

	static WCHAR ToDigit(UINT d, UINT n);
};

// -------------------------------------------------------------------------- //

inline stringx Converter::ToHexString(const BYTE* bytes, UINT length)
{
	std::wstring16 s;
	ConverterToHexStringL(bytes, length, s);
	return stringx(s);
}

inline stringx Converter::ToHexString(ref<ByteArray> bytes)
{
	return ConverterToHexString(bytes, 0, bytes->Count());
}

inline ref<ByteArray> Converter::FromHexString(stringx str)
{
	return FromHexString(str.c_str(), str.Length());
}

inline ref<ByteArray> Converter::FromHexString(PCWSTR str, UINT cch)
{
	if ((cch & 1) != 0)
		cdec_throw(Exception(EC_InvalidArg));
	ref<ByteArray> r = gc_new<ByteArray>(cch >> 1);
	ConverterFromHexStringA(str, cch, r, 0);
	return r;
}

// -------------------------------------------------------------------------- //

inline UINT Converter::ParseDigit(WCHAR ch, UINT n)
{
	UINT i = 0;
	if (ch >= '0' && ch <= '9')
		i = ch - '0';
	else if (ch >= 'A' && ch <= 'Z')
		i = ch - ('A' - 10);
	else if (ch >= 'a' && ch <= 'z')
		i = ch - ('a' - 10);
	else
		cdec_throw(Exception(EC_InvalidArg));
	if (i >= n)
		cdec_throw(Exception(EC_OutOfRange));
	return i;
}

inline WCHAR Converter::ToDigit(UINT d, UINT n)
{
	ASSERT(d < n || n <= 36);
	return d < 10 ? (d + '0') : (d - 10 + 'a');
}

template<typename T>
T Converter::ParseNumberPositive(stringx s, UINT n, T)
{
	T value = 0;
	foreach (WCHAR, ch, s)
	{
		T value2 = value * n + ParseDigit(ch, n);
		if (value2 < value)
			cdec_throw(Exception(EC_Overflow));
		value = value2;
	}
	return value;
}

template<typename T>
inline stringx Converter::FormatNumber(T value, UINT n)
{
	if (value >= 0)
		return FormatNumberPositive<T>(value, n);
	else
		return FormatNumberNegative<T>(value, n);
}

template<typename T>
stringx Converter::FormatNumberPositive(T value, UINT n)
{
	if (value == 0)
		return __X("0");

	WCHAR	buffer[64];	// UINT64 in binary
	UINT	pos = 64;
	while (value > 0)
	{
		buffer[--pos] = ToDigit(value % n, n);
		value /= n;
	}
	return stringx(buffer + pos, 64 - pos);
}

template<typename T>
stringx Converter::FormatNumberNegative(T value, UINT n)
{
	cdec_throw(Exception(EC_NotImplemented));
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
