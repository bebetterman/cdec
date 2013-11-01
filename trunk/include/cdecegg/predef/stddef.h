// -------------------------------------------------------------------------- //
// ��������
//
// �벻Ҫֱ�Ӱ������ļ���ʹ�� cdeccore.h �� predef.h
// -------------------------------------------------------------------------- //

#pragma once

// -------------------------------------------------------------------------- //
// α�ؼ���
// -------------------------------------------------------------------------- //

// interface - ����ӿ�
#define interface struct

// argument modifiers
#define IN
#define OUT
#define REF

// __novtable
#ifndef __novtable
#	if defined(X_CC_VC)
#		define __novtable		__declspec(novtable)
#	else
#		define __novtable
#	endif
#endif

// __forceinline
#ifdef X_CC_VC
#else
#   define __forceinline __inline__
#endif

// ___noop - �պ���
#ifdef X_CC_VC
	// VC �ṩ�� __noop �ؼ���
#else
	inline int _cdec_noop(...) { return 0; }
#	define __noop _cdec_noop
#endif

// -------------------------------------------------------------------------- //
// ��������
// -------------------------------------------------------------------------- //

#ifndef EXTERN_C
#	ifdef __cplusplus
#		define EXTERN_C				extern "C"
#	else
#		define EXTERN_C
#	endif
#endif

#ifdef X_CC_GCC
#	ifndef __fastcall
#		define __fastcall __attribute__((fastcall))
#	endif
#	ifndef __stdcall
#		define __stdcall __attribute__((stdcall))
#	endif
#	ifndef __cdecl
#		define __cdecl __attribute__((cdecl))
#	endif
#endif

#ifdef X_CC_GCC
//#	define DECLSPEC_IMPORT	__attribute__ (visibility("default"))
//#	define DECLSPEC_EXPORT	__attribute__ (visibility("default"))
#   define DECLSPEC_IMPORT
#   define DECLSPEC_EXPORT
#else
#	define DECLSPEC_IMPORT	__declspec(dllimport)
#	define DECLSPEC_EXPORT	__declspec(dllexport)
#endif

// -------------------------------------------------------------------------- //
// Namespace
// -------------------------------------------------------------------------- //

#define CDEC_NS_BEGIN		namespace cdec {
#define CDEC_NS_END			}

#define CDEC_EGG_NS_BEGIN	namespace cdec_egg {
#define CDEC_EGG_NS_END		}

// -------------------------------------------------------------------------- //
