// -------------------------------------------------------------------------- //
// ��ƽ̨��صĺ궨��
//
// �벻Ҫֱ�Ӱ������ļ���ʹ�� cdeccore.h �� predef.h
// -------------------------------------------------------------------------- //

#pragma once

// -------------------------------------------------------------------------- //
// ƽ̨����
// -------------------------------------------------------------------------- //

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#	define X_OS_WINDOWS
#	pragma message("X_OS_WINDOWS")
#	if defined(_WIN64)
#		define X_OS_WIN64
#		pragma message("X_OS_WIN64")
#	else
#		define X_OS_WIN32
#		pragma message("X_OS_WIN32")
#	endif
#elif defined(__linux__) || defined(__LINUX__) || defined(LINUX) || defined(_LINUX)
#	define X_OS_LINUX
#	pragma message("X_OS_LINUX")
#else
#	error Unsupported platform
#endif

// -------------------------------------------------------------------------- //
// ����������
// -------------------------------------------------------------------------- //

#if defined(_MSC_VER)
#	define X_CC_VC
#	pragma message("X_CC_VC")
#elif defined(__GNUG__) || defined(__GNUC__)
#	define X_CC_GCC
#	pragma message("X_CC_GCC")
#else
#	error Unsupported compiler
#endif

// -------------------------------------------------------------------------- //
// CPU ����
// -------------------------------------------------------------------------- //

#if defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64) || defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) || defined(__amd64__)
#	define X_CPU_X64
#	pragma message("X_CPU_X64")
#else
#	define X_CPU_X86
#	pragma message("X_CPU_X86")
#endif

// -------------------------------------------------------------------------- //
