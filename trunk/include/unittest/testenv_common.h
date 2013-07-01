// ��Ԫ���ԵĹ��ýӿ�
//
// ���ļ�����עΪ UTF-8����Ϊ���������������õ����� VC 6
//
// �벻Ҫֱ�����ô��ļ������� testenv.h

#pragma once

namespace UnitTest
{

	enum RunCaseResult
	{
		ResultOK,					// �ɹ�
		ResultAssert,				// ���ֶ���ʧ��
		ResultException,			// �����쳣
		ResultExceptionNotRaised,	// �滮���쳣û��ִ��
		ResultNotFound,				// ����������
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
