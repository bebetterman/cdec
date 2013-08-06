#include "stdafx.h"
#include "../include/lzmacdec.h"
#include "lzmalib.h"

// -------------------------------------------------------------------------- //
// IMPORTANT
//
// Building lzmalib before this project.
//
// Under Windows
//   Open import-win\lzma920\C\Util\LzmaLib\LzmaLib.vcxproj
//   Run build
//   * LzmaLib.vcxproj is for Visual Studio 2010 (10) or later
//
// Under Linux
//   cd import-win/lzma920/C/Util/LzmaLib
//   make -f makefile-a.gcc (static library, preferred)
//   make -f makefile.gcc (shared library)
//   * Choose either of static library or shared library but not both.
//     The static library is preferred.
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
#pragma comment(lib, "cdecegg")
#pragma comment(lib, "cdeccore")
#pragma comment(lib, "LzmaLib")
#endif

// -------------------------------------------------------------------------- //

int LzmaCdecCompress(
	BYTE* dest, int& destLen, const BYTE* src, int srcLen, BYTE* outProps, int& outPropsSize, 
	int level, UINT dictSize, int lc, int lp, int pb, int fb, int numThreads
	)
{
	size_t destLen_st = destLen;
	size_t outPropsSize_st = outPropsSize;

	int ret = LzmaCompress(
		dest, &destLen_st, src, srcLen, outProps, &outPropsSize_st, 
		level, dictSize, lc, lp, pb, fb, numThreads
		);

	destLen = (int)destLen_st;
	outPropsSize = (int)outPropsSize_st;
	return ret;
}

int LzmaCdecUncompress(BYTE* dest, int& destLen, const BYTE* src, int& srcLen, const BYTE* props, int propsSize)
{
	size_t destLen_st = destLen;
	size_t srcLen_st = srcLen;

	int ret = LzmaUncompress(dest, &destLen_st, src, &srcLen_st, props, propsSize);

	destLen = (int)destLen_st;
	srcLen = (int)srcLen_st;
	return ret;
}

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif
