#pragma once

#if defined(_DEBUG) || ENABLE_RELEASE_TEST
#	define ENABLE_TEST	1
#else
#	define ENABLE_TEST	1
#endif

#if ENABLE_TEST

// -------------------------------------------------------------------------- //

#define _MODULE_NAME	stavlcommon
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
		static stringx pathSampleBase = __X("..\\tests\\samples\\");
#else
		static stringx pathSampleBase = __X("../tests/samples/");
#endif
		return pathSampleBase + fname;
	}

	static void AssertByteRangeEqual(const void* ps, const void* pt, int c)
	{
		UNITTEST_ASSERT(memcmp(ps, pt, c) == 0);
	}

	static void AssertByteRangeEqual(ref<ByteArray> s, int offS, const void* pt, int c);

	static void AssertByteArrayEqual(ref<ByteArray> s, const void* pt, int ct);
};

// -------------------------------------------------------------------------- //

#endif
