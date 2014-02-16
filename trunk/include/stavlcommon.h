#pragma once

#ifndef __CDECCORE__
#error cdeccore expected
#endif

#ifndef __STAVL_COMMON__
#define __STAVL_COMMON__

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef STAVLCOMMON_EXPORTS
#define STAVLCOMMONEXPORT DECLSPEC_EXPORT
#else
#define STAVLCOMMONEXPORT DECLSPEC_IMPORT
#endif

#include "stavlcommon/convertera.h"
#include "stavlcommon/fastlogreader.h"
#include "stavlcommon/logwriter.h"

#endif
