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

class TestCdecEssVariant: public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecEssVariant)
		UNITTEST_METHOD(testVariantCoreLiteralGet)
		UNITTEST_METHOD(testVariantCoreLiteralSet)
		UNITTEST_METHOD(testVariantCoreObject)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testVariantCoreLiteralGet()
	{
		variant_core a;
		a.SetInt64(0x1122334455667788L);

		UNITTEST_ASSERT(a.GetInt64() == 0x1122334455667788L);
		UNITTEST_ASSERT(a.GetInt32() == 0x55667788);
		UNITTEST_ASSERT(a.GetInt16() == 0x7788);
		UNITTEST_ASSERT(a.GetInt8() == (INT8)0x88);

		UNITTEST_ASSERT(a.GetUInt64() == 0x1122334455667788L);
		UNITTEST_ASSERT(a.GetUInt32() == 0x55667788);
		UNITTEST_ASSERT(a.GetUInt16() == 0x7788);
		UNITTEST_ASSERT(a.GetUInt8() == 0x88);

#ifdef X_OS_WINDOWS
		UNITTEST_ASSERT(a.GetBool());
#else
		UNITTEST_ASSERT(a.GetBool() == 0x88);
		a.SetBool(true);
		UNITTEST_ASSERT(a.GetBool());
#endif
		a.ClearLiteral();
	}

	void testVariantCoreLiteralSet()
	{
		variant_core a;
		a.SetInt8((INT8)0x88);
		UNITTEST_ASSERT(a.Value == 0x0000000000000088L);
		a.ClearLiteral();

		a.SetInt16(0x7788);
		UNITTEST_ASSERT(a.Value == 0x0000000000007788L);
		a.ClearLiteral();

		a.SetInt32(0x55667788);
		UNITTEST_ASSERT(a.Value == 0x0000000055667788L);
		a.ClearLiteral();

		a.SetInt64(0x1122334455667788);
		UNITTEST_ASSERT(a.Value == 0x1122334455667788L);
		a.ClearLiteral();

		a.SetDouble(1.23);
		UNITTEST_ASSERT(Math::Equal(a.GetDouble(), 1.23));
		a.ClearLiteral();

		BYTE p[] = "3210";
		a.SetPtr(p);
		UNITTEST_ASSERT(a.GetPtr() == p);
		a.ClearLiteral();
	}

	void testVariantCoreObject()
	{
		variant_core a;
		a.SetString(__X("Hello kitty"));
		UNITTEST_ASSERT(a.GetString() == __X("Hello kitty"));
		a.ClearString();

		ref<ByteArray> b = gc_new<ByteArray>((const BYTE*)"3210", 4);
		a.SetObject(b);
		ref<ByteArray> t = ref_cast<ByteArray>(a.GetObject());
		UNITTEST_ASSERT(t == b && BitConverter::ReadInt32(t, 0) == 0x30313233);
		a.ClearObject();
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecEssVariant);

// -------------------------------------------------------------------------- //
#endif
