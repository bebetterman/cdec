#include "stdafx.h"
#include "../../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestCdecSortedMap : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecSortedMap)
		UNITTEST_METHOD(testGetSet)
		UNITTEST_METHOD(testEnum)
		UNITTEST_METHOD(testEnumKeys)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testGetSet()
	{
		int v;
		ref<SortedMapVV<int, int> > a = gc_new<SortedMapVV<int, int> >();
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
		typedef SortedMapVV<int, int> map_type;
		typedef map_type::KeyValuePair key_value_pair;

		ref<map_type> r = gc_new<map_type>();
		r->Insert(1, 100);
		r->Insert(4, 400);
		r->Insert(2, 200);
		r->Insert(5, 500);
		r->Insert(3, 300);

		int rt0[] = { 101, 202, 303, 404, 505 };
		VerifyRangeKV(r->GetEnumerator(), rt0, 5);		// 101 202 303 404 505
		VerifyRangeKV(r->EnumRange(2, 4), rt0 + 1, 3);	// 202 303 404
		VerifyRangeKV(r->EnumRange(2, 3), rt0 + 1, 2);	// 202 303
		VerifyRangeKV(r->EnumRange(2, 2), rt0 + 1, 1);	// 202
		
		r = gc_new<map_type>();
		r->Insert(2, 200);
		r->Insert(4, 400);
		r->Insert(6, 600);

		int rt1[] = { 202, 404, 606 };
		VerifyRangeKV(r->EnumRange(1, 7), rt1, 3);		// 202 404 606
		VerifyRangeKV(r->EnumRange(1, 6), rt1, 3);		// 202 404 606
		VerifyRangeKV(r->EnumRange(1, 5), rt1, 2);		// 202 404

		VerifyRangeKV(r->EnumRange(2, 7), rt1, 3);		// 202 404 606
		VerifyRangeKV(r->EnumRange(3, 7), rt1 + 1, 2);	// 404 606

		VerifyRangeKV(r->EnumRange(3, 3), NULL, 0);		// {}
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
		VerifyRange(r->EnumKeys(), rt0, 3);				// 2 4 6
		VerifyRange(r->EnumValues(), rt1, 3);			// 200 400 600
		
		VerifyRange(r->EnumKeysRange(2, 6), rt0, 3);	// 2 4 6
		VerifyRange(r->EnumValuesRange(2, 6), rt1, 3);	// 200 400 600
		
		VerifyRange(r->EnumKeysRange(3, 5), rt0 + 1, 1);	// 4
		VerifyRange(r->EnumValuesRange(3, 5), rt1 + 1, 1);	// 400

		VerifyRange(r->EnumKeysRange(3, 3), NULL, 0);	// {}
		VerifyRange(r->EnumValuesRange(3, 3), NULL, 0);	// {}
	}

	void VerifyRangeKV(ref<IEnumerator<std::pair<int, int> > > e, int rt[], int n)
	{
		ref<IEnumerable<std::pair<int, int> > > c = gc_new<Enumerable<std::pair<int, int> > >(e);
		VerifyRangeKV(c, rt, n);
	}

	void VerifyRangeKV(ref<IEnumerable<std::pair<int, int> > > e, int rt[], int n)
	{
		typedef std::pair<int, int> pair;
		std::vector<int> rr;
		foreach (pair, item, e)
			rr.push_back(item.first + item.second);
		UNITTEST_ASSERT(rr.size() == n);
		for (int i = 0; i < n; ++i)
			UNITTEST_ASSERT(rr[i] == rt[i]);
	}

	void VerifyRange(ref<IEnumerable<int> > e, int rt[], int n)
	{
		std::vector<int> rr;
		foreach (int, v, e)
			rr.push_back(v);
		UNITTEST_ASSERT(rr.size() == n);
		for (int i = 0; i < n; ++i)
			UNITTEST_ASSERT(rr[i] == rt[i]);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecSortedMap);

// -------------------------------------------------------------------------- //
#endif
