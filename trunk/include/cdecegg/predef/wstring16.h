#pragma once

// -------------------------------------------------------------------------- //
// UCS-2 String Functions
//
// Used by C-- Library, and may be used (but not recommended) in applications
// -------------------------------------------------------------------------- //

inline int wstrlen16(PCWSTR s)
{
	int n = 0;
	while (*s++ != 0)
		++n;
	return n;
}

inline int wstrcmp16(PCWSTR lhs, PCWSTR rhs)
{
	while (*lhs == *rhs)
	{
		if (*lhs == 0)
			return 0;
		++lhs;
		++rhs;
	}
	return *lhs < *rhs ? -1 : 1;
}

inline int wstrncmp16(PCWSTR lhs, PCWSTR rhs, int n)
{
	while (n > 0 && *lhs == *rhs)
	{
		if (*lhs == 0)
			return 0;
		++lhs;
		++rhs;
		--n;
	}
	return n == 0 ? 0 : (*lhs < *rhs ? -1 : 1);
}

inline int wstricmp16(PCWSTR lhs, PCWSTR rhs)
{
	while (true)
	{
		WCHAR a = *lhs++, b = *rhs++;
		if (a >= 'A' && a <= 'Z') a += 'a' - 'A';
		if (b >= 'A' && b <= 'Z') b += 'a' - 'A';

		if (a == b)
		{
			if (a == 0)
				return 0;
		}
		else if (a < b)
			return -1;
		else
			return 1;
	}
}

// -------------------------------------------------------------------------- //
// STL string for UCS-2
// -------------------------------------------------------------------------- //

namespace std { typedef basic_string<WCHAR> wstring16; }

// -------------------------------------------------------------------------- //
