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
		UNITTEST_METHOD(TestFloatValue)
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
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_String && root->TextValue() == __X("abc"));

		s = __X(" \" abc \" ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_String && root->TextValue() == __X(" abc "));

		s = __X("\"\"");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_String && root->TextValue() == __X(""));
	}

	void TestIntValue()
	{
		stringx s = __X("123456789012345");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Integer && root->IntValue() == 123456789012345);

		s = __X(" 123 ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Integer && root->IntValue() == 123);

		s = __X("-1");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Integer && root->IntValue() == -1);
	}

	void TestFloatValue()
	{
		stringx s = __X("3.14159");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Float && Math::Equal(root->FloatValue(), 3.14159));

		s = __X("0.0");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Float && Math::Equal(root->FloatValue(), 0.0));

		s = __X("-0.003");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Float && Math::Equal(root->FloatValue(), -0.003));
	}

	void TestBooleanValue()
	{
		stringx s = __X("true");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Boolean && root->BoolValue() == 1);

		s = __X(" TrUe ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Boolean && root->BoolValue() == 1);

		s = __X(" fAlSe ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Boolean && root->BoolValue() == 0);
	}

	void TestNoneValue()
	{
		stringx s = __X("null");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() ==JSN_None);

		s = __X(" NuLl ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_None);
	}

	void TestSimpleDictionary()
	{
		stringx s = __X("{\"a\":\"b\"}");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Dictionary && root->NodeDictionary()->Count() == 1);
		ref<JsonNode> node = root->NodeDictionary()->Get(__X("a"));
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->GetName() == __X("a") && node->TextValue() == __X("b"));

		s = __X(" { \"a\" : \"1\" , \"b\":\"\" } ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Dictionary && root->NodeDictionary()->Count() == 2);
		node = root->NodeDictionary()->Get(__X("a"));
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->TextValue() == __X("1"));
		node = root->NodeDictionary()->Get(__X("b"));
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->TextValue() == __X(""));

		s = __X("{}");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Dictionary && root->NodeDictionary()->Count() == 0);
	}

	void TestSimpleList()
	{
		stringx s = __X("[\"a\"]");
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_NodeList && root->NodeList()->Count() == 1);
		ref<JsonNode> node = root->NodeList()->at(0);
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->GetName() == NULL && node->TextValue() == __X("a"));

		s = __X(" [ \"a\" , \"\" ] ");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_NodeList && root->NodeList()->Count() == 2);
		node = root->NodeList()->at(0);
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->TextValue() == __X("a"));
		node = root->NodeList()->at(1);
		UNITTEST_ASSERT(node->GetType() == JSN_String && node->TextValue() == __X(""));

		s = __X("[]");
		root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_NodeList && root->NodeList()->Count() == 0);
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
		ref<JsonNode> root = JsonParser::ParseText(s);
		UNITTEST_ASSERT(root->GetType() == JSN_Dictionary && root->NodeDictionary()->Count() == 2);

		ref<JsonNode> node1 = root->NodeDictionary()->Get(__X("return"));	// \[return]
		UNITTEST_ASSERT(node1->GetType() == JSN_Dictionary && node1->NodeDictionary()->Count() == 2);

		ref<JsonNode> node2 = node1->NodeDictionary()->Get(__X("u"));	// \[return]\[u]
		UNITTEST_ASSERT(node2->GetType() == JSN_NodeList && node2->NodeList()->Count() == 2);

		ref<JsonNode> node3 = node2->NodeList()->at(0);	// \[return]\[u]\[0]
		UNITTEST_ASSERT(node3->GetType() == JSN_Dictionary && node3->NodeDictionary()->Count() == 2);
		UNITTEST_ASSERT(node3->NodeDictionary()->Get(__X("u"))->TextValue() == __X("http://118.144.67.133/ufa/") && node3->NodeDictionary()->Get(__X("t"))->TextValue() == __X("2"));

		node3 = node2->NodeList()->at(1); 	// \[return]\[u]\[1]
		UNITTEST_ASSERT(node3->NodeDictionary()->Get(__X("u"))->TextValue() == __X("http://61.189.5.13/ufa/") && node3->NodeDictionary()->Get(__X("t"))->TextValue() == __X("1"));

		node2 = node1->NodeDictionary()->Get(__X("b"));	// \[return]\[b]
		UNITTEST_ASSERT(node2->GetType() == JSN_NodeList && node2->NodeList()->Count() == 1);

		node3 = node2->NodeList()->at(0);	// \[return]\[b]\[0]
		UNITTEST_ASSERT(node3->GetType() == JSN_Dictionary && node3->NodeDictionary()->Count() == 3);
		UNITTEST_ASSERT(node3->NodeDictionary()->Get(__X("id"))->TextValue() == __X("0") && node3->NodeDictionary()->Get(__X("index"))->TextValue() == __X("1"));

		ref<JsonNode> node4 = node3->NodeDictionary()->Get(__X("u"));	// \[return]\[b]\[0]\[u]
		UNITTEST_ASSERT(node4->GetType() == JSN_NodeList && node4->NodeList()->Count() == 2);
		UNITTEST_ASSERT(node4->NodeList()->at(0)->TextValue() == __X("0") && node4->NodeList()->at(1)->TextValue() == __X("1"));

		node1 = root->NodeDictionary()->Get(__X("code"));	// \[code]
		UNITTEST_ASSERT(node1->GetType() == JSN_String && node1->TextValue() == __X("Ok"));
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonParser);

// -------------------------------------------------------------------------- //
#endif
