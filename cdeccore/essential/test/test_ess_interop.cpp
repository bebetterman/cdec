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

class PinptrSample
{
private:
	int		m_x;

private:
	PinptrSample(): m_x(0) {}

public:
	void	SetX(int x) { m_x = x; }
	int		GetX() const { return m_x; }

	static pin_ptr<PinptrSample> GetInstance()
	{
		static PinptrSample inst;
		return pin_ptr<PinptrSample>::CreatePinptr(&inst);
	}

	static const pin_ptr<PinptrSample> GetConstInstance()
	{
		return GetInstance();
	}
};

class SampleInterop: public Object
{
	DECLARE_REF_CLASS(SampleInterop)
};

// -------------------------------------------------------------------------- //

class TestCdecEssentialInterop: public UnitTestSuite
{
	UNITTEST_SUITE(TestCdecEssentialInterop)
		UNITTEST_METHOD(testPinPtr)
		UNITTEST_METHOD(testFromRef)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testPinPtr()
	{
		pin_ptr<PinptrSample>	pinSample;
		pinSample = PinptrSample::GetInstance();
		UNITTEST_ASSERT(pinSample->GetX() == 0);

		pin_ptr<PinptrSample>	pinSample2 = pinSample;
		UNITTEST_ASSERT(pinSample == pinSample2);
		UNITTEST_ASSERT(!(pinSample != pinSample2));
		(*pinSample2).SetX(123);

		pinSample2 = NULL;
		UNITTEST_ASSERT(pinSample != pinSample2);
		UNITTEST_ASSERT(!(pinSample == pinSample2));

		PinptrSample*	p = pinSample.ptr();
		UNITTEST_ASSERT(p->GetX() == 123);
		UNITTEST_ASSERT(pinSample == p);
		UNITTEST_ASSERT(!(pinSample != p));

		UNITTEST_ASSERT(pinSample2 != p);
		UNITTEST_ASSERT(!(pinSample2 == p));

		pinSample->SetX(0);
	}

	void testFromRef()
	{
		ref<SampleInterop>	s = gc_new<SampleInterop>();
		pin_ptr<SampleInterop> pinS = s;
		pinS = s;
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestCdecEssentialInterop);

// -------------------------------------------------------------------------- //
#endif
