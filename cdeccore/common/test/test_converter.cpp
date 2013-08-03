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
		UNITTEST_METHOD(testFromHexString1)
		UNITTEST_METHOD(testFromHexString2)
		UNITTEST_METHOD(testFromHexString3)
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

		UNITTEST_ASSERT(Converter::ToInt32(__X("+1")) == 1);
		UNITTEST_ASSERT(Converter::ToInt32(__X("-1")) == -1);

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

		UNITTEST_ASSERT(Converter::ToString((int)-1) == __X("-1"));

		UNITTEST_ASSERT(Converter::ToString((UINT)0) == __X("0"));
		UNITTEST_ASSERT(Converter::ToString((UINT)9) == __X("9"));
		UNITTEST_ASSERT(Converter::ToString((UINT)681) == __X("681"));
	}

	void testFromHexString1()
	{
		// int FromHexString(PCWSTR str, int cch, BYTE* bytes, int length);
		BYTE buf[16];
		memset(buf, 0x33, sizeof(buf));
		UNITTEST_ASSERT(Converter::FromHexString(__X("cafe88"), 6, buf + 1, 3) == 3);
		TestEnv::AssertByteRangeEqual(buf, "\x33\xca\xfe\x88\x33\x33", 6);

		// int FromHexString(stringx str, int pos, int cch, BYTE* bytes, int length);
		memset(buf, 0x33, sizeof(buf));
		UNITTEST_ASSERT(Converter::FromHexString(__X("abeef"), 1, 4, buf + 1, 2) == 2);
		TestEnv::AssertByteRangeEqual(buf, "\x33\xbe\xef\x33", 4);

		// int FromHexString(stringx str, BYTE* bytes, int length);
		memset(buf, 0x33, sizeof(buf));
		UNITTEST_ASSERT(Converter::FromHexString(__X("3A"), buf + 1, 2) == 1);
		TestEnv::AssertByteRangeEqual(buf, "\x33\x3A\x33\x33", 4);
	}

	void testFromHexString2()
	{
		// int FromHexString(PCWSTR str, int cch, ref<ByteArray> bytes, int offset) { return ConverterFromHexStringA(str, cch, bytes, offset); }
		ref<ByteArray> r = gc_new<ByteArray>(16);
		r->MemoryFill(0x33, 0, r->Count());
		UNITTEST_ASSERT(Converter::FromHexString(__X("cafe88"), 6, r, 1) == 3);
		TestEnv::AssertByteRangeEqual(r, 0, "\x33\xca\xfe\x88\x33\x33", 6);

		// int FromHexString(stringx str, int pos, int cch, ref<ByteArray> bytes, int offset);
		r->MemoryFill(0x33, 0, r->Count());
		UNITTEST_ASSERT(Converter::FromHexString(__X("abeef"), 1, 4, r, 1) == 2);
		TestEnv::AssertByteRangeEqual(r, 0, "\x33\xbe\xef\x33", 4);

		// int FromHexString(stringx str, ref<ByteArray> bytes, int offset) { return FromHexString(str.c_str(), str.Length(), bytes, offset); }
		r->MemoryFill(0x33, 0, r->Count());
		UNITTEST_ASSERT(Converter::FromHexString(__X("3A"), r, 1) == 1);
		TestEnv::AssertByteRangeEqual(r, 0, "\x33\x3A\x33\x33", 4);
	}

	void testFromHexString3()
	{
		// ref<ByteArray> FromHexString(PCWSTR str, UINT cch)
		ref<ByteArray> r = Converter::FromHexString(__X("CA3888"), 4);
		BYTE t1[] = { 0xCA, 0x38 };
		TestEnv::AssertByteArrayEqual(r, t1, sizeof(t1));

		// ref<ByteArray> FromHexString(stringx str, int pos, int cch)
		r = Converter::FromHexString(__X("FACE00"), 1, 2);
		BYTE t2[] = { 0xAC };
		TestEnv::AssertByteArrayEqual(r, t2, sizeof(t2));

		// ref<ByteArray> FromHexString(stringx str)
		r = Converter::FromHexString(__X("FACE00"));
		BYTE t3[] = { 0xFA, 0xCE, 0 };
		TestEnv::AssertByteArrayEqual(r, t3, sizeof(t3));
	}

	void testToHexString()
	{
		// stringx	ToHexString(const BYTE* bytes, int length);
		BYTE rs[] = { 0xBA, 0xBE, 3, 0x30 };
		UNITTEST_ASSERT(Converter::ToHexString(rs + 1, 2) == __X("be03"));

		// stringx	ToHexString(ref<ByteArray> bytes, int offset, int length);
		ref<ByteArray> r = gc_new<ByteArray>(rs, sizeof(rs));
		UNITTEST_ASSERT(Converter::ToHexString(r, 1, 2) == __X("be03"));

		// stringx	ToHexString(ref<ByteArray> bytes);
		UNITTEST_ASSERT(Converter::ToHexString(r) == __X("babe0330"));		
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestConverter);

// -------------------------------------------------------------------------- //
#endif
