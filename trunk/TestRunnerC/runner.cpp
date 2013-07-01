// -------------------------------------------------------------------------- //
//	文件名		：	runner.cpp
//	创建者		：	杨钢
//	创建时间	：	4/22/2008 9:24:08 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#include "stdafx.h"
#include "report.h"
#include "runner.h"

// -------------------------------------------------------------------------- //

RunningArgs	g_RunningArgs;

Runner::Runner(): _hModule(NULL), _pProject(NULL)
{
	if (!g_RunningArgs.nameLog.empty())
		_pReport = new ReportXml(g_RunningArgs.nameLog.c_str());
	else if (g_RunningArgs.flags & fVerbose)
		_pReport = new ReportConsoleVerbose();
	else
		_pReport = new ReportConsole;
}

bool Runner::SetProject(ITestProject* pProject)
{
	if (pProject == NULL)
	{
		ASSERT(false);
		return false;

	}
	_pProject = pProject;
	_pProject->SetReport(this);
	return true;
}

bool Runner::TryLoad()
{
	if (_pProject != NULL)
	{
		return true;
	}

	ASSERT(_hModule == NULL);
	const char* nameModule = g_RunningArgs.nameModule.c_str();

#ifdef X_OS_WINDOWS
	_hModule = LoadLibraryA(nameModule);
#else // linux
	_hModule = dlopen(nameModule, RTLD_NOW);
#endif

	if (_hModule == NULL)
	{
		printf("Unable to load module: %s\n", g_RunningArgs.nameModule.c_str());
		return false;
	}

	typedef ITestProject* (__stdcall* FuncPtr)();
#ifdef X_OS_WINDOWS
#	ifndef X_CPU_X64
		const char* PROCNAME = "_GetTestProject@0";
#	else
		const char* PROCNAME = "GetTestProject";
#	endif
	FuncPtr fp = (FuncPtr)GetProcAddress(_hModule, PROCNAME);
#else // linux
	FuncPtr fp = (FuncPtr)dlsym(_hModule,"GetTestProject");
#endif
	if (fp == NULL)
	{
		puts("Procedure GetTestProject not found.");
		return false;
	}

	ASSERT(_pProject == NULL);
	_pProject = fp();
	if (_pProject == NULL)
	{
		puts("Failed to get TestProject instance.");
		return false;
	}

	_pProject->SetReport(this);
	return true;
}

void Runner::RunAll()
{
	_pReport->BeginSession();
	int n = _pProject->GetSuiteCount();
	for (int i = 0; i < n; ++i)
	{
		ITestSuite* pSuite = _pProject->GetSuiteByIndex(i);
		ASSERT(pSuite != NULL);
		_RunSuite(pSuite);
	}
	_pReport->EndSession();
}

void Runner::RunSuite(const char* suiteName)
{
	ITestSuite* pSuite = _pProject->GetSuiteByName(g_RunningArgs.nameSuite.c_str());
	if (pSuite == NULL)
	{
		printf("Suite \"%s\" not found.\n", g_RunningArgs.nameSuite.c_str());
		return;
	}

	_pReport->BeginSession();
	_RunSuite(pSuite);
	_pReport->EndSession();
}

void Runner::RunCase(const char* suiteName, const char* caseName)
{
	ITestSuite* pSuite = _pProject->GetSuiteByName(g_RunningArgs.nameSuite.c_str());
	if (pSuite == NULL)
	{
		printf("Suite \"%s\" not found.\n", g_RunningArgs.nameSuite.c_str());
		return;
	}

	_pReport->BeginSession();
	_pReport->BeginSuite(suiteName);
	pSuite->BeginRun();

	_pReport->BeginCase(caseName);
	int r = pSuite->RunCaseByName(g_RunningArgs.nameCase.c_str());
	_pReport->EndCase(r);

	pSuite->EndRun();
	_pReport->EndSuite();
	_pReport->EndSession();

	if (r == ResultNotFound)
	{
		printf("Case \"%s\" not found in Suite \"%s\"\n", g_RunningArgs.nameCase.c_str(), g_RunningArgs.nameSuite.c_str());
	}
}

void Runner::_RunSuite(ITestSuite* pSuite)
{
	int n = pSuite->GetCaseCount();

	_pReport->BeginSuite(pSuite->GetName());
	pSuite->BeginRun();
	for (int i = 0; i < n; ++i)
	{
		_pReport->BeginCase(pSuite->GetCaseName(i));
		int r = pSuite->RunCaseByIndex(i);
		_pReport->EndCase(r);
	}
	pSuite->EndRun();
	_pReport->EndSuite();
}

void Runner::Close()
{
	_pProject = NULL;
	if (_hModule != NULL)
	{
#ifdef X_OS_WINDOWS
		FreeLibrary(_hModule);
#else // linux
		dlclose(_hModule);
#endif
		_hModule = NULL;
	}
	if (_pReport != NULL)
	{
		delete _pReport;
		_pReport = NULL;
	}
}

int Runner::AssertReport(const char* message, const char* file, int line)
{
	_pReport->AssertReport(message, file, line);
	return (g_RunningArgs.flags & fDebug) ? 1 : 0;
}

// -------------------------------------------------------------------------- //
