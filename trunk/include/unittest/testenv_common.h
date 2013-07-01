// 单元测试的公用接口
//
// 此文件不标注为 UTF-8，因为其他编译器可能用到，如 VC 6
//
// 请不要直接引用此文件，改用 testenv.h

#pragma once

namespace UnitTest
{

	enum RunCaseResult
	{
		ResultOK,					// 成功
		ResultAssert,				// 出现断言失败
		ResultException,			// 出现异常
		ResultExceptionNotRaised,	// 规划的异常没有执行
		ResultNotFound,				// 案例不存在
	};

	interface ITestReport
	{
		virtual int __stdcall AssertReport(const char* message, const char* file, int line) = 0;
	};

	interface ITestSuite
	{
		virtual const char* __stdcall GetName() = 0;
		virtual int __stdcall GetCaseCount() = 0;
		virtual const char* __stdcall GetCaseName(int) = 0;

		virtual void __stdcall BeginRun() = 0;
		virtual int __stdcall RunCaseByIndex(int) = 0;
		virtual int __stdcall RunCaseByName(const char*) = 0;
		virtual void __stdcall EndRun() = 0;
	};

	interface ITestProject
	{
		virtual int __stdcall GetSuiteCount() = 0;
		virtual ITestSuite* __stdcall GetSuiteByIndex(int) = 0;
		virtual ITestSuite* __stdcall GetSuiteByName(const char*) = 0;
		virtual void __stdcall SetReport(ITestReport*) = 0;
	};
}
