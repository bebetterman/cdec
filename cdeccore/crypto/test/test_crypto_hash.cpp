#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestCryptoHash : public UnitTestSuite
{
	UNITTEST_SUITE(TestCryptoHash)
		UNITTEST_METHOD(testMD5)
		UNITTEST_METHOD(testSHA1)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testMD5()
	{
		ref<MD5> md5 = MD5::Create();
		md5->Update((BYTE*)"hello", 5);
		ref<ByteArray> digest = md5->Final();
		UNITTEST_ASSERT(digest->Count() == 16 && digest->at(0) == 0x5d && digest->at(15) == 0x92);

		digest = gc_new<ByteArray>(40);
		md5->Reset();
		md5->Update(digest, 0, 20);
		md5->Update(digest, 20, 20);
		digest = md5->Final();
		UNITTEST_ASSERT(digest->Count() == 16 && digest->at(0) == 0xfd && digest->at(15) == 0x10);

		ref<Stream> istream = gc_new<MemoryStream>("hello", 5);
		digest = md5->Compute(istream);
		UNITTEST_ASSERT(digest->Count() == 16 && digest->at(0) == 0x5d && digest->at(15) == 0x92);
	}

	void testSHA1()
	{
		ref<SHA1> sha1 = SHA1::Create();
		sha1->Update((BYTE*)"hello", 5);
		ref<ByteArray> digest = sha1->Final();
		UNITTEST_ASSERT(digest->Count() == 20 && digest->at(0) == 0xaa && digest->at(19) == 0x4d);

		digest = gc_new<ByteArray>(40);
		sha1->Reset();
		sha1->Update(digest, 0, 20);
		sha1->Update(digest, 20, 20);
		digest = sha1->Final();
		UNITTEST_ASSERT(digest->Count() == 20 && digest->at(0) == 0xb8 && digest->at(19) == 0xdb);
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestCryptoHash);

// -------------------------------------------------------------------------- //
#endif
