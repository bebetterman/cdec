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
	int Getv() { return v; }
};

struct Z
{
	int v;
	Z(int n) : v(n) {}
	~Z() {}
};

class  TestCdecArrayList : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecArrayList)
		UNITTEST_METHOD(testValueTypeArrayList)
		UNITTEST_METHOD(testReferenceTypeArrayList)
		UNITTEST_METHOD(testEnumArrayListV)
		UNITTEST_METHOD(testEnumArrayList)
		UNITTEST_METHOD(testSort)
	UNITTEST_SUITE_END()
	
public:
	void setUp()
	{
	}

	void testValueTypeArrayList()
	{
		ref<ArrayListV<int> >  vecBuffer = gc_new<ArrayListV<int> >();
		int test1 = 10;
		int test2 = 12355;
		int test3 = 0;
		int test4 = -1;
		vecBuffer->Add(test1);
		vecBuffer->Add(test2);
		vecBuffer->Add(test3);
		vecBuffer->Add(test4);

		UNITTEST_ASSERT(!vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 4);

		UNITTEST_ASSERT(vecBuffer->at(0) == 10);
		UNITTEST_ASSERT(vecBuffer->at(1) == 12355);
		UNITTEST_ASSERT(vecBuffer->at(2) == 0);
		UNITTEST_ASSERT(vecBuffer->at(3) == -1);

		UNITTEST_ASSERT(vecBuffer->Last() == -1);
		UNITTEST_ASSERT(vecBuffer->First() == 10);

		vecBuffer->RemoveAt(0);
		UNITTEST_ASSERT(vecBuffer->at(0) == 12355);
		UNITTEST_ASSERT(vecBuffer->First() == 12355);
		UNITTEST_ASSERT(!vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 3);

		vecBuffer->RemoveLast();
		UNITTEST_ASSERT(vecBuffer->at(0) == 12355);
		UNITTEST_ASSERT(vecBuffer->First() == 12355);
		UNITTEST_ASSERT(vecBuffer->Last() == 0);
		UNITTEST_ASSERT(!vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 2);

		vecBuffer->RemoveRange(0,2);
		UNITTEST_ASSERT(vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 0);

		vecBuffer->Add(test1);
		vecBuffer->Add(test2);
		vecBuffer->Add(test3);
		vecBuffer->Add(test4);

		UNITTEST_ASSERT(!vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 4);

		vecBuffer->at(0) = 15632;
		UNITTEST_ASSERT(vecBuffer->First() == 15632);
		
		int test5 = 5;
		vecBuffer->Insert(4,test5);
		UNITTEST_ASSERT(vecBuffer->Count() == 5);
		UNITTEST_ASSERT(vecBuffer->Last() == 5);

		vecBuffer->Clear();
		UNITTEST_ASSERT(vecBuffer->Empty());
		UNITTEST_ASSERT(vecBuffer->Count() == 0);

// 		UNITTEST_ASSERT(vecBuffer[0] == 10);
// 		UNITTEST_ASSERT(vecBuffer[1] == 12355);
// 		UNITTEST_ASSERT(vecBuffer[2] == 0);
// 		UNITTEST_ASSERT(vecBuffer[3] == -1);

		ref<ArrayListV<Z> > testStruct = gc_new<ArrayListV<Z> >();
		Z z1(10);
		Z z2(12355);
		Z z3(0);
		Z z4(-1);
		testStruct->Add(z1);
		testStruct->Add(z2);
		testStruct->Add(z3);
		testStruct->Add(z4);

		UNITTEST_ASSERT(!testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 4);

		UNITTEST_ASSERT(testStruct->at(0).v == 10);
		UNITTEST_ASSERT(testStruct->at(1).v == 12355);
		UNITTEST_ASSERT(testStruct->at(2).v == 0);
		UNITTEST_ASSERT(testStruct->at(3).v == -1);

		UNITTEST_ASSERT(testStruct->Last().v == -1);
		UNITTEST_ASSERT(testStruct->First().v == 10);

		testStruct->RemoveAt(0);
		UNITTEST_ASSERT(testStruct->at(0).v == 12355);
		UNITTEST_ASSERT(testStruct->First().v == 12355);
		UNITTEST_ASSERT(!testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 3);

		testStruct->RemoveLast();
		UNITTEST_ASSERT(testStruct->at(0).v == 12355);
		UNITTEST_ASSERT(testStruct->First().v == 12355);
		UNITTEST_ASSERT(testStruct->Last().v == 0);
		UNITTEST_ASSERT(!testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 2);

		testStruct->RemoveRange(0,2);
		UNITTEST_ASSERT(testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 0);

		testStruct->Add(z1);
		testStruct->Add(z2);
		testStruct->Add(z3);
		testStruct->Add(z4);

		UNITTEST_ASSERT(!testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 4);

		Z z5(5);
		testStruct->Insert(4,z5);
		UNITTEST_ASSERT(testStruct->Count() == 5);
		UNITTEST_ASSERT(testStruct->Last().v == 5);

		testStruct->Clear();
		UNITTEST_ASSERT(testStruct->Empty());
		UNITTEST_ASSERT(testStruct->Count() == 0);
		
	}

	void testReferenceTypeArrayList()
	{
		ref<ArrayList<X> > testClass = gc_new<ArrayList<X> >();
		ref<X> x1 = gc_new<X>(10);
		ref<X> x2 = gc_new<X>(12355);
		ref<X> x3 = gc_new<X>(0);
		ref<X> x4 = gc_new<X>(-1);
		
		testClass->Add(x1);
		testClass->Add(x2);
		testClass->Add(x3);
		testClass->Add(x4);

		UNITTEST_ASSERT(!testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 4);

		UNITTEST_ASSERT(testClass->at(0)->Getv() == 10);
		UNITTEST_ASSERT(testClass->at(1)->Getv() == 12355);
		UNITTEST_ASSERT(testClass->at(2)->Getv() == 0);
		UNITTEST_ASSERT(testClass->at(3)->Getv() == -1);

		UNITTEST_ASSERT(testClass->Last()->Getv() == -1);
		UNITTEST_ASSERT(testClass->First()->Getv() == 10);

		testClass->RemoveAt(0);
		UNITTEST_ASSERT(testClass->at(0)->Getv() == 12355);
		UNITTEST_ASSERT(testClass->First()->Getv() == 12355);
		UNITTEST_ASSERT(!testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 3);

		testClass->RemoveLast();
		UNITTEST_ASSERT(testClass->at(0)->Getv() == 12355);
		UNITTEST_ASSERT(testClass->First()->Getv() == 12355);
		UNITTEST_ASSERT(testClass->Last()->Getv() == 0);
		UNITTEST_ASSERT(!testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 2);

		testClass->RemoveRange(0,2);
		UNITTEST_ASSERT(testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 0);

		testClass->Add(x1);
		testClass->Add(x2);
		testClass->Add(x3);
		testClass->Add(x4);

		UNITTEST_ASSERT(!testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 4);

		ref<X> x5 = gc_new<X>(5);
		testClass->Insert(4,x5);
		UNITTEST_ASSERT(testClass->Count() == 5);
		UNITTEST_ASSERT(testClass->Last()->Getv() == 5);
		
		testClass->Clear();
		UNITTEST_ASSERT(testClass->Empty());
		UNITTEST_ASSERT(testClass->Count() == 0);			

	}

	void testEnumArrayListV()
	{
		ref<ArrayListV<int> > arr = gc_new<ArrayListV<int> >();
		int c = 0;
		foreach (int, v, arr)
			++c;
		UNITTEST_ASSERT(c == 0);

		const int buf [3] = { 1, 2, 3 };
		for (int i = 0; i < 3; ++i)
			arr->Add(buf[i]);
		foreach (int, v, arr)
			UNITTEST_ASSERT(v == buf[c++]);
		UNITTEST_ASSERT(c == 3);
	}

	void testEnumArrayList()
	{
		ref<ArrayList<X> > arr = gc_new<ArrayList<X> >();
		int c = 0;
		foreach (ref<X>, v, arr)
			++c;
		UNITTEST_ASSERT(c == 0);

		ref<X> buf[3];
		buf[0] = gc_new<X>(1);
		buf[1] = NULL;
		buf[2] = gc_new<X>(3);

		for (int i = 0; i < 3; ++i)
			arr->Add(buf[i]);
		foreach (ref<X>, v, arr)
			UNITTEST_ASSERT(v == buf[c++]);
		UNITTEST_ASSERT(c == 3);
	}

	void testSort()
	{
		ref<ArrayListV<int> > a = gc_new<ArrayListV<int> >();
		a->Add(1); a->Add(9); a->Add(8); a->Add(0);
		a->Sort();
		int v1[] = { 0, 1, 8, 9 };
		UNITTEST_ASSERT(CheckSequence(a, v1, 4));
	}

	static bool CheckSequence(ref<IntArrayList> arr, const int v[], int n)
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

UNITTEST_SUITE_REGISTRATION(TestCdecArrayList);

// -------------------------------------------------------------------------- //
#endif
