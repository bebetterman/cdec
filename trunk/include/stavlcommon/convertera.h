#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// ConverterA
// -------------------------------------------------------------------------- //

class ConverterA: public Object
{
	DECLARE_REF_CLASS(ConverterA)

public:
	static INT8		ToInt8(const char* value, int n = 10) { return ParseNumber<INT8>(value, n, (INT8)0); }
	static UINT8	ToUInt8(const char* value, int n = 10) { return ParseNumberPositive<UINT8>(value, n, (UINT8)0); }

	static INT16	ToInt16(const char* value, int n = 10) { return ParseNumber<INT16>(value, n, (INT16)0); }
	static UINT16	ToUInt16(const char* value, int n = 10) { return ParseNumberPositive<UINT16>(value, n, (UINT16)0); }

	static int		ToInt32(const char* value, int n = 10) { return ParseNumber<INT32>(value, n, (INT32)0); }
	static UINT		ToUInt32(const char* value, int n = 10) { return ParseNumberPositive<UINT32>(value, n, (UINT32)0); }

	static INT64	ToInt64(const char* value, int n = 10) { return ParseNumber<INT64>(value, n, (INT64)0); }
	static UINT64	ToUInt64(const char* value, int n = 10) { return ParseNumberPositive<UINT64>(value, n, (UINT64)0); }

	static std::string	ToString(int value, int n = 10) { return FormatNumber<int>(value, n); }
	static std::string	ToString(UINT value, int n = 10) { return FormatNumberPositive<UINT>(value, n); }

	static std::string	ToString(INT64 value, int n = 10) { return FormatNumber<INT64>(value, n); }
	static std::string	ToString(UINT64 value, int n = 10) { return FormatNumberPositive<UINT64>(value, n); }

	static std::string	ToString(long value, int n = 10) { return FormatNumber<int>(value, n); }
	static std::string	ToString(unsigned long value, int n = 10) { return FormatNumberPositive<UINT>(value, n); }

protected:
	template<typename T>
	static T ParseNumber(const char* s, UINT n, T);

	template<typename T>
	static T ParseNumberPositive(const char* s, UINT n, T);

	template<typename T>
	static std::string FormatNumber(T value, UINT n);

	template<typename T>
	static std::string FormatNumberPositive(T value, UINT n);

	template<typename T>
	static std::string FormatNumberNegative(T value, UINT n);

public:	// Internal usage
	static UINT ParseDigit(char ch, UINT n);
	static char ToDigit(UINT d, UINT n);
};

// -------------------------------------------------------------------------- //

inline UINT ConverterA::ParseDigit(char ch, UINT n)
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

inline char ConverterA::ToDigit(UINT d, UINT n)
{
	ASSERT(d < n || n <= 36);
	return d < 10 ? (d + '0') : (d - 10 + 'a');
}

template<typename T>
T ConverterA::ParseNumber(const char* s, UINT n, T)
{
	char ch = s[0];
	if (ch == '+')
		return ParseNumberPositive(s + 1, n, T());
	else if (ch == '-')
		return -ParseNumberPositive(s + 1, n, T());
	else
		return ParseNumberPositive(s, n, T());
}

template<typename T>
T ConverterA::ParseNumberPositive(const char* s, UINT n, T)
{
	T value = 0;
	char ch;
	while ((ch = *s++) != 0)
	{
		T value2 = value * n + ParseDigit(ch, n);
		if (value2 < value)
			cdec_throw(Exception(EC_Overflow));
		value = value2;
	}
	return value;
}

template<typename T>
inline std::string ConverterA::FormatNumber(T value, UINT n)
{
	if (value >= 0)
		return FormatNumberPositive<T>(value, n);
	else
		return FormatNumberNegative<T>(value, n);
}

template<typename T>
std::string ConverterA::FormatNumberPositive(T value, UINT n)
{
	if (value == 0)
		return "0";

	char	buffer[64];	// UINT64 in binary
	UINT	pos = 64;
	while (value > 0)
	{
		buffer[--pos] = ToDigit(value % n, n);
		value /= n;
	}
	return std::string(buffer + pos, 64 - pos);
}

template<typename T>
std::string ConverterA::FormatNumberNegative(T value, UINT n)
{
	std::string str = FormatNumberPositive(-value, n);
	return '-' + str;
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
