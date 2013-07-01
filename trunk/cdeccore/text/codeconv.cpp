#include "stdafx.h"
#include "codeconv.h"

#ifndef X_OS_WINDOWS
#	include <iconv.h>
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Implementions
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

// -------------------------------------------------------------------------- //

#else

#define CP_ACP               0		// default to ANSI code page
#define CP_CHS				936     // Chinese Simplified
#define CP_CHT				950		// Chinese Traditional
#define CH_JP				932		// Japanese
#define CH_VT				1258	// Vietnamese
#define CP_UTF8              65001	// UTF-8 translation

bool CodePageStr(UINT nCodePage, char* pszPage)
{
	ASSERT(pszPage != NULL);
	switch (nCodePage)
	{
	case CH_JP://Japanese
		strcpy(pszPage, "CP932");
		break;
	case CP_CHS://CHS
		strcpy(pszPage, "CP936");
		break;
	case CP_CHT://CHT
		strcpy(pszPage, "CP950");
		break;
	case CH_VT://Vietnamese
		strcpy(pszPage, "CP1258");
		break;
	case CP_UTF8:
		strcpy(pszPage, "UTF-8");
		break;
	default:
		cdec_throw(Exception(EC_InvalidArg));
	}
	return true;
}

int PxCodeConv::MultiByteToWideChar(UINT CodePage, DWORD, PCSTR lpMultiByteStr, int cbMultiByte, PWSTR lpWideCharStr, int cchWideChar)
{
	ASSERT(lpMultiByteStr != NULL);
	char type[20];
	CodePageStr(CodePage, type);
	iconv_t cd = iconv_open("UCS-2LE", type);
	if (cd == (iconv_t) -1)
		return 0;

	size_t outbytes;
	size_t inbytesleft;
	size_t outbytesleft;
	if (cbMultiByte == -1)
        inbytesleft = strlen(lpMultiByteStr) + 1; // °üº¬ ¡®\0¡¯
	else
		inbytesleft = (size_t)cbMultiByte;

	size_t convertRes;
	if (cchWideChar == 0)
	{
		outbytesleft = inbytesleft * 2;
		outbytes = outbytesleft;
		WCHAR chTmp[outbytesleft];
		WCHAR* p = chTmp;
		convertRes = iconv(cd, (char**) &lpMultiByteStr, &inbytesleft,
				(char**) &p, &outbytesleft);
		if (convertRes == (size_t) -1)
		{
			iconv_close(cd);
			return 0;
		}
	}
	else
	{
		outbytes = ((size_t) cchWideChar) * sizeof(WCHAR);
		outbytesleft = outbytes;
		convertRes = iconv(cd, (char**) &lpMultiByteStr, &inbytesleft,
				(char**) &lpWideCharStr, &outbytesleft);
		if (convertRes == (size_t) -1)
		{
			iconv_close(cd);
			return 0;
		}
	}
	iconv_close(cd);
	return (outbytes - outbytesleft) / sizeof(WCHAR);
}

int PxCodeConv::WideCharToMultiByte(UINT CodePage, DWORD, PCWSTR lpWideCharStr, int cchWideChar, PSTR lpMultiByteStr, int cbMultiByte)
{
	ASSERT(lpWideCharStr != NULL);
	char type[20];
	if (!CodePageStr(CodePage, type))
		return 0;
	iconv_t cd = iconv_open(type, "UCS-2LE");

	if (cd == (iconv_t) -1)
	{
		return 0;
	}
	size_t outbytes;
	size_t inbytesleft;
	size_t outbytesleft;
	if (cchWideChar == -1)
	{
		int len = wstrlen16(lpWideCharStr)+ 1; // °üº¬ ¡®\0'
		inbytesleft = len * sizeof(WCHAR);
		outbytesleft = (size_t) len * 3;
	}
	else
	{
		inbytesleft = (size_t) cchWideChar * sizeof(WCHAR);
   		outbytesleft = (size_t) cchWideChar * 3;
	}

	size_t convertRes;
	if (cbMultiByte == 0)
	{
		outbytes = outbytesleft;
		char chTmp[outbytes];
		char* p = chTmp;
		convertRes = iconv(cd, (char**) &lpWideCharStr, &inbytesleft,
				(char**) &p, &outbytesleft);
		if (convertRes == (size_t) -1)
		{
			iconv_close(cd);
			return 0;
		}
	}
	else
	{
		outbytes = (size_t) cbMultiByte;
		outbytesleft = outbytes;
		convertRes = iconv(cd, (char**) &lpWideCharStr, &inbytesleft,
				(char**) &lpMultiByteStr, &outbytesleft);
		if (convertRes == (size_t) -1)
		{
			iconv_close(cd);
			return 0;
		}
	}
	iconv_close(cd);
	return (outbytes - outbytesleft);
}

bool PxCodeConv::IsDBCSLeadByteEx(UINT CodePage, BYTE TestChar)
{
	// TODO Not under tested
	return (TestChar & 0x80) != 0;
}

// -------------------------------------------------------------------------- //
#endif

CDEC_NS_END
