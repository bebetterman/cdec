#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

bool StringEqual(std::string a, std::string b)
{
	return a.size() == b.size() && strcmp(a.c_str(), b.c_str()) == 0;
}

class TestEncoding: public UnitTestSuite
{
	UNITTEST_SUITE(TestEncoding)
		UNITTEST_METHOD(testEncoding)
		UNITTEST_METHOD(testEncodingChars)
		UNITTEST_METHOD(testEncodingIANA)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testEncoding()
	{
#ifdef X_OS_WINDOWS
		UNITTEST_ASSERT( Encoding::get_Default()->get_CP() == 936);
#else
                UNITTEST_ASSERT( Encoding::get_Default()->get_CP() == 65001);
#endif

		ref<Encoding>	ecDefault = gc_new<Encoding>(936);
		ref<Encoding>	ecUnicode = Encoding::get_Unicode();
		ref<Encoding>	ecUtf8	  = Encoding::get_UTF8();
		UNITTEST_ASSERT(ecDefault->get_CP() == 936);
		UNITTEST_ASSERT(ecUnicode->get_CP() == 1200);
		UNITTEST_ASSERT(ecUtf8->get_CP() == 65001);

		std::string		sst	= "Hello, world.";
		stringx			swt = __X("Hello, world.");
		std::string		ss	= ecDefault->FromUnicode(swt.ToStdString());
		stringx			sw	= ecDefault->ToUnicode(sst);
		UNITTEST_ASSERT(StringEqual(ss, sst));
		UNITTEST_ASSERT(sw == swt);

		ss = ecUtf8->FromUnicode(swt.ToStdString());
		sw = ecUtf8->ToUnicode(ss);
		UNITTEST_ASSERT(sw.Length() == swt.Length() && sw == swt);

		char chs_a[] = { (char)0xC4, (char)0xE3, (char)0xBA, (char)0xC3, 0 };	// GBK ÄãºÃ
		WCHAR chs_w[] = { 0x4F60, 0x597D, 0 };	// Unicode ÄãºÃ
		sst = chs_a;
		swt = chs_w;

		ss	= ecDefault->FromUnicode(swt.ToStdString());
		sw	= ecDefault->ToUnicode(sst);
		UNITTEST_ASSERT(StringEqual(ss, sst));
		UNITTEST_ASSERT(sw.Length() == swt.Length() && sw == swt);

		ss = ecUtf8->FromUnicode(swt.ToStdString());
		sw = ecUtf8->ToUnicode(ss);
		UNITTEST_ASSERT(sw.Length() == swt.Length() && sw == swt);
	}

	void testEncodingChars()
	{
		// SBCS
		ref<Encoding> encoding = gc_new<Encoding>(1252);	// Western European (Windows) / Latin-1
		UNITTEST_ASSERT(encoding->get_Class() == Encoding::SBCS);
		const char cs1[] = { 'Z', (char)0xD6 };
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs1, 1) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs1 + 1, 1) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs1, 2) == Encoding::CharError);

		// MBCS
		encoding->ChangeCP(936);		// Chinese Simplified (GB2312) / GBK
		UNITTEST_ASSERT(encoding->get_Class() == Encoding::DBCS);
		const char* cs2 = "ZÖÐ.";
		UNITTEST_ASSERT(!encoding->IsDBCSLeading(cs2[0]));
		UNITTEST_ASSERT(encoding->IsDBCSLeading(cs2[1]));
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs2, 1) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs2, 2) == Encoding::CharError);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs2 + 1, 1) == Encoding::CharLeading);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs2 + 1, 2) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs2 + 1, 3) == Encoding::CharError);

		// UTF-8
		encoding->ChangeCP(Encoding::UTF8);
		UNITTEST_ASSERT(encoding->get_Class() == Encoding::UTF_MBCS);
		const char cs3[] = "Z\xC3\x96\xE4\xB8\xAD.";
		UNITTEST_ASSERT(encoding->GetUtf8TrailingCount(cs3[0]) == 0);
		UNITTEST_ASSERT(encoding->GetUtf8TrailingCount(cs3[1]) == 1);
		UNITTEST_ASSERT(encoding->GetUtf8TrailingCount(cs3[3]) == 2);
		UNITTEST_ASSERT(!encoding->IsUtf8TrailingChar(cs3[0]));
		UNITTEST_ASSERT(!encoding->IsUtf8TrailingChar(cs3[1]) && encoding->IsUtf8TrailingChar(cs3[2]));
		UNITTEST_ASSERT(!encoding->IsUtf8TrailingChar(cs3[3]) && encoding->IsUtf8TrailingChar(cs3[4]));
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3, 1) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3, 2) == Encoding::CharError);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 1, 1) == Encoding::CharLeading);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 1, 2) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 1, 3) == Encoding::CharError);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 3, 2) == Encoding::CharLeading);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 3, 3) == Encoding::CharCompleted);
		UNITTEST_ASSERT(encoding->ValidateMultiByteChars(cs3 + 3, 4) == Encoding::CharError);

		UNITTEST_ASSERT(encoding->DecodeUtf8Char(cs3, 1) == __X('Z'));
		UNITTEST_ASSERT(encoding->DecodeUtf8Char(cs3 + 1, 2) == 0xD6);
		UNITTEST_ASSERT(encoding->DecodeUtf8Char(cs3 + 3, 3) == 0x4E2D);
	}

	void testEncodingIANA()
	{
		ref<Encoding> encoding;
		encoding = gc_new<Encoding>(936);
		UNITTEST_ASSERT(encoding->get_WebName() == __X("gb2312"));
		encoding = gc_new<Encoding>(1252);
		UNITTEST_ASSERT(encoding->get_WebName() == __X("Windows-1252"));
		encoding = Encoding::get_Unicode();
		UNITTEST_ASSERT(encoding->get_WebName() == __X("utf-16"));
		encoding = Encoding::get_UTF8();
		UNITTEST_ASSERT(encoding->get_WebName() == __X("utf-8"));

		encoding = Encoding::FromWebName(__X("gb2312"));
		UNITTEST_ASSERT(encoding->get_CP() == 936);
		encoding = Encoding::FromWebName(__X("windows-1252"));
		UNITTEST_ASSERT(encoding->get_CP() == 1252);
		encoding = Encoding::FromWebName(__X("utf-8"));
		UNITTEST_ASSERT(encoding->get_CP() == Encoding::UTF8);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestEncoding);

// -------------------------------------------------------------------------- //
#endif
