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
		UNITTEST_METHOD(testTimestamp)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testFormat()
	{
		DateTime dt;
		dt.Set(2005, 5, 3, 2, 5, 2, 50);
		UNITTEST_ASSERT(dt.Format() == __X("2005-05-03 02:05:02"));

		dt.Set(2005, 12, 23, 22, 15, 12, 50);
		UNITTEST_ASSERT(dt.Format() == __X("2005-12-23 22:15:12"));
	}

	void testTimestamp()
	{
		// Local 2013-10-23 17:18:58
		// UTC 2013-10-23 09:18:58
		// Timestamp 1382519938

		DateTime dt;
		dt.SetUtc(1382519938, 0);
		UNITTEST_ASSERT(dt.Format() == __X("2013-10-23 09:18:58"));
		UNITTEST_ASSERT(dt.ToTimestamp() == 1382519938);
		
		dt.SetLocal(1382519938, 0);
		UNITTEST_ASSERT(dt.Format() == __X("2013-10-23 17:18:58"));
		UNITTEST_ASSERT(dt.ToTimestamp() == 1382519938 + 8 * 3600);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestDateTime);

// -------------------------------------------------------------------------- //
#endif
