#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Wrap functions
// -------------------------------------------------------------------------- //

class PxCodeConv
{
public:
	static UINT	GetACP();
	static int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, PCSTR lpMultiByteStr, int cbMultiByte, PWSTR lpWideCharStr, int cchWideChar);
	static int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, PCWSTR lpWideCharStr, int cchWideChar, PSTR lpMultiByteStr, int cbMultiByte);
	static bool IsDBCSLeadByteEx(UINT CodePage, BYTE TestChar);
};

// -------------------------------------------------------------------------- //
// Inline implementions
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

inline UINT PxCodeConv::GetACP() { return ::GetACP(); }

inline int PxCodeConv::MultiByteToWideChar(UINT CodePage, DWORD dwFlags, PCSTR lpMultiByteStr, int cbMultiByte, PWSTR lpWideCharStr, int cchWideChar)
{
	return ::MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
}

inline int PxCodeConv::WideCharToMultiByte(UINT CodePage, DWORD dwFlags, PCWSTR lpWideCharStr, int cchWideChar, PSTR lpMultiByteStr, int cbMultiByte)
{
	// Default char is not used
	return ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, NULL, NULL);
}

inline bool PxCodeConv::IsDBCSLeadByteEx(UINT CodePage, BYTE TestChar) { return ::IsDBCSLeadByteEx(CodePage, TestChar) != 0; }

#else

inline UINT PxCodeConv::GetACP() { return Encoding::UTF8; }

#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
