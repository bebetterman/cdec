// -------------------------------------------------------------------------- //
//	文件名		：	test_bitop.cpp
//	创建者		：	杨钢
//	创建时间	：	2005-4-15 15:45:02
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "../../../test_support.h"

using namespace cdec_egg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestBitOp: public UnitTestSuite
{
	UNITTEST_SUITE(TestBitOp)
		UNITTEST_METHOD(testGetMaskBits)
		UNITTEST_METHOD(testSetMaskBits)
		UNITTEST_METHOD(testUpdateBitAccording)
		UNITTEST_METHOD(test_wstrcmp2)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testGetMaskBits()
	{
		UNITTEST_ASSERT(GetMaskBits(0x5F, 0x1C) == 0x1C);
	}

	void testSetMaskBits()
	{
		UINT a = 0xCCCCCCCC;
		SetMaskBits(a, 0xF0F0F0F0, 0x69696969);
		UNITTEST_ASSERT(a == 0x6C6C6C6C);

		a = 0xCCCCCCCC;
		SetMaskBits(a, 0x69696969, 0xF0F0F0F0);
		UNITTEST_ASSERT(a == 0xE4E4E4E4);
	}

	void testUpdateBitAccording()
	{
		UINT a = 0xE3;
		UpdateBitAccording(a, 0x55, 0xFB, 0x4);
		UNITTEST_ASSERT(a == 0xA2);

		a = 0xE3;
		UpdateBitAccording(a, 0x55, 0x4, 0x4);
		UNITTEST_ASSERT(a == 0xF7);

		UNITTEST_ASSERT( IsBitIdentical(0xD8, 0x72, 0x55) );
		UNITTEST_ASSERT( !IsBitIdentical(0xFF, 0xEF, 0xF0) );
	}

	void test_wstrcmp2()
	{
		WCHAR	a[] = __X(""), b[] = __X(""),
				c[] = __X("bug"),
				d[] = __X("bugs"),
				e[] = __X("big");

#if 0
		UNITTEST_ASSERT( alg::wstrcmp2(a, b) == 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(a, NULL) == 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(NULL, b) == 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(NULL, NULL) == 0 );

		UNITTEST_ASSERT( alg::wstrcmp2(c, d) < 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(d, c) > 0 );

		UNITTEST_ASSERT( alg::wstrcmp2(d, e) > 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(e, d) < 0 );

		c[1] = 'i';
		UNITTEST_ASSERT( alg::wstrcmp2(c, e) == 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(e, c) == 0 );

		UNITTEST_ASSERT( alg::wstrcmp2(c, NULL) > 0 );
		UNITTEST_ASSERT( alg::wstrcmp2(NULL, c) < 0 );
#endif
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestBitOp);

// -------------------------------------------------------------------------- //
#endif
