#pragma once

#if defined(_DEBUG) || ENABLE_RELEASE_TEST
#	define ENABLE_TEST	1
#else
#	define ENABLE_TEST	0
#endif

#if ENABLE_TEST

// -------------------------------------------------------------------------- //

#define _MODULE_NAME	cdecegg
#include "../include/unittest.h"

using namespace UnitTest;

// -------------------------------------------------------------------------- //

template<class _Ty>
struct ObjectRefSpy: _Ty
{
	using _Ty::_ref;

	static UINT GetRef(_Ty* pThis)
	{
		return ((ObjectRefSpy<_Ty >*)pThis)->_ref;
	}
};

// -------------------------------------------------------------------------- //

#endif
