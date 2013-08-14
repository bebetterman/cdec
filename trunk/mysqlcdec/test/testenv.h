#pragma once

#if defined(_DEBUG) || ENABLE_RELEASE_TEST
#	define ENABLE_TEST	1
#else
#	define ENABLE_TEST	0
#endif

#if ENABLE_TEST

// -------------------------------------------------------------------------- //

#define _MODULE_NAME	mysqlcdec
#include "../../include/unittest.h"

using namespace UnitTest;
using namespace cdec;

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
class TestEnv
{
public:
	// 获取测试样张目录
	static stringx get_sample_path(stringx fname)
	{
#ifdef X_OS_WINDOWS
		static stringx pathSampleBase = __X("..\\mysqltests\\");
#else
		static stringx pathSampleBase = __X("../mysqltests/");
#endif
		return pathSampleBase + fname;
	}
};
#endif
