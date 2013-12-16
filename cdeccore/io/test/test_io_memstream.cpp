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
		UNITTEST_METHOD(testSetLength)
		UNITTEST_METHOD(testWriteExtend)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testNormalRW()
	{
		int bf = 0x12345678;
		ref<MemoryStream> s = gc_new<MemoryStream>();
		UNITTEST_ASSERT(s->Length() == 0);
		UNITTEST_ASSERT(s->Write(&bf, 4) == 4);
		UNITTEST_ASSERT(s->Length() == 4);
		UNITTEST_ASSERT(s->Position() == 4);
		
		int v = 0;
		UNITTEST_ASSERT(s->Seek(2, Stream::SeekBegin) == 2);
		UNITTEST_ASSERT(s->Read(&v, 4) == 2);
		UNITTEST_ASSERT(v == 0x1234 && s->Position() == 4);
	}

	void testSetLength()
	{
		int bf = 0x12345678;
		ref<MemoryStream> s = gc_new<MemoryStream>();
		s->Write(&bf, 4);

		s->SetLength(6);
		UNITTEST_ASSERT(s->Length() == 6 && s->Position() == 4);
		
		int v;
		s->Seek(2, Stream::SeekBegin);
		UNITTEST_ASSERT(s->Read(&v, 4) == 4);
		UNITTEST_ASSERT(v == 0x1234);

		s->SetLength(2);
		UNITTEST_ASSERT(s->Length() == 2 && s->Position() == 6);
		
		s->Seek(0, Stream::SeekBegin);
		UNITTEST_ASSERT(s->Read(&v, 4) == 2);
		UNITTEST_ASSERT(v == 0x5678);

		ref<ByteArray> data = s->GetBytes();
		UNITTEST_ASSERT(data->Count() == 2);
		UNITTEST_ASSERT(data->at(0) == 0x78 && data->at(1) == 0x56);
		UNITTEST_ASSERT(memcmp(s->GetRawBuffer(), "\x78\x56", 2) == 0);
	}

	void testWriteExtend()
	{
		int bf = 0x12345678;
		ref<MemoryStream> s = gc_new<MemoryStream>();
		UNITTEST_ASSERT(s->Seek(2, Stream::SeekBegin) == 2);
		UNITTEST_ASSERT(s->Write(&bf, 4) == 4);
		UNITTEST_ASSERT(s->Position() == 6);

		int v;
		s->Seek(0, Stream::SeekBegin);
		UNITTEST_ASSERT(s->Read(&v, 4) == 4);
		UNITTEST_ASSERT(v = 0x56780000);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestMemoryStream);

// -------------------------------------------------------------------------- //
#endif
