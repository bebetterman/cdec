#pragma once

#ifndef __CDECCORE__
#error cdeccore Expected
#endif

#ifndef __CDEC_LZMA__
#define __CDEC_LZMA__
#endif

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef LZMACDEC_EXPORTS
#define LZMACDECEXPORT DECLSPEC_EXPORT
#else
#define LZMACDECEXPORT DECLSPEC_IMPORT
#endif

// -------------------------------------------------------------------------- //

LZMACDECEXPORT int LzmaCdecCompress(
	BYTE* dest, int& destLen, const BYTE* src, int srcLen,
	BYTE* outProps, int& outPropsSize, /* *outPropsSize must be = 5 */
	int level,      /* 0 <= level <= 9, default = 5 */
	UINT dictSize,  /* default = (1 << 24) */
	int lc,        /* 0 <= lc <= 8, default = 3  */
	int lp,        /* 0 <= lp <= 4, default = 0  */
	int pb,        /* 0 <= pb <= 4, default = 2  */
	int fb,        /* 5 <= fb <= 273, default = 32 */
	int numThreads /* 1 or 2, default = 2 */
);

LZMACDECEXPORT int LzmaCdecUncompress(
	BYTE* dest, int& destLen, const BYTE* src, int& srcLen, const BYTE* props, int propsSize
);

// -------------------------------------------------------------------------- //
