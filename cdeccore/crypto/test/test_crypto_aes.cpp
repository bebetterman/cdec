#include "stdafx.h"
#include "../../test_support.h"

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
		UNITTEST_METHOD(testAesEcb128)
		UNITTEST_METHOD(testAesCbc128)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testAesEcb128()
	{
		BYTE _key[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
		BYTE _src[32] = {
			0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		ref<AES> aes = gc_new<AES>();
		ref<ByteArray> key = gc_new<ByteArray>(_key, 16);
		aes->SetKey(key);

		ref<ByteArray> src = gc_new<ByteArray>(_src, 32);
		ref<ByteArray> dest = gc_new<ByteArray>(16);

		ref<ICryptoTransform> e = aes->CreateEncryptor();
		e->Transform(src, 0, dest, 0, 16);
		UNITTEST_ASSERT(Converter::ToHexString(dest) == __X("ac26591c0f8bd80ee7c7e3a2d14e2b22"));

		ref<ICryptoTransform> d = aes->CreateDecryptor();
		d->Transform(dest, 0, src, 16, 16);
		UNITTEST_ASSERT(Converter::ToHexString(src, 16, 16) == __X("808182838485868788898a8b8c8d8e8f"));
	}

	void testAesCbc128()
	{
		PCWSTR _key = __X("000102030405060708090a0b0c0d0e0f");
		PCWSTR _iv = __X("101112131415161718191a1b1c1d1e1f");
		PCWSTR _ssc[] = {
			__X("954f64f2e4e86e9eee82d20216684899a93b9ddb22e8ab104c61e728831d6d5a"),
			__X("1f51afc19343249ef3b4495be12dc4f3e0b296f5c91be273a66c1216a66da5fd"),
			__X("24dbfbf2953d5862f57436198ada0a437e92bdee860d80b1c4014a69edecddc6"),
			__X("f5251d19ef0ccd3192c0516cd6d54744971c1b6a1ac2fbef3d3275522402fa92"),
			__X("c6a46856f35678eb9422d2fbad27f763b7d9b91567cecb9702177e22e0ed5e98"),
			__X("c33d82965259ab55ee513a1d62f535f9bb168053f233e9cdda450acac8780268"),
			__X("25a3aaa6cf1106801b9b39e71f97b3158e2d3c30da82ba212f577600bf6ca857"),
			__X("43cb511f061f33fe87a86476ec74d74297b4262a64dde1c95003f3d8c3d7aff2")
		};

		ref<AES> aes = gc_new<AES>();
		aes->SetMode(Cipher_CBC);
		ref<ByteArray> key = Converter::FromHexString(_key, 32);
		aes->SetKey(key);
		ref<ByteArray> iv = Converter::FromHexString(_iv, 32);
		aes->SetIV(iv);

		ref<ByteArray> src = gc_new<ByteArray>(256);
		for (int i = 0; i < 256; ++i)
			src->at(i) = (BYTE)i;

		ref<ByteArray> dest = gc_new<ByteArray>(256);
		ref<ICryptoTransform> e = aes->CreateEncryptor();
		e->Transform(src, 0, dest, 0, 256);
		
		for (int i = 0; i < 256 / 32; ++i)
			UNITTEST_ASSERT(Converter::ToHexString(dest, i * 32, 32) == _ssc[i]);

		ref<ByteArray> rev = gc_new<ByteArray>(256);
		ref<ICryptoTransform> d = aes->CreateDecryptor();
		d->Transform(dest, 0, rev, 0, 256);

		for (int i = 0; i < 256; ++i)
			UNITTEST_ASSERT(rev->at(i) == i);
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestCryptoAes);

// -------------------------------------------------------------------------- //
#endif
