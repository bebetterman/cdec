#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestJsonWriter : public UnitTestSuite
{
	UNITTEST_SUITE(TestJsonWriter)

		UNITTEST_METHOD(TestWriteValues)

		UNITTEST_METHOD(TestWriteDictionary)
		UNITTEST_METHOD(TestWriteDictionaryFormat)
		UNITTEST_METHOD(TestWriteList)
		UNITTEST_METHOD(TestWriteListFormat)

		UNITTEST_METHOD(TestWriteDictionaryUnderDictionary)
		UNITTEST_METHOD(TestWriteDictionaryUnderDictionaryFormat)
		UNITTEST_METHOD(TestWriteListUnderDictionary)
		UNITTEST_METHOD(TestWriteListUnderDictionaryFormat)

		UNITTEST_METHOD(TestWriteDictionaryUnderList)
		UNITTEST_METHOD(TestWriteDictionaryUnderListFormat)
		UNITTEST_METHOD(TestWriteListUnderList)
		UNITTEST_METHOD(TestWriteListUnderListFormat)

		UNITTEST_METHOD(TestComplextSample)

	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void TestWriteValues()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->WriteString(NULL, __X("abc"));
		UNITTEST_ASSERT(wr->Complete() == __X("\"abc\""));

		wr->Reset();
		wr->WriteInt(NULL, 123);
		UNITTEST_ASSERT(wr->Complete() == __X("123"));

		wr->Reset();
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		UNITTEST_ASSERT(wr->Complete() == __X("\"abc\",123"));

		// Multiple values with format
		wr->Reset();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		UNITTEST_ASSERT(wr->Complete() == __X("\"abc\",\n123"));
	}

	void TestWriteDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"name\":\"abc\"}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->WriteInt(__X("value"), 123);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"name\":\"abc\",\"value\":123}"));
	}

	void TestWriteDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"name\":\"abc\"\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->WriteString(__X("name"), __X("abc"));
		wr->WriteInt(__X("value"), 123);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"name\":\"abc\",\n \"value\":123\n}"));
	}

	void TestWriteList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\"abc\"]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\"abc\",123]"));
	}

	void TestWriteListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n \"abc\"\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->WriteString(NULL, __X("abc"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n \"abc\",\n 123\n]"));
	}

	void TestWriteDictionaryUnderDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"dict\":{}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"dict\":{\"key\":123}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"dict\":{\"key\":123,\"value\":456}}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"dict\":{\"key\":123},\"value\":456}"));
	}

	void TestWriteDictionaryUnderDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"dict\":{\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"dict\":{\n  \"key\":123\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"dict\":{\n  \"key\":123,\n  \"value\":456\n }\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginDictionary(__X("dict"));
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"dict\":{\n  \"key\":123\n },\n \"value\":456\n}"));
	}

	void TestWriteListUnderDictionary()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"list\":[]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"list\":[123]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"list\":[123,456]}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\"list\":[123],\"value\":456}"));
	}

	void TestWriteListUnderDictionaryFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"list\":[\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"list\":[\n  123\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"list\":[\n  123,\n  456\n ]\n}"));

		wr->Reset();
		wr->BeginDictionary(NULL);
		wr->BeginList(__X("list"));
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		UNITTEST_ASSERT(wr->Complete() == __X("{\n \"list\":[\n  123\n ],\n \"value\":456\n}"));
	}

	void TestWriteDictionaryUnderList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[{}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[{\"key\":123}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[{\"key\":123,\"value\":456}]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[{\"key\":123},456]"));
	}

	void TestWriteDictionaryUnderListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n {\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n {\n  \"key\":123\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->WriteInt(__X("value"), 456);
		wr->EndDictionary();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n {\n  \"key\":123,\n  \"value\":456\n }\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginDictionary(NULL);
		wr->WriteInt(__X("key"), 123);
		wr->EndDictionary();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n {\n  \"key\":123\n },\n 456\n]"));
	}

	void TestWriteListUnderList()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[[]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[[123]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[[123,456]]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[[123],456]"));
	}

	void TestWriteListUnderListFormat()
	{
		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X(" ");
		wr->NewLineChars = __X("\n");
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n [\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n [\n  123\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->WriteInt(NULL, 456);
		wr->EndList();
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n [\n  123,\n  456\n ]\n]"));

		wr->Reset();
		wr->BeginList(NULL);
		wr->BeginList(NULL);
		wr->WriteInt(NULL, 123);
		wr->EndList();
		wr->WriteInt(NULL, 456);
		wr->EndList();
		UNITTEST_ASSERT(wr->Complete() == __X("[\n [\n  123\n ],\n 456\n]"));
	}

	void TestComplextSample()
	{
		stringx s = stringx() +
			__X("{\n") +
			__X("  \"return\":{\n") +
			__X("    \"u\":[\n") +
			__X("      {\n") +
			__X("        \"u\":\"http://118.144.67.133/ufa/\",\n") +
			__X("        \"t\":2\n") +
			__X("      },\n") +
			__X("      {\n") +
			__X("        \"u\":\"http://61.189.5.13/ufa/\",\n") +
			__X("        \"t\":1\n") +
			__X("      }\n") +
			__X("    ],\n") +
			__X("    \"p\":\"2\"\n") +
			__X("  },\n") +
			__X("  \"code\":\"Ok\"\n") +
			__X("}");

		ref<JsonWriter> wr = gc_new<JsonWriter>();
		wr->IndentChars = __X("  ");
		wr->NewLineChars = __X("\n");

		wr->BeginDictionary(NULL);
			wr->BeginDictionary(__X("return"));
			wr->BeginList(__X("u"));
				wr->BeginDictionary(NULL);
				wr->WriteString(__X("u"), __X("http://118.144.67.133/ufa/"));
				wr->WriteInt(__X("t"), 2);
				wr->EndDictionary();
				wr->BeginDictionary(NULL);
				wr->WriteString(__X("u"), __X("http://61.189.5.13/ufa/"));
				wr->WriteInt(__X("t"), 1);
				wr->EndDictionary();
			wr->EndList();
			wr->WriteString(__X("p"), __X("2"));
			wr->EndDictionary();			  
			wr->WriteString(__X("code"), __X("Ok"));
		wr->EndDictionary();

		stringx t = wr->Complete();
		UNITTEST_ASSERT(s == t);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestJsonWriter);

// -------------------------------------------------------------------------- //
#endif
