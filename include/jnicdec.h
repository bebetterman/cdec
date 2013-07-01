#pragma once

// -------------------------------------------------------------------------- //
// JNI Kit
//
// JNI Kit in KFC provides a set of utility classes and functions that helps
// easier writing JNI functions.
//
// However, kfc.dll contains non of this utilities.
// -------------------------------------------------------------------------- //

inline void* PoolAlloc(UINT cb) { return malloc(cb); }
inline void PoolFree(void* p, UINT) { free(p); }

#include "jnicdec/valuebag.h"
#include "jnicdec/objectpool.h"

// -------------------------------------------------------------------------- //
