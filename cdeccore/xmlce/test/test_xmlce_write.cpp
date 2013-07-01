#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestXmlceWrite: public UnitTestSuite
{
	UNITTEST_SUITE(TestXmlceWrite)
		UNITTEST_METHOD(testWriteBasic)
		UNITTEST_METHOD(testComposit)
	UNITTEST_SUITE_END()

public:
	void setUp() {}
	void tearDown() {}

	void testWriteBasic()
	{
		ref<StringWriter> wr = gc_new<StringWriter>();
		ref<XmlWriter> xmlw = gc_new<XmlWriter>(wr);

		xmlw->BeginDocument();
		xmlw->BeginElement(__X("document"));
		xmlw->AddAttribute(__X("name"), __X("test"));
		xmlw->EndElement(__X("document"));
		xmlw->EndDocument();

		stringx xmltext = wr->GetStringBuilder()->ToString();
		PCWSTR xmltext_t = __X("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<document name=\"test\" />\n");
		UNITTEST_ASSERT(xmltext == xmltext_t);
	}

	void testComposit()
	{
		ref<StringWriter> wr = gc_new<StringWriter>();
		ref<XmlWriter> xmlw = gc_new<XmlWriter>(wr);

		xmlw->BeginDocument(false, NULL);
		xmlw->BeginElement(__X("library"));
		xmlw->BeginElement(__X("book"));
		xmlw->AddAttribute(__X("name"), __X("test"));
		xmlw->AddAttribute(__X("alias"), __X(""));
		xmlw->AddValue(__X("moon"));
		xmlw->EndElement(__X("book"));
		xmlw->BeginElement(__X("book"));
		xmlw->EndElement(__X("book"));
		xmlw->EndElement(__X("library"));
		xmlw->EndDocument();

		stringx xmltext = wr->GetStringBuilder()->ToString();
		PCWSTR xmltext_t = __X("<library>\n  <book name=\"test\" alias=\"\">moon</book>\n  <book />\n</library>\n");
		UNITTEST_ASSERT(xmltext == xmltext_t);
	}
};

UNITTEST_SUITE_REGISTRATION(TestXmlceWrite);

// -------------------------------------------------------------------------- //
#endif
