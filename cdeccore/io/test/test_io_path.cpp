#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestCdecPath : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecPath)
		UNITTEST_METHOD(testGetFileName)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testGetFileName()
	{
		UNITTEST_ASSERT(Path::GetFileName(__X("D:\\abc\\def")) == __X("def"));
		UNITTEST_ASSERT(Path::GetFileName(__X("/abc/def")) == __X("def"));
		UNITTEST_ASSERT(Path::GetFileName(__X("/def")) == __X("def"));
		UNITTEST_ASSERT(Path::GetFileName(__X("def")) == __X("def"));
		UNITTEST_ASSERT(Path::GetFileName(__X("")) == __X(""));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecPath);

// -------------------------------------------------------------------------- //
#endif
