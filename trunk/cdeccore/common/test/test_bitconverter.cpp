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
		UNITTEST_METHOD(testExchange)
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

	void testExchange()
	{
		UNITTEST_ASSERT(BitConverter::ExchangeUInt16(0x1234) == 0x3412);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt16(0xCDEF) == 0xEFCD);
		UNITTEST_ASSERT(BitConverter::ExchangeInt16(0x1234) == 0x3412);
		UNITTEST_ASSERT(BitConverter::ExchangeInt16(0xCDEF) == (INT16)0xEFCD);

		UNITTEST_ASSERT(BitConverter::ExchangeUInt16(0xCA) == 0xCA00);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt16(0xCA00) == 0xCA);

		UNITTEST_ASSERT(BitConverter::ExchangeUInt32(0x12345678) == 0x78563412);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt32(0x89ABCDEF) == 0xEFCDAB89);
		UNITTEST_ASSERT(BitConverter::ExchangeInt32(0x12345678) == 0x78563412);
		UNITTEST_ASSERT(BitConverter::ExchangeInt32(0x89ABCDEF) == 0xEFCDAB89);

		UNITTEST_ASSERT(BitConverter::ExchangeUInt32(0xCA) == 0xCA000000);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt32(0xCA0000) == 0xCA00);

		UNITTEST_ASSERT(BitConverter::ExchangeUInt64(0x0123456789ABCDEF) == 0xEFCDAB8967452301);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt64(0xEFCDAB8967452301) == 0x0123456789ABCDEF);
		UNITTEST_ASSERT(BitConverter::ExchangeInt64(0x0123456789ABCDEF) == 0xEFCDAB8967452301);
		UNITTEST_ASSERT(BitConverter::ExchangeInt64(0xEFCDAB8967452301) == 0x0123456789ABCDEF);

		UNITTEST_ASSERT(BitConverter::ExchangeUInt64(0xCA) == 0xCA00000000000000);
		UNITTEST_ASSERT(BitConverter::ExchangeUInt64(0xCA0000000000) == 0xCA0000);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestBitConverter);

// -------------------------------------------------------------------------- //
#endif
