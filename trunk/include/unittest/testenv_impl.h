// 单元测试的公用接口
//
// 此文件不标注为 UTF-8，因为其他编译器可能用到，如 VC 6
//
// 请不要直接引用此文件，改用 testenv.h

#pragma once

// PORT: 2010.3.30
// 为解决 gcc 编译的模块符号冲突，增加了一层 namespace，需要为每一个模块定义一个
// _MODULE_NAME 宏，目前只能加在工程文件中，如 _MODULE_NAME=kfcwow
namespace UnitTest
{
    namespace _MODULE_NAME
    {

	struct AssertException
	{
	};

	class TestProject: public ITestProject
	{
	protected:
		ITestReport*				_report;
		std::vector<ITestSuite*>	_suites;

	public:
		TestProject(): _report(NULL) {}

		void	AddSuite(const char* name, ITestSuite* pSuite)
		{
			_suites.push_back(pSuite);
		}

		int __stdcall GetSuiteCount()
		{
			return _suites.size();
		}

		ITestSuite* __stdcall GetSuiteByIndex(int index)
		{
			return _suites[index];
		}

		ITestSuite* __stdcall GetSuiteByName(const char* name)
		{
			for (size_t i = 0; i < _suites.size(); ++i)
			{
				ITestSuite*	pSuite = _suites[i];
				if (strcmp(pSuite->GetName(), name) == 0)
					return pSuite;
			}
			return NULL;
		}

		void __stdcall SetReport(ITestReport* report)
		{
			ASSERT((_report == NULL) != (report == NULL));
			_report = report;
		}

		ITestReport* GetReport()
		{
			return _report;
		}
	};

	TestProject* _GetTestProject();

	class TestSuite: public ITestSuite
	{
	protected:
		typedef void (TestSuite::*Addr)();
		enum CaseFlags
		{
			fDefault = 0,
			fException = 1,		// 案例会抛出异常
		};
		struct CaseInf
		{
			const char*		_Name;
			Addr		_Addr;
			CaseFlags	_Flags;
			CaseInf(const char* name, Addr addr, CaseFlags flags):
			_Name(name), _Addr(addr), _Flags(flags)
			{
			}
		};
		typedef std::vector<CaseInf>	CaseList;
		CaseList		_TestMethods;
		const char*			_Name;

	protected:
		int InvokeCase(const CaseInf& infCase)
		{
			int	r = ResultOK;
			try
			{
				setUp();
				Addr addr = infCase._Addr;
				(this->*addr)();
				tearDown();
			}
			catch (AssertException)
			{
				r = ResultAssert;
			}
			catch (...)
			{
				r = ResultException;
			}
			if (infCase._Flags & fException)
			{
				if (r == ResultException)
					r = ResultOK;
				else if (r == ResultOK)
					r = ResultExceptionNotRaised;
			}
			return r;
		}

	public:
		virtual void setUp() = 0;
		virtual void tearDown() = 0;

		TestSuite(const char* Name): _Name(Name)
		{
			//impGetTestProject()->AddSuite(Name, this);
			_GetTestProject()->AddSuite(Name, this);
		}

		void AddCase(const char* name, Addr addr, CaseFlags flags = fDefault)
		{
			_TestMethods.push_back(CaseInf(name, addr, flags));
		}

		const CaseInf* GetCaseByName(const char* name)
		{
			for (CaseList::const_iterator it = _TestMethods.begin(); it != _TestMethods.end(); ++it)
			{
				if (strcmp(it->_Name, name) == 0)
					return &(*it);
			}
			return NULL;
		}

		const char* __stdcall GetName()
		{
			return _Name;
		}

		int __stdcall GetCaseCount()
		{
			return _TestMethods.size();
		}

		const char* __stdcall GetCaseName(int index)
		{
			return _TestMethods[index]._Name;
		}

		void __stdcall BeginRun()
		{
		}

