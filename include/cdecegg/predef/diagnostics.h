#pragma once

// -------------------------------------------------------------------------- //
// Assertion
// -------------------------------------------------------------------------- //

CDECEGGEXPORT void CdecDiagAssertReport(bool expr, char* file, int line, char* msg);

#if defined(X_CC_VC)
#	define ASSERT _ASSERTE
#else

inline void _cdec_assert(char* file, int line, char* msg)
{
	if (!expr)
		CdecDiagAssertReport(file, line, msg);
}

#	define ASSERT(e) _cdec_assert((e), __FILE__, __LINE__, #e)
#endif

// -------------------------------------------------------------------------- //
