#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class X : public Object
{
	DECLARE_REF_CLASS(X)
	int v;
	X(): v(0) {}
	X(int _v): v(_v) {}
	~X() {}

public:
	int& V() { return v; }
};

typedef Array<X> XArray;

struct Z
{
	int v;
	Z() : v(0) {}
	~Z() {}
};

// -------------------------------------------------------------------------- //

class TestCdecArray : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecArray)
		UNITTEST_METHOD(testValueTypeArray)
		UNITTEST_METHOD(testReferenceTypeArray)
		UNITTEST_METHOD(testConstructors)
		UNITTEST_METHOD(testEnumArrayV)
		UNITTEST_METHOD(testEnumArray)
		UNITTEST_METHOD(testSort)
		UNITTEST_METHOD(testCopyFrom)
		UNITTEST_METHOD(testMemoryCompare)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testValueTypeArray()
	{
		ref<ArrayV<int> > a = gc_new<ArrayV<int> >(10);
		UNITTEST_ASSERT(a->Count() == 10);
		UNITTEST_ASSERT(a->at(0) == 0);

		for (int i = 0; i < 10; ++i)
		{
			a->at(i) = i;
		}
		UNITTEST_ASSERT(a->at(5) == 5);

		ref<ArrayV<Z> > b = gc_new<ArrayV<Z> >(5);
		UNITTEST_ASSERT(b->Count() == 5);
		UNITTEST_ASSERT(b->at(0).v == 0);
	}
	
	void testReferenceTypeArray()
	{
		ref<Array<X> > a = gc_new<Array<X> >(3);
		UNITTEST_ASSERT(a->Count() == 3);

		a->at(0) = gc_new<X>();
		UNITTEST_ASSERT(a->at(0)->V() == 0);

		a->at(1) = gc_new<X>(4);
		UNITTEST_ASSERT(a->at(1)->V() == 4);
	}

	void testConstructors()
	{
		int r[] = { 1, 2, 3, 4 };
		ref<IntArray> a = gc_new<IntArray>(r, 4);
		UNITTEST_ASSERT(a->Count() == 4 && CheckSequence(a, r, 4));

		ref<IntArray> b = gc_new<IntArray>(a, 1, 2);
		UNITTEST_ASSERT(b->Count() == 2 && CheckSequence(b, r + 1, 2));

		ref<X> x = gc_new<X>(1);
		ref<X> xr[] = { x, x, x, x };
		ref<XArray> xa = gc_new<XArray>(xr, 4);
		x->V() = 2;
		UNITTEST_ASSERT(xa->Count() == 4 && xa->at(0)->V() == 2 && xa->at(3)->V() == 2);

		ref<XArray> xb = gc_new<XArray>(xa, 1, 2);
		x->V() = 3;
		UNITTEST_ASSERT(xb->Count() == 2 && xb->at(0)->V() == 3 && xb->at(1)->V() == 3);
	}

	void testEnumArrayV()
	{
		ref<ArrayV<int> > arr = gc_new<ArrayV<int> >(0);
		int c = 0;
		foreach (int, v, arr)
			++c;
		UNITTEST_ASSERT(c == 0);

		arr = gc_new<ArrayV<int> >(3);
		const int buf [3] = { 1, 2, 3 };
		for (int i = 0; i < 3; ++i)
			arr->at(i) = buf[i];
		foreach (int, v, arr)
			UNITTEST_ASSERT(v == buf[c++]);
		UNITTEST_ASSERT(c == 3);
	}

	void testEnumArray()
	{
		ref<Array<X> > arr = gc_new<Array<X> >(0);
		int c = 0;
		foreach (ref<X>, v, arr)
			++c;
		UNITTEST_ASSERT(c == 0);

		ref<X> buf[3];
		buf[0] = gc_new<X>(1);
		buf[1] = NULL;
		buf[2] = gc_new<X>(3);

		arr = gc_new<Array<X> >(3);
		for (int i = 0; i < 3; ++i)
			arr->at(i) = buf[i];
		foreach (ref<X>, v, arr)
			UNITTEST_ASSERT(v == buf[c++]);
		UNITTEST_ASSERT(c == 3);
	}

	void testSort()
	{
		int v1s[] = { 1, 9, 8, 0 }, v1t[] = { 0, 1, 8, 9 };
		ref<ArrayV<int> > a = gc_new<ArrayV<int> >(v1s, 4);
		a->Sort();
		UNITTEST_ASSERT(CheckSequence(a, v1t, 4));
	}

	void testCopyFrom()
	{
		int v1s[] = { 1, 2, 3, 4 }, v1t[] = { 9, 8, 7, 6, 5, 4 };
		int v1r[] = { 9, 1, 2, 3, 4, 4 };
		ref<IntArray> a = gc_new<IntArray>(v1s, 4);
		ref<IntArray> b = gc_new<IntArray>(v1t, 6);
		b->CopyFrom(1, a, 0, 4);
		UNITTEST_ASSERT(CheckSequence(b, v1r, 6));

		a = gc_new<IntArray>(v1s, 4);
		b = gc_new<IntArray>(v1t, 6);
		a->CopyFrom(0, b, 2, 2);
		int v2r[] = { 7, 6, 3, 4 };
		UNITTEST_ASSERT(CheckSequence(a, v2r, 4));
	}

	void testMemoryCompare()
	{
		BYTE vs[] = { 1, 2, 3, 4, 5 };
		ref<ByteArray> a = gc_new<ByteArray>(vs, 5);
		ref<ByteArray> b = gc_new<ByteArray>(vs, 5);
		UNITTEST_ASSERT(a != b && a->GetBuffer() != b->GetBuffer());
		UNITTEST_ASSERT(a->MemoryCompare(1, b, 1, 4) == 0);
		UNITTEST_ASSERT(a->MemoryCompare(0, b, 1, 4) == -1);
		UNITTEST_ASSERT(a->MemoryCompare(0, b, 1, 0) == 0);

		UNITTEST_ASSERT(a->MemoryCompare(0, vs, 5) == 0);
	}
	
	static bool CheckSequence(ref<IntArray> arr, const int v[], int n)
	{
		if (arr->Count() != n)
			return false;
		int i = 0;
		foreach (int, a, arr)
		{
			if (a != v[i++])
				return false;
		}
		return true;
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecArray);

// -------------------------------------------------------------------------- //
#endif
