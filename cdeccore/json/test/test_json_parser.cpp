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
		UNITTEST_METHOD(TestStringValue)
		UNITTEST_METHOD(TestIntValue)
		UNITTEST_METHOD(TestBooleanValue)
		UNITTEST_METHOD(TestNoneValue)
		UNITTEST_METHOD(TestSimpleDictionary)
		UNITTEST_METHOD(TestSimpleList)
		UNITTEST_METHOD(TestComplextSample)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void TestStringValue()
	{
		stringx s = __X("\"abc\"");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_String && dom->Root->TextValue == __X("abc"));

		s = __X(" \" abc \" ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_String && dom->Root->TextValue == __X(" abc "));

		s = __X("\"\"");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_String && dom->Root->TextValue == __X(""));
	}

	void TestIntValue()
	{
		stringx s = __X("123");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Integer && dom->Root->IntValue == 123);

		s = __X(" 123 ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Integer && dom->Root->IntValue == 123);

	}

	void TestBooleanValue()
	{
		ref<JsonDom> dom = gc_new<JsonDom>();
		stringx s = __X("true");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Boolean && dom->Root->IntValue == 1);

		s = __X(" TrUe ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Boolean && dom->Root->IntValue == 1);

		s = __X(" fAlSe ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Boolean && dom->Root->IntValue == 0);
	}

	void TestNoneValue()
	{
		ref<JsonDom> dom = gc_new<JsonDom>();
		stringx s = __X("null");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType ==JSN_None);

		s = __X(" NuLl ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_None);
	}

	void TestSimpleDictionary()
	{
		stringx s = __X("{\"a\":\"b\"}");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Dictionary && dom->Root->NodeDict->Count() == 1);
		ref<JsonNode> node = dom->Root->NodeDict->Get(__X("a"));
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->Name == __X("a") && node->TextValue == __X("b"));

		s = __X(" { \"a\" : \"1\" , \"b\":\"\" } ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Dictionary && dom->Root->NodeDict->Count() == 2);
		node = dom->Root->NodeDict->Get(__X("a"));
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->TextValue == __X("1"));
		node = dom->Root->NodeDict->Get(__X("b"));
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->TextValue == __X(""));

		s = __X("{}");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Dictionary && dom->Root->NodeDict->Count() == 0);
	}

	void TestSimpleList()
	{
		stringx s = __X("[\"a\"]");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_NodeList && dom->Root->NodeList->Count() == 1);
		ref<JsonNode> node = dom->Root->NodeList->at(0);
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->Name == NULL && node->TextValue == __X("a"));

		s = __X(" [ \"a\" , \"\" ] ");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_NodeList && dom->Root->NodeList->Count() == 2);
		node = dom->Root->NodeList->at(0);
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->TextValue == __X("a"));
		node = dom->Root->NodeList->at(1);
		UNITTEST_ASSERT(node->NodeType == JSN_String && node->TextValue == __X(""));

		s = __X("[]");
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_NodeList && dom->Root->NodeList->Count() == 0);
	}

	void TestComplextSample()
	{
		stringx s =
			stringx(__X("{")) +
			__X("  \"return\":{") +
			__X("    \"u\":[") +
			__X("      {\"u\":\"http://118.144.67.133/ufa/\",\"t\":\"2\"},") +
			__X("      {\"u\":\"http://61.189.5.13/ufa/\",\"t\":\"1\"}") +
			__X("    ],") +
			__X("    \"b\":[") +
			__X("      {") +
			__X("        \"id\":\"0\",") +
			__X("        \"u\":[\"0\",\"1\"],") +
			__X("        \"index\":\"1\"") +
			__X("      }") +
			__X("    ]") +
			__X("  },") +
			__X("  \"code\":\"Ok\"") +
			__X("}");
		ref<JsonDom> dom = gc_new<JsonDom>();
		dom->Load(s);
		UNITTEST_ASSERT(dom->Root->NodeType == JSN_Dictionary && dom->Root->NodeDict->Count() == 2);

		ref<JsonNode> node1 = dom->Root->NodeDict->Get(__X("return"));	// \[return]
		UNITTEST_ASSERT(node1->NodeType == JSN_Dictionary && node1->NodeDict->Count() == 2);

		ref<JsonNode> node2 = node1->NodeDict->Get(__X("u"));	// \[return]\[u]
		UNITTEST_ASSERT(node2->NodeType == JSN_NodeList && node2->NodeList->Count() == 2);

		ref<JsonNode> node3 = node2->NodeList->at(0);	// \[return]\[u]\[0]
		UNITTEST_ASSERT(node3->NodeType == JSN_Dictionary && node3->NodeDict->Count() == 2);
		UNITTEST_ASSERT(node3->NodeDict->Get(__X("u"))->TextValue == __X("http://118.144.67.133/ufa/") && node3->NodeDict->Get(__X("t"))->TextValue == __X("2"));

		node3 = node2->NodeList->at(1); 	// \[return]\[u]\[1]
		UNITTEST_ASSERT(node3->NodeDict->Get(__X("u"))->TextValue == __X("http://61.189.5.13/ufa/") && node3->NodeDict->Get(__X("t"))->TextValue == __X("1"));

		node2 = node1->NodeDict->Get(__X("b"));	// \[return]\[b]
		UNITTEST_ASSERT(node2->NodeType == JSN_NodeList && node2->NodeList->Count() == 1);

		node3 = node2->NodeList->at(0);	// \[return]\[b]\[0]
		UNITTEST_ASSERT(node3->NodeType == JSN_Dictionary && node3->NodeDict->Count() == 3);
		UNITTEST_ASSERT(node3->NodeDict->Get(__X("id"))->TextValue == __X("0") && node3->NodeDict->Get(__X("index"))->TextValue == __X("1"));

		ref<JsonNode> node4 = node3->NodeDict->Get(__X("u"));	// \[return]\[b]\[0]\[u]
		UNITTEST_ASSERT(node4->NodeType == JSN_NodeList && node4->NodeList->Count() == 2);
		UNITTEST_ASSERT(node4->NodeList->at(0)->TextValue == __X("0") && node4->NodeList->at(1)->TextValue == __X("1"));

		node1 = dom->Root->NodeDict->Get(__X("code"));	// \[code]
		UNITTEST_ASSERT(node1->NodeType == JSN_String && node1->TextValue == __X("Ok"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonParser);

// -------------------------------------------------------------------------- //
#endif
