#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestConverter: public UnitTestSuite
{
	UNITTEST_SUITE(TestConverter)
		UNITTEST_METHOD(testStrToInt32)
		UNITTEST_METHOD(testStrToInt64)
		UNITTEST_METHOD(testInt32ToStr)
		UNITTEST_METHOD(testFromHexString)
		UNITTEST_METHOD(testToHexString)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStrToInt32()
	{
		UNITTEST_ASSERT(Converter::ToInt32(__X("0")) == 0);
		UNITTEST_ASSERT(Converter::ToInt32(__X("123")) == 123);
		
		UNITTEST_ASSERT(Converter::ToInt32(__X("1001011000111110"), 2) == 0x963E);
		UNITTEST_ASSERT(Converter::ToInt32(__X("330face"), 16) == 0x330FACE);

		UNITTEST_ASSERT(Converter::ToUInt32(__X("4294967295")) == 0xFFFFFFFF);
	}

	void testStrToInt64()
	{
		UNITTEST_ASSERT(Converter::ToInt64(__X("0")) == 0);
		UNITTEST_ASSERT(Converter::ToInt64(__X("9223372036854775807")) == 0x7FFFFFFFFFFFFFFFL);
		UNITTEST_ASSERT(Converter::ToUInt64(__X("9223372036854775808")) == 0x8000000000000000UL);
		UNITTEST_ASSERT(Converter::ToUInt64(__X("18446744073709551615")) == 0xFFFFFFFFFFFFFFFFUL);

		UNITTEST_ASSERT(Converter::ToInt64(__X("7FFFFFFFFFFFFFFF"), 16) == 0x7FFFFFFFFFFFFFFFL);
		UNITTEST_ASSERT(Converter::ToUInt64(__X("8000000000000000"), 16) == 0x8000000000000000UL);
		UNITTEST_ASSERT(Converter::ToUInt64(__X("FFFFFFFFffffffff"), 16) == 0xFFFFFFFFFFFFFFFFUL);
	}

	void testInt32ToStr()
	{
		UNITTEST_ASSERT(Converter::ToString((int)0) == __X("0"));
		UNITTEST_ASSERT(Converter::ToString((int)9) == __X("9"));
		UNITTEST_ASSERT(Converter::ToString((int)681) == __X("681"));

		UNITTEST_ASSERT(Converter::ToString((UINT)0) == __X("0"));
		UNITTEST_ASSERT(Converter::ToString((UINT)9) == __X("9"));
		UNITTEST_ASSERT(Converter::ToString((UINT)681) == __X("681"));
	}

	void testFromHexString()
	{
		ref<ByteArray> r = Converter::FromHexString(__X("FACE00"));
		UNITTEST_ASSERT(r->Count() == 3 && r->at(0) == 0xFA && r->at(1) == 0xCE && r->at(2) == 0);

		r = gc_new<ByteArray>(4);
		UNITTEST_ASSERT(Converter::FromHexString(__X("cafe80"), r, 1) == 3);
		UNITTEST_ASSERT(r->at(0) == 0 && r->at(1) == 0xCA && r->at(2) == 0xFE && r->at(3) == 0x80);

		BYTE buf;
		UNITTEST_ASSERT(Converter::FromHexString(__X("3A"), 2, &buf, 1) == 1);
		UNITTEST_ASSERT(buf == 0x3A);
	}

	void testToHexString()
	{
		BYTE rs[] = { 0xBA, 0xBE, 3, 0x30 };
		ref<ByteArray> r = gc_new<ByteArray>(rs, sizeof(rs));

		UNITTEST_ASSERT(Converter::ToHexString(r) == __X("babe0330"));
		UNITTEST_ASSERT(Converter::ToHexString(r, 1, 2) == __X("be03"));

		std::wstring16 s = __X("S");
		Converter::ToHexString(rs, 2, s);
		UNITTEST_ASSERT(s == __X("Sbabe"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestConverter);

// -------------------------------------------------------------------------- //
#endif
