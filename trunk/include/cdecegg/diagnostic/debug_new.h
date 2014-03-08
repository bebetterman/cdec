// -------------------------------------------------------------------------- //
// Diagnostic - Debug New
// -------------------------------------------------------------------------- //

#pragma once

#ifdef _DEBUG
#	define	Cdec_Enable_DebugNew	1
#else
#	define	Cdec_Enable_DebugNew	0
#endif

// -------------------------------------------------------------------------- //

#if Cdec_Enable_DebugNew

CDECEGGEXPORT void* CdecDebugMalloc(size_t size, const char* file, int line);
CDECEGGEXPORT void CdecDebugFree(void* p, const char* file, int line);

// default operators
inline void* operator new(size_t size) { return CdecDebugMalloc(size, NULL, 0); }
inline void* operator new[](size_t size) { return CdecDebugMalloc(size, NULL, 0); }
inline void operator delete(void* p) { CdecDebugFree(p, NULL, 0); }
inline void operator delete[](void* p) { CdecDebugFree(p, NULL, 0); }

// no-throw operators
inline void* operator new(size_t size, const std::nothrow_t&) throw() { return CdecDebugMalloc(size, NULL, 0); }
inline void* operator new[](size_t size, const std::nothrow_t&) throw() { return CdecDebugMalloc(size, NULL, 0); }
inline void operator delete(void* p, const std::nothrow_t&) { CdecDebugFree(p, NULL, 0); }
inline void operator delete[](void* p, const std::nothrow_t&) { CdecDebugFree(p, NULL, 0); }

// placement operators
inline void* operator new(size_t size, const char* file, int line) { return CdecDebugMalloc(size, file, line); }
inline void* operator new[](size_t size, const char* file, int line) { return CdecDebugMalloc(size, file, line); }
inline void operator delete(void* p, const char* file, int line) { CdecDebugFree(p, file, line); }
inline void operator delete[](void* p, const char* file, int line) { CdecDebugFree(p, file, line); }

#define DEBUG_NEW new(__FILE__, __LINE__)

#else

#define DEBUG_NEW new

#endif

// -------------------------------------------------------------------------- //
