#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestMemoryStream: public UnitTestSuite
{
	UNITTEST_SUITE(TestMemoryStream)
		UNITTEST_METHOD(testNormalRW)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testNormalRW()
	{
		INT bf = 0x12345678;
		ref<MemoryStream> s = gc_new<MemoryStream>();
		UNITTEST_ASSERT(s->Length() == 0);
		UNITTEST_ASSERT(s->Write(&bf, 4) == 4);
		UNITTEST_ASSERT(s->Length() == 4);
		UNITTEST_ASSERT(s->Position() == 4);
		
		INT v = 0;
		UNITTEST_ASSERT(s->Seek(2, Stream::SeekBegin) == 2);
		UNITTEST_ASSERT(s->Read(&v, 4) == 2);
		UNITTEST_ASSERT(v == 0x1234 && s->Position() == 4);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestMemoryStream);

// -------------------------------------------------------------------------- //
#endif
