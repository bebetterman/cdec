// cdeccore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

// -------------------------------------------------------------------------- //
// Config
// -------------------------------------------------------------------------- //

#pragma comment(lib, "cdecegg.lib")

bool cdec_PrintError = true;

// -------------------------------------------------------------------------- //
// Core Memory
// -------------------------------------------------------------------------- //

CDECCOREAPI(void*) CoreAllocT(size_t cbSize)
{
	return malloc(cbSize);
}

CDECCOREAPI(void) CoreFreeT(void* p, size_t)
{
	free(p);
}

CDECCOREAPI(void*) CoreAllocLC(size_t cbSize)
{
	return malloc(cbSize);
}

CDECCOREAPI(void) CoreFreeLC(void* p, size_t)
{
	free(p);
}

// -------------------------------------------------------------------------- //
// DLL Ö÷º¯Êý
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
