#include "stdafx.h"
#include "../../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestCryptoAes: public UnitTestSuite
{
	UNITTEST_SUITE(TestCryptoAes)
		UNITTEST_METHOD(testAes128)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testAes128()
	{
		BYTE _key[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
		ref<ByteArray> key = gc_new<ByteArray>(_key, 16);

		ref<AES> aes = gc_new<AES>();
		aes->SetKey(key);
		ref<ByteArray> src = gc_new<ByteArray>(32);
		ref<ByteArray> dest = gc_new<ByteArray>(16);
		UNITTEST_ASSERT(aes->Encode(src, 0, dest, 0, 16) == 16);
		UNITTEST_ASSERT(Converter::ToHexString(dest) == __X("c6a13b37878f5b826f4f8162a1c8d879"));

		for (int i = 16; i < 32; ++i) src->at(i) = i;	// Fill with incorrect data
		UNITTEST_ASSERT(aes->Decode(dest, 0, src, 16, 16) == 16);
		UNITTEST_ASSERT(Converter::ToHexString(src, 16, 16) == __X("00000000000000000000000000000000"));
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestCryptoAes);

// -------------------------------------------------------------------------- //
#endif
