#pragma once

// -------------------------------------------------------------------------- //
// Assertion
// -------------------------------------------------------------------------- //

CDECEGGEXPORT void CdecDiagAssertReport(char* file, int line, char* msg);

#if defined(X_CC_VC)

#	define ASSERT _ASSERTE

#else

inline void _cdec_assert(bool expr, char* file, int line, char* msg)
{
	if (!expr)
		CdecDiagAssertReport(file, line, msg);
}

#	if defined(_DEBUG)
#		define ASSERT(e) _cdec_assert((e), __FILE__, __LINE__, #e)
#	else
#		define ASSERT(e) ((void)0)
#	endif

#endif

// -------------------------------------------------------------------------- //
