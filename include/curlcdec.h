#pragma once

#ifndef __CDECCORE__
#error cdeccore Expected
#endif

#ifndef __CDEC_CURL__
#define __CDEC_CURL__

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef CURLCDEC_EXPORTS
#define CURLCDECEXPORT DECLSPEC_EXPORT
#else
#define CURLCDECEXPORT DECLSPEC_IMPORT
#endif

// -------------------------------------------------------------------------- //

#include "curlcdec/curl_common.h"
#include "curlcdec/curl_response.h"
#include "curlcdec/curl_easy.h"

// -------------------------------------------------------------------------- //

#endif
