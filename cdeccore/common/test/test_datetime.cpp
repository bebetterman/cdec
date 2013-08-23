#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestDateTime: public UnitTestSuite
{
	UNITTEST_SUITE(TestDateTime)
		UNITTEST_METHOD(testFormat)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testFormat()
	{
		DateTime dt = DateTime::Set(2005, 5, 3, 2, 5, 2, 50);
		UNITTEST_ASSERT(dt.Format() == __X("2005-05-03 02:05:02"));

		dt = DateTime::Set(2005, 12, 23, 22, 15, 12, 50);
		UNITTEST_ASSERT(dt.Format() == __X("2005-12-23 22:15:12"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestDateTime);

// -------------------------------------------------------------------------- //
#endif
