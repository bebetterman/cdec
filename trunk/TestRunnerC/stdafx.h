// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#include "../include/cdecegg/predef/platform.h"
#include "../include/cdecegg/predef/stdlibs.h"
#include "../include/cdecegg/predef/stdtypes.h"
#include "../include/cdecegg/predef/stddef.h"

#include <cassert>
#define ASSERT assert

#include <stdio.h>
#include <string.h>

#ifdef X_CC_VC
#include <conio.h>
#endif

#include <string>
#include <vector>

#include "../include/unittest.h"
using namespace UnitTest;

#ifdef X_OS_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
typedef void* HMODULE;
#endif
