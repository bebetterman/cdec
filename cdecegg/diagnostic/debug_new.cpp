#include "stdafx.h"

#if Cdec_Enable_DebugNew
// -------------------------------------------------------------------------- //

CDECEGGEXPORT void* CdecDebugMalloc(size_t size, const char* file, int line)
{
	return (BYTE*)malloc(size + 16) + 8;
}

CDECEGGEXPORT void CdecDebugFree(void* p, const char* file, int line)
{
	free((BYTE*)p - 8);
}

// -------------------------------------------------------------------------- //
#endif
