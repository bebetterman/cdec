#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class IntObject: public Object
{
public:
	int		Value;
	IntObject(): Value(0) {}
	IntObject(int v): Value(v) {}
};

typedef SortedMapVV<int, int>		TestMapVV;
typedef SortedMapVR<int, IntObject> TestMapVR;

typedef TestMapVV::KeyValuePair		TestPairVV;
typedef TestMapVR::KeyValuePair		TestPairVR;

class MapTestKit
{
public:
	static void VerifyRange_VV(ref<IEnumerable<TestPairVV> > e, int rk[], int rv[], int n)
	{
		int i = 0;
		foreach (TestPairVV, item, e)
		{
			UNITTEST_ASSERT(i < n);
			UNITTEST_ASSERT(rk[i] == item.first);
			UNITTEST_ASSERT(rv[i] == item.second);
			++i;
		}
		UNITTEST_ASSERT(i == n);
	}

	static void VerifyRange_VR(ref<IEnumerable<TestPairVR> > e, int rk[], int rv[], int n)
	{
		int i = 0;
		foreach (TestPairVR, item, e)
		{
			UNITTEST_ASSERT(i < n);
			UNITTEST_ASSERT(rk[i] == item.first);
			UNITTEST_ASSERT(rv[i] == item.second->Value);
			++i;
		}
		UNITTEST_ASSERT(i == n);
	}

	static void VerifyRange_V(ref<IEnumerable<int> > e, int rt[], int n)
	{
		int i = 0;
		foreach (int, v, e)
		{
			UNITTEST_ASSERT(i < n);
			UNITTEST_ASSERT(rt[i] == v);
			++i;
		}
		UNITTEST_ASSERT(i == n);
	}

	static void VerifyRange_R(ref<IEnumerable<ref<IntObject> > > e, int rt[], int n)
	{
		int i = 0;
		foreach (ref<IntObject>, v, e)
		{
			UNITTEST_ASSERT(i < n);
			UNITTEST_ASSERT(rt[i] == v->Value);
			++i;
		}
		UNITTEST_ASSERT(i == n);
	}
};

// -------------------------------------------------------------------------- //

class TestCdecSortedMap : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecSortedMap)
		UNITTEST_METHOD(testGetSet)
		UNITTEST_METHOD(testEnum)
		UNITTEST_METHOD(testEnumKeys)
		UNITTEST_METHOD(testMapVR)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testGetSet()
	{
		int v;
		ref<TestMapVV> a = gc_new<TestMapVV>();
		UNITTEST_ASSERT(a->Get(1) == 0);
		UNITTEST_ASSERT(!a->TryGet(1, v));
		UNITTEST_ASSERT(a->Count() == 0);

		UNITTEST_ASSERT(a->Set(1, 101));
		UNITTEST_ASSERT(a->TryGet(1, v) && v == 101);
		UNITTEST_ASSERT(a->Get(1) == 101 && a->Count() == 1);

		UNITTEST_ASSERT(!a->Set(1, 102));
		UNITTEST_ASSERT(a->Get(1) == 102 && a->Count() == 1);

		UNITTEST_ASSERT(!a->Insert(1, 103));
		UNITTEST_ASSERT(a->Get(1) == 102 && a->Count() == 1);

		UNITTEST_ASSERT(a->Insert(2, 200));
		UNITTEST_ASSERT(a->Get(2) == 200 && a->Count() == 2);

		UNITTEST_ASSERT(a->Remove(1) && a->Count() == 1);
		UNITTEST_ASSERT(!a->Remove(1) && a->Count() == 1);

		UNITTEST_ASSERT(a->Remove(2) && a->Count() == 0);
	}
	
	void testEnum()
	{
		ref<TestMapVV> r = gc_new<TestMapVV>();
		r->Insert(1, 100);
		r->Insert(4, 400);
		r->Insert(2, 200);
		r->Insert(5, 500);
		r->Insert(3, 300);

		int rk0[] = { 1, 2, 3, 4, 5 };
		int rv0[] = { 100, 200, 300, 400, 500 };
		MapTestKit::VerifyRange_VV(r->Enum(), rk0, rv0, 5);		// 101 202 303 404 505
		MapTestKit::VerifyRange_VV(r->EnumRange(2, 4), rk0 + 1, rv0 + 1, 3);	// 202 303 404
		MapTestKit::VerifyRange_VV(r->EnumRange(2, 3), rk0 + 1, rv0 + 1, 2);	// 202 303
		MapTestKit::VerifyRange_VV(r->EnumRange(2, 2), rk0 + 1, rv0 + 1, 1);	// 202
		
		r = gc_new<TestMapVV>();
		r->Insert(2, 200);
		r->Insert(4, 400);
		r->Insert(6, 600);

		int rk1[] = { 2, 4, 6 };
		int rv1[] = { 200, 400, 600 };
		MapTestKit::VerifyRange_VV(r->EnumRange(1, 7), rk1, rv1, 3);		// 202 404 606
		MapTestKit::VerifyRange_VV(r->EnumRange(1, 6), rk1, rv1, 3);		// 202 404 606
		MapTestKit::VerifyRange_VV(r->EnumRange(1, 5), rk1, rv1, 2);		// 202 404

		MapTestKit::VerifyRange_VV(r->EnumRange(2, 7), rk1, rv1, 3);		// 202 404 606
		MapTestKit::VerifyRange_VV(r->EnumRange(3, 7), rk1 + 1, rv1 + 1, 2);	// 404 606

		MapTestKit::VerifyRange_VV(r->EnumRange(3, 3), NULL, NULL, 0);		// {}
	}

	void testEnumKeys()
	{
		typedef SortedMapVV<int, int> map_type;
		ref<map_type> r = gc_new<map_type>();
		r->Insert(2, 200);
		r->Insert(4, 400);
		r->Insert(6, 600);

		int rt0[] = { 2, 4, 6 };
		int rt1[] = { 200, 400, 600 };
		MapTestKit::VerifyRange_V(r->EnumKeys(), rt0, 3);				// 2 4 6
		MapTestKit::VerifyRange_V(r->EnumValues(), rt1, 3);			// 200 400 600
		
		MapTestKit::VerifyRange_V(r->EnumKeysRange(2, 6), rt0, 3);	// 2 4 6
		MapTestKit::VerifyRange_V(r->EnumValuesRange(2, 6), rt1, 3);	// 200 400 600
		
		MapTestKit::VerifyRange_V(r->EnumKeysRange(3, 5), rt0 + 1, 1);	// 4
		MapTestKit::VerifyRange_V(r->EnumValuesRange(3, 5), rt1 + 1, 1);	// 400

		MapTestKit::VerifyRange_V(r->EnumKeysRange(3, 3), NULL, 0);	// {}
		MapTestKit::VerifyRange_V(r->EnumValuesRange(3, 3), NULL, 0);	// {}
	}

	void testMapVR()
	{
		ref<TestMapVR> r = gc_new<TestMapVR>();

		ref<IntObject> v1 = gc_new<IntObject>(100);
		r->Insert(2, v1);
		r->Insert(1, gc_new<IntObject>(100));
		r->Insert(3, v1);

		r->at(1)->Value = 101;
		r->at(2)->Value = 102;

		int rk1[] = { 1, 2, 3 };
		int rv1[] = { 101, 102, 102 };
		MapTestKit::VerifyRange_VR(r->Enum(), rk1, rv1, 3);
		MapTestKit::VerifyRange_V(r->EnumKeys(), rk1, 3);
		MapTestKit::VerifyRange_R(r->EnumValues(), rv1, 3);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecSortedMap);

// -------------------------------------------------------------------------- //
#endif
