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

class TestBitOrg : public UnitTestSuite
{
	UNITTEST_SUITE(TestBitOrg)
		UNITTEST_METHOD(testGetHighTrueBit)
		UNITTEST_METHOD(testGetBitMask)
		UNITTEST_METHOD(testGetMaxPow2LE)
		UNITTEST_METHOD(testGetBitCount)
		UNITTEST_METHOD(testRound)
	UNITTEST_SUITE_END()
	
public:
	void setUp()
	{
	}

	void testGetHighTrueBit()
	{
		UNITTEST_ASSERT(GetHighTrueBit(0) == 0);
		UNITTEST_ASSERT(GetHighTrueBit(1) == 0);
		UNITTEST_ASSERT(GetHighTrueBit(0x3888) == 13);
		UNITTEST_ASSERT(GetHighTrueBit(0x4786) == 14);
		UNITTEST_ASSERT(GetHighTrueBit(0x37182EC) == 25);
		UNITTEST_ASSERT(GetHighTrueBit(0xC0B0D0A0) == 31);
		UNITTEST_ASSERT(GetHighTrueBit(-1) == 31);

		UNITTEST_ASSERT(GetLowTrueBit(0) == 31);
		UNITTEST_ASSERT(GetLowTrueBit(1) == 0);
		UNITTEST_ASSERT(GetLowTrueBit(0x8080) == 7);
		UNITTEST_ASSERT(GetLowTrueBit(0x4786C00) == 10);
		UNITTEST_ASSERT(GetLowTrueBit(0x30000000) == 28);
		UNITTEST_ASSERT(GetLowTrueBit(0x80000000) == 31);
		UNITTEST_ASSERT(GetLowTrueBit(-1) == 0);

		UNITTEST_ASSERT(GetHighFalseBit(0) == 31);
		UNITTEST_ASSERT(GetHighFalseBit(0xCAFE0000) == 29);
		UNITTEST_ASSERT(GetHighFalseBit(0xFACE0000) == 26);
		UNITTEST_ASSERT(GetHighFalseBit(0xFFFFFF3C) == 7);
		UNITTEST_ASSERT(GetHighFalseBit(0xFFFFFFFE) == 0);
		UNITTEST_ASSERT(GetHighFalseBit(0xFFFFFFFF) == 0);

		UNITTEST_ASSERT(GetLowFalseBit(0) == 0);
		UNITTEST_ASSERT(GetLowFalseBit(0x353) == 2);
		UNITTEST_ASSERT(GetLowFalseBit(0x3DFFFFF) == 21);
		UNITTEST_ASSERT(GetLowFalseBit(0x7FFFFFFF) == 31);
		UNITTEST_ASSERT(GetLowFalseBit(0xFFFFFFFF) == 31);
	}

	void testGetBitMask()
	{
		UNITTEST_ASSERT(GetBitMask(0) == 1);
		UNITTEST_ASSERT(GetBitMask(4) == 0x10);
		UNITTEST_ASSERT(GetBitMask(31) == 0x80000000);

		UNITTEST_ASSERT(GetAccuHigh(0) == 0);
		UNITTEST_ASSERT(GetAccuHigh(1) == 0x80000000);
		UNITTEST_ASSERT(GetAccuHigh(7) == 0xFE000000);
		UNITTEST_ASSERT(GetAccuHigh(32) == 0xFFFFFFFF);

		UNITTEST_ASSERT(GetAccuLow(0) == 0);
		UNITTEST_ASSERT(GetAccuLow(1) == 1);
		UNITTEST_ASSERT(GetAccuLow(7) == 0x7F);
		UNITTEST_ASSERT(GetAccuLow(32) == 0xFFFFFFFF);
	}

	void testGetMaxPow2LE()
	{
		UNITTEST_ASSERT(GetMaxPow2LE(0) == 0);
		UNITTEST_ASSERT(GetMaxPow2LE(1) == 1);
		UNITTEST_ASSERT(GetMaxPow2LE(0x7C) == 0x40);
		UNITTEST_ASSERT(GetMaxPow2LE(0x80) == 0x80);
		UNITTEST_ASSERT(GetMaxPow2LE(0x81) == 0x80);
		UNITTEST_ASSERT(GetMaxPow2LE(0xFFFFFFFF) == 0x80000000);

		UNITTEST_ASSERT(GetMinPow2GE(0) == 1);
		UNITTEST_ASSERT(GetMinPow2GE(1) == 1);
		UNITTEST_ASSERT(GetMinPow2GE(0x7C) == 0x80);
		UNITTEST_ASSERT(GetMinPow2GE(0x80) == 0x80);
		UNITTEST_ASSERT(GetMinPow2GE(0x81) == 0x100);
		UNITTEST_ASSERT(GetMinPow2GE(0x80000000) == 0x80000000);
		UNITTEST_ASSERT(GetMinPow2GE(0x80000001) == 0);
	}

	void testGetBitCount()
	{
		UNITTEST_ASSERT(GetBitCount(0) == 0);
		for (int i = 0; i < 32; ++i)
			UNITTEST_ASSERT(GetBitCount(GetBitMask(i)) == 1);
		UNITTEST_ASSERT(GetBitCount(0xFFFFFFFF) == 32);

		UINT source[] = 
		{
			0x0000FFFF,	0xFFFF0000,	0x00FF00FF,	0xFF00FF00,	0x0F0F0F0F, 0xF0F0F0F0,	0x33333333, 0xCCCCCCCC,
			0x55555555, 0xAAAAAAAA, 0xFFFFFFFF,	0x12345678
		};

		UINT results[] = 
		{
			16,			16,			16,			16,			16,			16,			16,			16,
			16,			16,			32,			13
		};

		int n = sizeof(source) / sizeof(source[0]);
		for (int i = 0; i < n; ++i)
			UNITTEST_ASSERT(GetBitCount(source[i]) == results[i]);
	}

	void testRound()
	{
		UNITTEST_ASSERT(RoundUp2(0x3701, 0x400) == 0x3800);
		UNITTEST_ASSERT(RoundUp2(0x37FF, 0x400) == 0x3800);
		UNITTEST_ASSERT(RoundUp2(0x3800, 0x400) == 0x3800);

		UNITTEST_ASSERT(RoundUpN(73, 9) == 81);
		UNITTEST_ASSERT(RoundUpN(80, 9) == 81);
		UNITTEST_ASSERT(RoundUpN(81, 9) == 81);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestBitOrg);

// -------------------------------------------------------------------------- //
#endif
