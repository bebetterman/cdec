#pragma once

#ifndef __CDECCORE__
#error cdeccore Expected
#endif

#ifndef __CDEC_ZLIB__
#define __CDEC_ZLIB__
#endif

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef ZLIBCDEC_EXPORTS
#define ZLIBCDECEXPORT		DECLSPEC_EXPORT
#else
#define ZLIBCDECEXPORT		DECLSPEC_IMPORT
#endif

// -------------------------------------------------------------------------- //
CDEC_NS_BEGIN

class ZLIBCDECEXPORT ZlibCdecUtility
{
public:
	static int Uncompress(const void* pSrc, int lenSrc, void* pTag, int lenTag);

	static ref<ByteArray> Uncompress(ref<ByteArray> data, int offset, int count);

	static ref<ByteArray> Uncompress(ref<ByteArray> data)
	{
		return Uncompress(data, 0, data->Count());
	}
};

CDEC_NS_END
// -------------------------------------------------------------------------- //
