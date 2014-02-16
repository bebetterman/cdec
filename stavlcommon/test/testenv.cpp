#include "stdafx.h"
#include "testenv.h"

#if ENABLE_TEST

// -------------------------------------------------------------------------- //
// Config
// -------------------------------------------------------------------------- //

// Turn off error message when running unit tests
UNITTEST_MODULE_IMPLEMENT_(cdec_PrintError = false)

// -------------------------------------------------------------------------- //

void TestEnv::AssertByteRangeEqual(ref<ByteArray> s, int offS, const void* pt, int c)
{
	UNITTEST_ASSERT(offS + c <= s->Count());
	const BYTE* ptt = (const BYTE*)pt;
	for (int i = 0; i < c; ++i)
		UNITTEST_ASSERT(s->at(offS + i) == ptt[i]);
}

void TestEnv::AssertByteArrayEqual(ref<ByteArray> s, const void* pt, int ct)
{
	UNITTEST_ASSERT(s->Count() == ct);
	AssertByteRangeEqual(s, 0, pt, ct);
}

// -------------------------------------------------------------------------- //

#endif
