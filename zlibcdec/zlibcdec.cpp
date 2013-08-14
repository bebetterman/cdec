#include "stdafx.h"
#include "../include/zlibcdec.h"
#ifdef X_OS_WINDOWS
#include "../import-win/zlib128/include/zlib.h"
#else
#include <zlib.h>
#endif

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
#pragma comment(lib, "cdecegg")
#pragma comment(lib, "cdeccore")
#pragma comment(lib, "zdll")
#endif

bool cdec_PrintError = true;

// -------------------------------------------------------------------------- //
CDEC_NS_BEGIN

// This function returns the length of actual output (uncompressed) data, unless one of the
//   exceptions below raised.
//
// This function might throw the following ZlibException
// - EC_OutOfBuffer : The specified destination buffer is not large enough to hold the output data
//					  In this case, the buffer will be filled up to the capacity.
// - EC_OutOfMemory : Not enough memory.
// - EC_EC_InvalidValue : The input data was corrupted or incomplete.
//
int ZlibCdecUtility::Uncompress(const void* pSrc, int lenSrc, void* pDest, int lenDest)
{
	uLongf lenDist_l = lenDest;
	int code = uncompress((BYTE*)pDest, &lenDist_l, (const BYTE*)pSrc, lenSrc);
	if (code == Z_OK)
		return lenDist_l;
	else if (code == Z_BUF_ERROR)
		cdec_throw(Exception(EC_OutOfBuffer));
	else if (code == Z_MEM_ERROR)
		cdec_throw(Exception(EC_OutOfMemory));
	else if (code == Z_DATA_ERROR)
		cdec_throw(Exception(EC_InvalidValue));
	else
		cdec_throw(Exception(EC_Fail));
}

// Note that this function is not well-done
// It might throw exception of Out Of Buffer
ref<ByteArray> ZlibCdecUtility::Uncompress(ref<ByteArray> data, int offset, int length)
{
	if (CheckOutOfRange(offset, length, data->Count()))
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pinSrc = data->GetBuffer();
	const int BufferSize = 0x100000;	// 1MB buffer
	void* buffer = CoreAllocLC(BufferSize);
	int output = Uncompress(pinSrc.ptr() + offset, length, buffer, BufferSize);
	// If exception thrown, memory leaked

	ref<ByteArray> r = gc_new<ByteArray>((const BYTE*)buffer, output);
	CoreFreeLC(buffer, BufferSize);
	return r;
}

CDEC_NS_END
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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

// -------------------------------------------------------------------------- //