		int __stdcall RunCaseByIndex(int index)
		{
			const CaseInf& infCase = _TestMethods[index];
			return InvokeCase(infCase);
		}

		int __stdcall RunCaseByName(const char* name)
		{
			const CaseInf* pCase = GetCaseByName(name);
			if (pCase == NULL)
				return ResultNotFound;
			return InvokeCase(*pCase);
		}

		void __stdcall EndRun()
		{
		}
	};

	inline void _TestAssert(bool e, const char* message, const char* file, int line)
	{
		if (!e)
		{
			//static ITestReport* rpt = impGetTestProject()->GetReport();
			static ITestReport* rpt = _GetTestProject()->GetReport();

			if (rpt->AssertReport(message, file, line) != 0)
#ifdef _WIN32
#	ifndef _WIN64
				__asm int 3;
#	else
				__debugbreak();
#	endif
#else // linux
                __asm__ __volatile__ ("int $3");
#endif
			else
				throw UnitTest::_MODULE_NAME::AssertException();
		}
	}

	inline void _TestAssert(int e, const char* message, const char* file, int line)
	{
		_TestAssert(e != 0, message, file, line);
	}

    }
}

// -------------------------------------------------------------------------- //

#define UNITTEST_SUITE(name)											\
public:																	\
	typedef name ThisType;												\
	name (): UnitTest::_MODULE_NAME::TestSuite(#name)					\
	{													    			\
		ASSERT((void*)this == (void*)(UnitTest::_MODULE_NAME::TestSuite*)this);

#ifdef _WIN32
#define UNITTEST_METHOD(name)											\
		AddCase(#name, (Addr)&ThisType:: ## name);
#else // linux
#define UNITTEST_METHOD(name)											\
		AddCase(#name, (Addr)&ThisType:: name);
#endif

#ifdef _WIN32
#define UNITTEST_METHOD_EXCEPTION(name)									\
		AddCase(#name, (Addr)&ThisType:: ## name, fException);
#else // linux
#define UNITTEST_METHOD_EXCEPTION(name)									\
		AddCase(#name, (Addr)&ThisType:: name, fException);
#endif

#define UNITTEST_SUITE_END()											\
	}

#define UNITTEST_SUITE_REGISTRATION(name)								\
	static name suite_ ## name;

#define UNITTEST_ASSERT(e)												\
	UnitTest::_MODULE_NAME::_TestAssert(e, "ASSERT(" #e ")", __FILE__, __LINE__)

#define UNITTEST_ASSERT_EXCEPTION(e)									\
	do {																\
		bool f = false;													\
		try {															\
			(e);														\
		}																\
		catch (...) {													\
		f = true;														\
		}																\
		UnitTest::_MODULE_NAME::_TestAssert(f, "ASSERT_EXCEPTION(" #e ")", __FILE__, __LINE__);	\
	} while(0)

#define UNITTEST_MODULE_IMPLEMENT										\
namespace UnitTest														\
{																		\
    namespace _MODULE_NAME {											\
        TestProject* _GetTestProject()									\
        {																\
            static TestProject project;									\
            return &project;											\
        }																\
    }															        \
	KFC_EXPORT_API_(ITestProject*) GetTestProject()						\
	{																	\
		return _MODULE_NAME::_GetTestProject();							\
	}																	\
}

#define UNITTEST_MODULE_IMPLEMENT_(x)									\
namespace UnitTest														\
{																		\
    namespace _MODULE_NAME {									        \
        TestProject* _GetTestProject()									\
        {																\
            static TestProject project;									\
            return &project;											\
        }																\
    }													                \
	EXTERN_C DECLSPEC_EXPORT ITestProject* __stdcall GetTestProject()	\
	{																	\
		(x);															\
		return _MODULE_NAME::_GetTestProject();							\
	}																	\
}

// -------------------------------------------------------------------------- //

#define UnitTestSuite						UnitTest::_MODULE_NAME::TestSuite

// -------------------------------------------------------------------------- //
