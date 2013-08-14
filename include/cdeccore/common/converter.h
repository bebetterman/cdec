#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

// Internal usage

template<class _Ty> class ArrayV;

typedef ArrayV<BYTE> ByteArray;

CDECCOREEXPORT stringx	ConverterToHexStringL(const BYTE* bytes, int length);
CDECCOREEXPORT stringx	ConverterToHexStringA(ref<ByteArray> bytes, int offset, int length);

CDECCOREEXPORT int		ConverterFromHexStringL(PCWSTR str, int cch, BYTE* bytes, int capacity);
CDECCOREEXPORT int		ConverterFromHexStringA(PCWSTR str, int cch, ref<ByteArray> bytes, int offset);

// -------------------------------------------------------------------------- //

class Converter: public Object
{
	DECLARE_REF_CLASS(Converter)

public:
	static INT8		ToInt8(stringx value, int n = 10) { return ParseNumber<INT8>(value, n, (INT8)0); }
	static UINT8	ToUInt8(stringx value, int n = 10) { return ParseNumberPositive<UINT8>(value, n, (UINT8)0); }

	static INT16	ToInt16(stringx value, int n = 10) { return ParseNumber<INT16>(value, n, (INT16)0); }
	static UINT16	ToUInt16(stringx value, int n = 10) { return ParseNumberPositive<UINT16>(value, n, (UINT16)0); }

	static int		ToInt32(stringx value, int n = 10) { return ParseNumber<INT32>(value, n, (INT32)0); }
	static UINT		ToUInt32(stringx value, int n = 10) { return ParseNumberPositive<UINT32>(value, n, (UINT32)0); }

	static INT64	ToInt64(stringx value, int n = 10) { return ParseNumber<INT64>(value, n, (INT64)0); }
	static UINT64	ToUInt64(stringx value, int n = 10) { return ParseNumberPositive<UINT64>(value, n, (UINT64)0); }

	static stringx	ToString(int value, int n = 10) { return FormatNumber<int>(value, n); }
	static stringx	ToString(UINT value, int n = 10) { return FormatNumberPositive<UINT>(value, n); }

	static stringx	ToString(INT64 value, int n = 10) { return FormatNumber<INT64>(value, n); }
	static stringx	ToString(UINT64 value, int n = 10) { return FormatNumberPositive<UINT64>(value, n); }

	static stringx	ToString(long value, int n = 10) { return FormatNumber<int>(value, n); }
	static stringx	ToString(unsigned long value, int n = 10) { return FormatNumberPositive<UINT>(value, n); }

	static stringx	ToHexString(const BYTE* bytes, int length) { return ConverterToHexStringL(bytes, length); }
	static stringx	ToHexString(ref<ByteArray> bytes, int offset, int length) { return ConverterToHexStringA(bytes, offset, length); }
	static stringx	ToHexString(ref<ByteArray> bytes) { return ConverterToHexStringA(bytes, 0, bytes->Count()); }

	static int		FromHexString(PCWSTR str, int cch, BYTE* bytes, int length) { return ConverterFromHexStringL(str, cch, bytes, length); }
	static int		FromHexString(stringx str, int pos, int cch, BYTE* bytes, int length);
	static int		FromHexString(stringx str, BYTE* bytes, int length) { return FromHexString(str.c_str(), str.Length(), bytes, length); }

	static int		FromHexString(PCWSTR str, int cch, ref<ByteArray> bytes, int offset) { return ConverterFromHexStringA(str, cch, bytes, offset); }
	static int		FromHexString(stringx str, int pos, int cch, ref<ByteArray> bytes, int offset);
	static int		FromHexString(stringx str, ref<ByteArray> bytes, int offset) { return FromHexString(str.c_str(), str.Length(), bytes, offset); }

	static ref<ByteArray>	FromHexString(PCWSTR str, UINT cch);
	static ref<ByteArray>	FromHexString(stringx str, int pos, int cch);
	static ref<ByteArray>	FromHexString(stringx str) { return FromHexString(str.c_str(), str.Length()); }

protected:
	template<typename T>
	static T ParseNumber(stringx s, UINT n, T);

	template<typename T>
	static T ParseNumberPositive(stringx s, UINT n, T);

	template<typename T>
	static stringx FormatNumber(T value, UINT n);

	template<typename T>
	static stringx FormatNumberPositive(T value, UINT n);

	template<typename T>
	static stringx FormatNumberNegative(T value, UINT n);

public:	// Internal usage
	static UINT ParseDigit(WCHAR ch, UINT n);

	static WCHAR ToDigit(UINT d, UINT n);
};

// -------------------------------------------------------------------------- //

inline int Converter::FromHexString(stringx str, int pos, int cch, BYTE* bytes, int length)
{
	if (CheckOutOfRange(pos, cch, str.Length()))
		cdec_throw(Exception(EC_OutOfRange));
	return ConverterFromHexStringL(str.c_str() + pos, cch, bytes, length);
}

inline int Converter::FromHexString(stringx str, int pos, int cch, ref<ByteArray> bytes, int offset)
{
	if (CheckOutOfRange(pos, cch, str.Length()))
		cdec_throw(Exception(EC_OutOfRange));
	return ConverterFromHexStringA(str.c_str() + pos, cch, bytes, offset);
}

inline ref<ByteArray> Converter::FromHexString(PCWSTR str, UINT cch)
{
	ref<ByteArray> r = gc_new<ByteArray>(cch >> 1);
	ConverterFromHexStringA(str, cch, r, 0);
	return r;
}

inline ref<ByteArray> Converter::FromHexString(stringx str, int pos, int cch)
{
	if (CheckOutOfRange(pos, cch, str.Length()))
		cdec_throw(Exception(EC_OutOfRange));
	return FromHexString(str.c_str() + pos, cch);
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
T Converter::ParseNumber(stringx s, UINT n, T)
{
	WCHAR ch = s[0];
	if (ch == '+')
		return ParseNumberPositive(s.Substring(1), n, T());
	else if (ch == '-')
		return -ParseNumberPositive(s.Substring(1), n, T());
	else
		return ParseNumberPositive(s, n, T());
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
	stringx str = FormatNumberPositive(-value, n);
	return '-' + str;
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
