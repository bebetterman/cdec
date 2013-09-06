#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestBase64: public UnitTestSuite
{
	UNITTEST_SUITE(TestBase64)
		UNITTEST_METHOD(testNormal)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testNormal()
	{
		ref<Encoding> e = Encoding::get_UTF8();
		stringx org = __X("greate");
		ref<ByteArray> orga = e->GetBytes(org);
		stringx enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("Z3JlYXRl"));

		org = __X("baby");
		orga = e->GetBytes(org);
		enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("YmFieQ=="));

		org = __X("mouse");
		orga = e->GetBytes(org);
		enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("bW91c2U="));
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestBase64);

// -------------------------------------------------------------------------- //
#endif
