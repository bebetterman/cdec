#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// -------------------------------------------------------------------------- //
// Config
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
#	pragma comment(lib, "cdecegg.lib")
#	pragma comment(lib, "cdeccore.lib")
#endif

bool cdec_PrintError = true;

// -------------------------------------------------------------------------- //
// Windows Only
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
