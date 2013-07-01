#include "stdafx.h"
#include "../../test_support.h"

#if ENABLE_TEST

class TestWString16: public UnitTestSuite
{
	UNITTEST_SUITE(TestWString16)
		UNITTEST_METHOD(testStrlen)
		UNITTEST_METHOD(testStrcmp)
		UNITTEST_METHOD(testStricmp)
		UNITTEST_METHOD(testStl)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStrlen()
	{
		UNITTEST_ASSERT(wstrlen16(__X("")) == 0);
		UNITTEST_ASSERT(wstrlen16(__X("abc")) == 3);
	}

	void testStrcmp()
	{
		UNITTEST_ASSERT(wstrcmp16(__X(""), __X("")) == 0);
		UNITTEST_ASSERT(wstrcmp16(__X(""), __X("a")) == -1);
		UNITTEST_ASSERT(wstrcmp16(__X("a"), __X("")) == 1);

		UNITTEST_ASSERT(wstrcmp16(__X("abc"), __X("ab")) == 1);
		UNITTEST_ASSERT(wstrcmp16(__X("ab"), __X("abc")) == -1);

		UNITTEST_ASSERT(wstrcmp16(__X("abc"), __X("abc")) == 0);
		UNITTEST_ASSERT(wstrcmp16(__X("aac"), __X("abc")) == -1);
		UNITTEST_ASSERT(wstrcmp16(__X("adc"), __X("abc")) == 1);
	}

	void testStricmp()
	{

		UNITTEST_ASSERT(wstricmp16(__X(""), __X("")) == 0);
		UNITTEST_ASSERT(wstricmp16(__X(""), __X("a")) == -1);
		UNITTEST_ASSERT(wstricmp16(__X("a"), __X("")) == 1);
		
		UNITTEST_ASSERT(wstricmp16(__X("abc"), __X("AB")) == 1);
		UNITTEST_ASSERT(wstricmp16(__X("ab"), __X("ABC")) == -1);

		UNITTEST_ASSERT(wstricmp16(__X("abc"), __X("ABC")) == 0);
		UNITTEST_ASSERT(wstricmp16(__X("aac"), __X("ABC")) == -1);
		UNITTEST_ASSERT(wstricmp16(__X("adc"), __X("ABC")) == 1);
		
		UNITTEST_ASSERT(wstricmp16(__X("a"), __X("A")) == 0);
		UNITTEST_ASSERT(wstricmp16(__X("z"), __X("Z")) == 0);
		UNITTEST_ASSERT(wstricmp16(__X("@"), __X("`")) == -1);
		UNITTEST_ASSERT(wstricmp16(__X("["), __X("{")) == -1);

		UNITTEST_ASSERT(wstricmp16(__X("`"), __X("[")) == 1);
		UNITTEST_ASSERT(wstricmp16(__X("JOHN_HENRY"), __X("JOHNSTON")) == -1);
	}

	void testStl()
	{
		std::wstring16 s = __X("123");
		UNITTEST_ASSERT(s.size() == 3 && s == __X("123"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestWString16)

#endif
