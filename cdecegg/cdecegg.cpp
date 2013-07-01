// cdeccore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

// -------------------------------------------------------------------------- //
// Config
// -------------------------------------------------------------------------- //

bool cdec_PrintError = true;

// -------------------------------------------------------------------------- //
// DLL Main Entry (WIN32)
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
