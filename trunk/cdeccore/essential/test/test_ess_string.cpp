// -------------------------------------------------------------------------- //
//	文件名		：	test_cdec_essential.cpp
//	创建者		：	杨钢
//	创建时间	：	5/9/2008 10:54:01 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

struct StringxSpy: stringx
{
	using stringx::m_pContent;
};

inline StringContent* getStringContent(stringx& s)
{
	return ((StringxSpy&)s).m_pContent;
}

typedef ObjectRefSpy<StringContent>	StringContentSpy;

// -------------------------------------------------------------------------- //

class TestCdecEssString: public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecEssString)
		UNITTEST_METHOD(testStringAssign)
		UNITTEST_METHOD(testStringAppend)
		UNITTEST_METHOD(testStringTrim)
		UNITTEST_METHOD(teststringxSplit)
		UNITTEST_METHOD(testSubstring)
		UNITTEST_METHOD(testStartsWith)
		UNITTEST_METHOD(testToUpper)
		UNITTEST_METHOD(testPadding)
		UNITTEST_METHOD(testEnumString)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStringAssign()
	{
		stringx		a;
		UNITTEST_ASSERT(a.ContentBuffer() == NULL && a.Length() == 0);

		a = __X("");
		UNITTEST_ASSERT(a.Length() == 0 && a.ContentBuffer() != NULL);

		a = NULL;
		UNITTEST_ASSERT(a.Length() == 0 && a.ContentBuffer() == NULL);

		stringx		b = __X("Hello");
		UNITTEST_ASSERT(b.Length() == 5);

		a = b;
		StringContent*	pa = getStringContent(a);
		StringContent*	pb = getStringContent(b);
		UNITTEST_ASSERT(pa == pb);
		UNITTEST_ASSERT(StringContentSpy::GetRef(pa) == 2);

		std::wstring16 msg = __X("Hello");
		a = msg;
		pa = getStringContent(a);
		UNITTEST_ASSERT(pa != pb);
		UNITTEST_ASSERT(a.Length() == b.Length());
		UNITTEST_ASSERT(wstrcmp16(a.ContentBuffer().ptr(), b.ContentBuffer().ptr()) == 0);
		UNITTEST_ASSERT(StringContentSpy::GetRef(pb) == 1);

		stringx		c(a), d(msg);
		UNITTEST_ASSERT(getStringContent(c) != getStringContent(d));
		UNITTEST_ASSERT(c.Length() == d.Length());
		UNITTEST_ASSERT(wstrcmp16(c.ContentBuffer().ptr(), d.ContentBuffer().ptr()) == 0);

		UNITTEST_ASSERT(stringx('0', 6) == __X("000000"));

		stringx		str1 = __X("helloworld");
		stringx		ends1 = __X("world");
		stringx		ends2 = __X("hello");
		stringx		ends3;
		UNITTEST_ASSERT(str1.EndsWith(ends1) == true);
		UNITTEST_ASSERT(str1.EndsWith(ends2) == false);
		UNITTEST_ASSERT(str1.EndsWith(ends3) == true);

		stringx		m1 = __X("abc");
		stringx		m2 = __X("ab");
		stringx		m3 = __X("cba");
		std::wstring16	wstr1 = __X("abc");
		std::wstring16	wstr2 = __X("ab");
		std::wstring16	wstr3 = __X("cba");

		UNITTEST_ASSERT(m1 == wstr1);
		UNITTEST_ASSERT(m1 == __X("abc"));

		UNITTEST_ASSERT(m1 != m2);
		UNITTEST_ASSERT(m1 != wstr2);

		UNITTEST_ASSERT(m1 > m2);
		UNITTEST_ASSERT(m3 > m1);
		UNITTEST_ASSERT(m1 > wstr2);
		UNITTEST_ASSERT(m3 > wstr1);

		UNITTEST_ASSERT(m1 >= m2);
		UNITTEST_ASSERT(m3 >= m1);
		UNITTEST_ASSERT(m1 >= wstr1);
		UNITTEST_ASSERT(m1 >= wstr2);
		UNITTEST_ASSERT(m3 >= wstr1);

		UNITTEST_ASSERT(m2 < m1);
		UNITTEST_ASSERT(m1 < m3);
		UNITTEST_ASSERT(m2 < wstr1);
		UNITTEST_ASSERT(m1 < wstr3);

		UNITTEST_ASSERT(m2 <= m1);
		UNITTEST_ASSERT(m1 <= m3);
		UNITTEST_ASSERT(m1 <= wstr1);
		UNITTEST_ASSERT(m2 <= wstr1);
		UNITTEST_ASSERT(m1 <= wstr3);
	}

	void testStringAppend()
	{
		stringx s = __X("abc");
		UNITTEST_ASSERT((s + (PCWSTR)NULL) == __X("abc"));
		UNITTEST_ASSERT((s + __X("")) == __X("abc"));
		UNITTEST_ASSERT((s + __X("d")) == __X("abcd"));
		UNITTEST_ASSERT((s + __X("def")) == __X("abcdef"));
		UNITTEST_ASSERT((s + stringx()) == __X("abc"));
		UNITTEST_ASSERT((s + (stringx)NULL) == __X("abc"));
		UNITTEST_ASSERT((s + stringx(__X("def"))) == __X("abcdef"));
		UNITTEST_ASSERT((s + 'd') == __X("abcd"));
		UNITTEST_ASSERT((s += __X("def")) == __X("abcdef"));

		s = __X("abc");
		UNITTEST_ASSERT(__X("123") + s == __X("123abc"));
		UNITTEST_ASSERT('X' + s == __X("Xabc"));
	}

	void testStringTrim()
	{
		UNITTEST_ASSERT(stringx().Trim() == NULL);
		UNITTEST_ASSERT(stringx(__X("")).Trim() == __X(""));
		UNITTEST_ASSERT(stringx(__X("\r\n\t \b")).Trim() == __X(""));
		UNITTEST_ASSERT(stringx(__X("  abc")).Trim() == __X("abc"));
		UNITTEST_ASSERT(stringx(__X("  abc  ")).Trim() == __X("abc"));
		UNITTEST_ASSERT(stringx(__X("abc  ")).Trim() == __X("abc"));

		UNITTEST_ASSERT(stringx().TrimLeft() == NULL);
		UNITTEST_ASSERT(stringx(__X("")).TrimLeft() == __X(""));
		UNITTEST_ASSERT(stringx(__X("\r\n\t \b")).TrimLeft() == __X(""));
		UNITTEST_ASSERT(stringx(__X("  abc")).TrimLeft() == __X("abc"));
		UNITTEST_ASSERT(stringx(__X("  abc  ")).TrimLeft() == __X("abc  "));
		UNITTEST_ASSERT(stringx(__X("abc  ")).TrimLeft() == __X("abc  "));

		UNITTEST_ASSERT(stringx().TrimRight() == NULL);
		UNITTEST_ASSERT(stringx(__X("")).TrimRight() == __X(""));
		UNITTEST_ASSERT(stringx(__X("\r\n\t \b")).TrimRight() == __X(""));
		UNITTEST_ASSERT(stringx(__X("  abc")).TrimRight() == __X("  abc"));
		UNITTEST_ASSERT(stringx(__X("  abc  ")).TrimRight() == __X("  abc"));
		UNITTEST_ASSERT(stringx(__X("abc  ")).TrimRight() == __X("abc"));
	}

	void teststringxSplit()
	{
		stringx source = __X("abc\t0\t\tX");
		ref<ArrayListV<stringx> > strs = source.Split('\t');
		UNITTEST_ASSERT(strs->Count() == 4);
		UNITTEST_ASSERT(strs->at(0) == __X("abc") && strs->at(1) == __X("0") &&
			strs->at(2).Length() == 0 && strs->at(3) == __X("X"));

		stringx seperator = __X("@,$");
		stringx testStrx = __X("ABC@DFE,wERG$FDAS,FDA,,,");

		ref<ArrayListV<stringx> > strxTemp = testStrx.Split(seperator);
	}

	void testSubstring()
	{
		stringx a;
		UNITTEST_ASSERT(a.Substring(0) == NULL);
		UNITTEST_ASSERT(a.Substring(0, 0) == NULL);

		a = __X("");
		UNITTEST_ASSERT(a.Substring(0) == __X(""));
		UNITTEST_ASSERT(a.Substring(0, 0) == __X(""));

		a = __X("abc");
		UNITTEST_ASSERT(a.Substring(0) == __X("abc"));
		UNITTEST_ASSERT(a.Substring(2) == __X("c"));
		UNITTEST_ASSERT(a.Substring(3) == __X(""));

		UNITTEST_ASSERT(a.Substring(0, 3) == __X("abc"));
		UNITTEST_ASSERT(a.Substring(1, 1) == __X("b"));
		UNITTEST_ASSERT(a.Substring(3, 0) == __X(""));
	}

	void testStartsWith()
	{
		stringx a = __X("Hello, Cdec");
		UNITTEST_ASSERT(a.StartsWith(a));
		UNITTEST_ASSERT(a.StartsWith(__X("Hel")));
		UNITTEST_ASSERT(a.StartsWith(__X("")));
		UNITTEST_ASSERT(a.StartsWith(NULL));
		UNITTEST_ASSERT(!a.StartsWith(__X("Helo")));

		UNITTEST_ASSERT(a.EndsWith(a));
		UNITTEST_ASSERT(a.EndsWith(__X("dec")));
		UNITTEST_ASSERT(a.EndsWith(__X("")));
		UNITTEST_ASSERT(a.EndsWith(NULL));
		UNITTEST_ASSERT(!a.EndsWith(__X("cdec")));
	}

	void testToUpper()
	{
		stringx a = __X("6-14-12 Akasaka, Minato-ku, Tokyo");
		UNITTEST_ASSERT(a.ToUpper() == __X("6-14-12 AKASAKA, MINATO-KU, TOKYO"));
		UNITTEST_ASSERT(a.ToLower() == __X("6-14-12 akasaka, minato-ku, tokyo"));
	}

	void testPadding()
	{
		stringx a = __X("1234");
		UNITTEST_ASSERT(a.PadLeft('0', 6) == __X("001234"));
		UNITTEST_ASSERT(a.PadLeft('0', 5) == __X("01234"));
		UNITTEST_ASSERT(a.PadLeft('0', 4) == __X("1234"));
		UNITTEST_ASSERT(a.PadLeft('0', 3) == __X("1234"));
		UNITTEST_ASSERT(a.PadRight('#', 6) == __X("1234##"));
		UNITTEST_ASSERT(a.PadRight('#', 5) == __X("1234#"));
		UNITTEST_ASSERT(a.PadRight('#', 4) == __X("1234"));
		UNITTEST_ASSERT(a.PadRight('#', 3) == __X("1234"));

		a = __X("");
		UNITTEST_ASSERT(a.PadLeft('0', 1) == __X("0"));
		UNITTEST_ASSERT(a.PadLeft('0', 0) == __X(""));
		UNITTEST_ASSERT(a.PadRight('#', 1) == __X("#"));
		UNITTEST_ASSERT(a.PadRight('#', 0) == __X(""));

		a = NULL;
		UNITTEST_ASSERT(a.PadLeft('0', 1) == __X("0"));
		UNITTEST_ASSERT(a.PadLeft('0', 0) == NULL);
		UNITTEST_ASSERT(a.PadRight('#', 1) == __X("#"));
		UNITTEST_ASSERT(a.PadRight('#', 0) == NULL);
	}

	void testEnumString()
	{
		stringx s;
		int i = 0;
		foreach (WCHAR, c, s)
			++i;
		UNITTEST_ASSERT(i == 0);

		s = __X("");
		foreach (WCHAR, c, s)
			++i;
		UNITTEST_ASSERT(i == 0);

		PCWSTR const buf = __X("123");
		PCWSTR p = buf;
		s = buf;
		foreach (WCHAR, c, s)
			UNITTEST_ASSERT(c == p[i++]);
		UNITTEST_ASSERT(i == 3);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecEssString);

// -------------------------------------------------------------------------- //
#endif
