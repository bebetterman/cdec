#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestConverterA: public UnitTestSuite
{
	UNITTEST_SUITE(TestConverterA)
		UNITTEST_METHOD(testStrToInt32)
		UNITTEST_METHOD(testStrToInt64)
		UNITTEST_METHOD(testInt32ToStr)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStrToInt32()
	{
		UNITTEST_ASSERT(ConverterA::ToInt32("0") == 0);
		UNITTEST_ASSERT(ConverterA::ToInt32("2147483647") == 2147483647);
		//UNITTEST_ASSERT(ConverterA::ToInt32("2147483648") == -2147483648);	// overflow
		UNITTEST_ASSERT(ConverterA::ToInt32("-2147483647") == -2147483647);
		//UNITTEST_ASSERT(ConverterA::ToInt32("-2147483648") == -2147483648);	// overflow
		UNITTEST_ASSERT(ConverterA::ToInt32("-1") == -1);
		
		UNITTEST_ASSERT(ConverterA::ToInt32("1001011000111110", 2) == 0x963E);
		UNITTEST_ASSERT(ConverterA::ToInt32("330face", 16) == 0x330FACE);

		UNITTEST_ASSERT(ConverterA::ToInt32("+1") == 1);
		UNITTEST_ASSERT(ConverterA::ToInt32("-1") == -1);

		UNITTEST_ASSERT(ConverterA::ToUInt32("4294967295") == 0xFFFFFFFF);
	}

	void testStrToInt64()
	{
		UNITTEST_ASSERT(ConverterA::ToInt64("0") == 0);
		UNITTEST_ASSERT(ConverterA::ToInt64("9223372036854775807") == 0x7FFFFFFFFFFFFFFFL);
		UNITTEST_ASSERT(ConverterA::ToUInt64("9223372036854775808") == 0x8000000000000000UL);
		UNITTEST_ASSERT(ConverterA::ToUInt64("18446744073709551615") == 0xFFFFFFFFFFFFFFFFUL);

		UNITTEST_ASSERT(ConverterA::ToInt64("7FFFFFFFFFFFFFFF", 16) == 0x7FFFFFFFFFFFFFFFL);
		UNITTEST_ASSERT(ConverterA::ToUInt64("8000000000000000", 16) == 0x8000000000000000UL);
		UNITTEST_ASSERT(ConverterA::ToUInt64("FFFFFFFFffffffff", 16) == 0xFFFFFFFFFFFFFFFFUL);
	}

	void testInt32ToStr()
	{
		UNITTEST_ASSERT(ConverterA::ToString((int)0) == "0");
		UNITTEST_ASSERT(ConverterA::ToString((int)9) == "9");
		UNITTEST_ASSERT(ConverterA::ToString((int)681) == "681");

		UNITTEST_ASSERT(ConverterA::ToString((int)-1) == "-1");

		UNITTEST_ASSERT(ConverterA::ToString((UINT)0) == "0");
		UNITTEST_ASSERT(ConverterA::ToString((UINT)9) == "9");
		UNITTEST_ASSERT(ConverterA::ToString((UINT)681) == "681");
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestConverterA);

// -------------------------------------------------------------------------- //
#endif
