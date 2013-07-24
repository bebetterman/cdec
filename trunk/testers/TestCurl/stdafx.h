// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
#	define CURL_STATICLIB
#	include <Windows.h>
#	include "../../import-win/curl-7.31.0/include/curl/curl.h"
#else
#	include <curl/curl.h>
#endif

#include <assert.h>
#define UNITTEST_ASSERT(f)	assert(f)

#include <string>
