#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestBitConverter: public UnitTestSuite
{
	UNITTEST_SUITE(TestBitConverter)
		UNITTEST_METHOD(testReadInt)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testReadInt()
	{
		BYTE s[] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80 };
		ref<ByteArray> a = gc_new<ByteArray>(s, 8);

		UNITTEST_ASSERT(BitConverter::ReadInt16(a, 6) == (INT16)0x8070);
		UNITTEST_ASSERT(BitConverter::ReadUInt16(a, 6) == 0x8070);

		UNITTEST_ASSERT(BitConverter::ReadInt32(a, 4) == 0x80706050);
		UNITTEST_ASSERT(BitConverter::ReadUInt32(a, 4) == 0x80706050);

		UNITTEST_ASSERT(BitConverter::ReadInt64(a, 0) == 0x8070605040302010);
		UNITTEST_ASSERT(BitConverter::ReadUInt64(a, 0) == 0x8070605040302010);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestBitConverter);

// -------------------------------------------------------------------------- //
#endif
