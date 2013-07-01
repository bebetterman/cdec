#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void Console::WriteChar(WCHAR ch)
{
#ifdef X_OS_WINDOWS
	putwchar(ch);
#else
	if ((ch & 0xFF80) != 0)
	{
		std::string s = Encoding::EncodeUtf8Char(ch);
		printf(s.c_str());
	}
	else
		putchar(ch);
#endif
}

void Console::Write(stringx value)
{
#ifdef X_OS_WINDOWS
	_cputws(value.c_str());
#else
	std::string s = Encoding::get_UTF8()->FromUnicode(value);
	printf(s.c_str());
#endif
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
