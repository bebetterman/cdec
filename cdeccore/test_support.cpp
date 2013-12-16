#include "stdafx.h"
#include "test_support.h"

#if ENABLE_TEST

// -------------------------------------------------------------------------- //
// Config
// -------------------------------------------------------------------------- //

// Turn off error message when running unit tests
UNITTEST_MODULE_IMPLEMENT_(cdec_PrintError = false)

WCHAR TEXT_ChsLine2[] = { 0x4E2D, 0x6587, 0xFF1A, 0x7B2C, 0x32, 0x884C, 0 };	// "中文：第2行"
WCHAR TEXT_Chs[] = { 0x4E2D, 0x6587, 0 }; // "中文"

char	TEXT_ChsEng_UTF8[] = "\xE4\xB8\xAD\xE6\x96\x87\x41\x42\x43";	// 中文ABC
WCHAR	TEXT_ChsEng_Wide[] = { 0x4E2D, 0x6587, 'A', 'B', 'C', 0 };

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
