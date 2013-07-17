#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestJsonWriter : public UnitTestSuite
{
	UNITTEST_SUITE(TestJsonWriter)
		UNITTEST_METHOD(testWriteValues)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testWriteValues()
	{
		/*
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->WriteString(NULL, __X("abc"));
		UNITTEST_ASSERT(wr->Complete() == __X("\"abc\""));

		wr->Reset();
		wr->WriteInt(null, 123);
		UNITTEST_ASSERT(wr->Complete() == "123");

		wr->Reset();
		wr->WriteString(null, "abc");
		wr->WriteInt(null, 123);
		UNITTEST_ASSERT(wr->Complete() == "\"abc\",123");

		// Multiple values with format
		wr->Reset();
		wr->IndentChars = " ";
		wr->NewLineChars = "\n";
		wr->WriteString(null, "abc");
		wr->WriteInt(null, 123);
		UNITTEST_ASSERT(wr->Complete() == "\"abc\",\n123");
		*/
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonWriter);

// -------------------------------------------------------------------------- //
#endif