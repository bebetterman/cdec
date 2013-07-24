// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "../include/cdeccore.h"
#include "../include/curlcdec.h"

#ifdef X_OS_WINDOWS
#define CURL_STATICLIB
#include "../import-win/curl-7.31.0/include/curl/curl.h"
#else
#include "curl/curl.h"
#endif
