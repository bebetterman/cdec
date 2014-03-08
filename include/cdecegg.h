#pragma once

#ifndef __CDEC_EGG__
#define __CDEC_EGG__

// Define this macro to enable Cdec EGG internal library
#define USE_EGG_INTERNAL

// -------------------------------------------------------------------------- //
// Predef
// -------------------------------------------------------------------------- //

#include "cdecegg/predef/platform.h"
#include "cdecegg/predef/stdlibs.h"
#include "cdecegg/predef/stdtypes.h"
#include "cdecegg/predef/stddef.h"

#ifdef CDECEGG_EXPORTS
#	define CDECEGGEXPORT		DECLSPEC_EXPORT
#else
#	define CDECEGGEXPORT		DECLSPEC_IMPORT
#endif

#include "cdecegg/predef/diagnostics.h"

#include "cdecegg/diagnostic/debug_new.h"

#include "cdecegg/predef/exception.h"
#include "cdecegg/predef/wstring16.h"

// -------------------------------------------------------------------------- //
// Toolkit - Value Manipulation Functions
// -------------------------------------------------------------------------- //

#include "cdecegg/common/basic/bitop.h"
#include "cdecegg/common/basic/bitorg.h"

// -------------------------------------------------------------------------- //
#endif
