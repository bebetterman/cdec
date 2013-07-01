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

static int CountO = 0;

interface ItfA: Object
{
	DECLARE_REF_CLASS(ItfA)

	virtual UINT GetSignature() = 0;
};

interface ItfB: Object
{
	DECLARE_REF_CLASS(ItfB)

	virtual UINT GetSignature() = 0;
};

class ClsO: public Object
{
	DECLARE_REF_CLASS(ClsO)

public:
	UINT	ConstructID;

	ClsO(): ConstructID(0) { ++CountO; }
	ClsO(int): ConstructID(1) { ++CountO; }
	ClsO(int, char, PCSTR): ConstructID(2) { ++CountO; }
	ClsO(ref<ClsO>): ConstructID(3) { ++CountO; }
	~ClsO() { --CountO; }

	int	Add(int a, int b) { return a + b; }
};

class ClsA: public ItfA
{
	DECLARE_REF_CLASS(ClsA)

	UINT GetSignature() { return 101; }
};

class ClsB: public ItfB
{
	DECLARE_REF_CLASS(ClsB)

	UINT GetSignature() { return 102; }
};

class ClsX: public ClsA, public ClsB
{
	DECLARE_REF_CLASS(ClsX)

};

// -------------------------------------------------------------------------- //

class TestCdecEssential: public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecEssential);
	UNITTEST_METHOD(testRefSimpleClass)
	UNITTEST_METHOD(testParameteredConstructor)
	UNITTEST_METHOD(testWeakRef)
	UNITTEST_METHOD(testRefCast)
	UNITTEST_METHOD(testRefCastWeak)
	UNITTEST_METHOD(testRefCastToWeak)
	UNITTEST_METHOD(testRefCastFromWeak)
	UNITTEST_METHOD(testMultiInterfaces)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testRefSimpleClass()
	{
		// 测试 ref 的基本方法
		// 以及用 gc_new 创建对象到对象销毁的全过程
		ref<ClsO>	o;

		// ref 引用，析构函数自动释放
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsO>	r = gc_new<ClsO>();
			UNITTEST_ASSERT(CountO == 1);
		}
		UNITTEST_ASSERT(CountO == 0);

		// 使用 gc_new 创建的对象，如果没有接收者，自动销毁
		gc_new<ClsO>();
		UNITTEST_ASSERT(CountO == 0);

		// 调用方法
		o = gc_new<ClsO>();
		UNITTEST_ASSERT(o->Add(2, 3) == 5);

		// ref 锁住对象
		ref<ClsO>	p = o;
		UNITTEST_ASSERT(p == o && !(p != o) && p != NULL);
		p = NULL;
		UNITTEST_ASSERT(!(p == o) && p != o && p == NULL);
		UNITTEST_ASSERT(CountO == 1);

		o = NULL;
		UNITTEST_ASSERT(CountO == 0);
	}

	void testParameteredConstructor()
	{
		// 测试带有参数的构造函数
		UNITTEST_ASSERT(CountO == 0);	
		{
			ref<ClsO>	a = gc_new<ClsO>();
			UNITTEST_ASSERT(a->ConstructID == 0);
			
			ref<ClsO>	b = gc_new<ClsO>(1);
			UNITTEST_ASSERT(b->ConstructID == 1);
			
			ref<ClsO>	c = gc_new<ClsO>(1, 'a', "Hello");
			UNITTEST_ASSERT(c->ConstructID == 2);

			ref<ClsO>	d = gc_new<ClsO>(a);
			UNITTEST_ASSERT(d->ConstructID == 3);

			UNITTEST_ASSERT(CountO == 4);
		}
		UNITTEST_ASSERT(CountO == 0);

		// 补充测试，判断类型创建的正确性
		ref<ClsA>	a = gc_new<ClsA>();
		ref<ClsB>	b = gc_new<ClsB>();
		UNITTEST_ASSERT(a->GetSignature() == 101);
		UNITTEST_ASSERT(b->GetSignature() == 102);
		UNITTEST_ASSERT(CountO == 0);
	}

	void testWeakRef()
	{
		UNITTEST_ASSERT(CountO == 0);
		{
			// 同一类型间的 ref / weak_ref 互换，构造函数
			ref<ClsA>	a = gc_new<ClsA>();
			weak_ref<ClsA>	b = a;
			weak_ref<ClsA>	c = b;
			ref<ClsA>	d = c;
			UNITTEST_ASSERT(d->GetSignature() == 101);
		}

		UNITTEST_ASSERT(CountO == 0);
		{
			// 同一类型间的 ref / weak_ref 互换，赋值操作符
			ref<ClsA>	a = gc_new<ClsA>();
			weak_ref<ClsA>	b, c;
			b = a; c = b;
			ref<ClsA>	d;
			d = c;
			UNITTEST_ASSERT(d->GetSignature() == 101);
		}

		UNITTEST_ASSERT(CountO == 0);
		{
			// weak_ref 无法锁住生命周期
			ref<ClsA>	a = gc_new<ClsA>();
			weak_ref<ClsA>	b = a;
			a = NULL;
			UNITTEST_ASSERT(b != NULL && CountO == 0);

			weak_ref<ClsA> c = gc_new<ClsA>();
			UNITTEST_ASSERT(c != NULL && CountO == 0);
		}

		UNITTEST_ASSERT(CountO == 0);
	}

	void testRefCast()
	{
		// 测试 ref 引用之间的类型转换
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	a = gc_new<ClsA>();
			ref<ItfA>	ia = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			ref<ItfA>	ia2;
			ia2 = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	a = gc_new<ClsA>();
			ref<ItfA>	ia = a;
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			ref<ItfA>	ia2;
			ia2 = a;
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ItfA>	ia = gc_new<ClsA>();
			ref<ClsA>	a = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a->GetSignature() == 101);
			
			ref<ClsA>	a2;
			a2 = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
	}
	
	void testRefCastWeak()
	{
		// 测试 weak_ref 引用之间的类型转换
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	_a = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ClsA>	a = _a;
			weak_ref<ItfA>	ia = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			weak_ref<ItfA>	ia2;
			ia2 = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	_a = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ClsA>	a = _a;
			weak_ref<ItfA>	ia = a;
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			weak_ref<ItfA>	ia2;
			ia2 = a;
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ItfA>	_ia = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ItfA>	ia = _ia;
			weak_ref<ClsA>	a = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a->GetSignature() == 101);
			
			weak_ref<ClsA>	a2;
			a2 = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
	}

	void testRefCastToWeak()
	{
		// 测试从一种类型的 ref 转换到另一种类型的 weak_ref
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	a = gc_new<ClsA>();
			weak_ref<ItfA>	ia = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			weak_ref<ItfA>	ia2;
			ia2 = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	a = gc_new<ClsA>();
			weak_ref<ItfA>	ia = a;
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			weak_ref<ItfA>	ia2;
			ia2 = a;
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ItfA>	ia = gc_new<ClsA>();
			weak_ref<ClsA>	a = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a->GetSignature() == 101);
			
			weak_ref<ClsA>	a2;
			a2 = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
	}

	void testRefCastFromWeak()
	{
		// 测试从一种类型的 weak_ref 转换到另一种类型的 ref
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	_a = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ClsA>	a = _a;
			ref<ItfA>	ia = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			ref<ItfA>	ia2;
			ia2 = ref_cast<ItfA>(a);
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ClsA>	_a = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ClsA>	a = _a;
			ref<ItfA>	ia = a;
			UNITTEST_ASSERT(ia->GetSignature() == 101);
			
			ref<ItfA>	ia2;
			ia2 = a;
			UNITTEST_ASSERT(ia2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
		{
			ref<ItfA>	_ia = gc_new<ClsA>();	// 用于锁住对象
			weak_ref<ItfA>	ia = _ia;
			ref<ClsA>	a = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a->GetSignature() == 101);
			
			ref<ClsA>	a2;
			a2 = ref_cast<ClsA>(ia);
			UNITTEST_ASSERT(a2->GetSignature() == 101);
		}
		
		UNITTEST_ASSERT(CountO == 0);
	}

	void testMultiInterfaces()
	{

	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecEssential);

// -------------------------------------------------------------------------- //
#endif
