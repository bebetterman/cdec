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

class  TestCdecStack : public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecStack)
		UNITTEST_METHOD(testValueTypeStack)
		UNITTEST_METHOD(testReferenceTypeStack)
	UNITTEST_SUITE_END()
	
public:
	void setUp()
	{
	}

	void testValueTypeStack()
	{
		ref<StackV<int> > stack = gc_new<StackV<int> >();
		UNITTEST_ASSERT(stack->Count() == 0 && stack->Empty());

		stack->Push(1);
		UNITTEST_ASSERT(stack->Count() == 1 && !stack->Empty());
		UNITTEST_ASSERT(stack->Peek() == 1);

		stack->Push(2);
		UNITTEST_ASSERT(stack->Count() == 2 && stack->Peek() == 2);

		UNITTEST_ASSERT(stack->Pop() == 2);
		UNITTEST_ASSERT(stack->Count() == 1 && stack->Peek() == 1);

		UNITTEST_ASSERT(stack->Pop() == 1);
		UNITTEST_ASSERT(stack->Count() == 0 && stack->Empty());
	}

	void testReferenceTypeStack()
	{
		ref<Stack<X> > stack = gc_new<Stack<X> >();
		UNITTEST_ASSERT(stack->Count() == 0 && stack->Empty());

		ref<X> x1 = gc_new<X>(1);
		stack->Push(x1);
		UNITTEST_ASSERT(stack->Count() == 1 && !stack->Empty());
		UNITTEST_ASSERT(stack->Peek() == x1);

		ref<X> x2 = gc_new<X>(2);
		stack->Push(x2);
		UNITTEST_ASSERT(stack->Count() == 2 && stack->Peek() == x2);

		UNITTEST_ASSERT(stack->Pop() == x2);
		UNITTEST_ASSERT(stack->Count() == 1 && stack->Peek() == x1);

		UNITTEST_ASSERT(stack->Pop() == x1);
		UNITTEST_ASSERT(stack->Count() == 0 && stack->Empty());
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecStack);

// -------------------------------------------------------------------------- //
#endif
