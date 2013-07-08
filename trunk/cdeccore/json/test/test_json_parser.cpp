#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestJsonParser : public UnitTestSuite
{
	UNITTEST_SUITE(TestJsonParser)
		UNITTEST_METHOD(testStringValue)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStringValue()
	{
		stringx s = __X("\"abc\"");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_String && dom->Root->TextValue == __X("abc"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonParser);

// -------------------------------------------------------------------------- //
#endif
