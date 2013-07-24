// curlcdec.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#pragma comment(lib, "cdecegg")
#pragma comment(lib, "cdeccore")

#if defined(X_OS_WIN32)
#	ifdef _DEBUG
#		pragma comment(lib, "libcurl_vc10_static_debug")
#	else
#	endif
#elif defined(X_OS_WIN64)
#endif

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
