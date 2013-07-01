#include "stdafx.h"
#include "../../test_support.h"

#if ENABLE_TEST

class TestStdTypes: public UnitTestSuite
{
	UNITTEST_SUITE(TestStdTypes)
		UNITTEST_METHOD(testInteger)
		UNITTEST_METHOD(testChar)
		UNITTEST_METHOD(testFloat)
		UNITTEST_METHOD(testOther)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testInteger()
	{
		UNITTEST_ASSERT(sizeof(int) == 4 && sizeof(UINT) == 4);

		UNITTEST_ASSERT(sizeof(INT8) == 1 && sizeof(UINT8) == 1);
		UNITTEST_ASSERT(sizeof(INT16) == 2 && sizeof(UINT16) == 2);
		UNITTEST_ASSERT(sizeof(INT32) == 4 && sizeof(UINT32) == 4);
		UNITTEST_ASSERT(sizeof(INT64) == 8 && sizeof(UINT64) == 8);

		UNITTEST_ASSERT(sizeof(BYTE) == 1);
		UNITTEST_ASSERT(sizeof(WORD) == 2);
		UNITTEST_ASSERT(sizeof(DWORD) == 4);

#ifdef X_CPU_X86
		UNITTEST_ASSERT(sizeof(long) == 4);
#else
#	ifdef X_CC_VC
		UNITTEST_ASSERT(sizeof(long) == 4);
#	else
		UNITTEST_ASSERT(sizeof(long) == 8);
#	endif
#endif
	}

	void testChar()
	{
		UNITTEST_ASSERT(sizeof(char) == 1);
		UNITTEST_ASSERT(sizeof(WCHAR) == 2);
#ifdef X_CC_VC
		UNITTEST_ASSERT(sizeof(wchar_t) == 2);
#else
		UNITTEST_ASSERT(sizeof(wchar_t) == 4);
#endif
	}

	void testFloat()
	{
		UNITTEST_ASSERT(sizeof(float) == 4);
		UNITTEST_ASSERT(sizeof(double) == 8);
		
#ifdef X_CPU_X86
		UNITTEST_ASSERT(sizeof(long double) == 8);
#else
#       ifdef X_CC_VC
                UNITTEST_ASSERT(sizeof(long double) == 8);
#       else
                UNITTEST_ASSERT(sizeof(long double) == 16);
#       endif
#endif
	}

	void testOther()
	{
		UNITTEST_ASSERT(sizeof(bool) == 1);

#ifdef X_CPU_X86
		UNITTEST_ASSERT(sizeof(void*) == 4);
		UNITTEST_ASSERT(sizeof(int*) == 4);
		UNITTEST_ASSERT(sizeof(INT_PTR) == 4);
		UNITTEST_ASSERT(sizeof(UINT_PTR) == 4);
#else
		UNITTEST_ASSERT(sizeof(void*) == 8);
		UNITTEST_ASSERT(sizeof(int*) == 8);
		UNITTEST_ASSERT(sizeof(INT_PTR) == 8);
		UNITTEST_ASSERT(sizeof(UINT_PTR) == 8);
#endif
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestStdTypes)

#endif
