// -------------------------------------------------------------------------- //
// 基础类型
//
// 请不要直接包含本文件，使用 cdeccore.h 或 predef.h
// -------------------------------------------------------------------------- //

#pragma once

// -------------------------------------------------------------------------- //
// 基本值类型
// -------------------------------------------------------------------------- //

typedef unsigned int UINT;

typedef signed char INT8;
typedef unsigned char UINT8;

typedef short int INT16;
typedef unsigned short int UINT16;

typedef int INT32;
typedef unsigned int UINT32;

#if defined(X_CC_VC)

typedef __int64 INT64;
typedef unsigned __int64 UINT64;

#else

typedef long long INT64;
typedef unsigned long long UINT64;

#endif

typedef UINT8 BYTE;
typedef UINT16 WORD;

#if defined(X_CC_VC)

typedef unsigned long DWORD;

#else

typedef UINT32 DWORD;

#endif

// -------------------------------------------------------------------------- //
// 指针类型
// -------------------------------------------------------------------------- //

#ifdef X_CPU_X86

typedef int INT_PTR;
typedef UINT UINT_PTR;

#else

typedef INT64 INT_PTR;
typedef UINT64 UINT_PTR;

#endif

// -------------------------------------------------------------------------- //
// UCS-2 宽字符类型
// -------------------------------------------------------------------------- //

#if defined(X_CC_VC)
	typedef wchar_t WCHAR;
#	define __X(str)	L ## str
#elif defined(CDEC_DISABLE_CPP0X)
	typedef WORD	WCHAR;
#else
	typedef char16_t WCHAR;	// C++ 0x
#	define __X(str)	u ## str
#endif

typedef char *PSTR;
typedef const char* PCSTR;
typedef WCHAR *PWSTR;
typedef const WCHAR *PCWSTR;

// -------------------------------------------------------------------------- //
