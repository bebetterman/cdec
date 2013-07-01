// -------------------------------------------------------------------------- //
//	文件名		：	runner.h
//	创建者		：	杨钢
//	创建时间	：	4/22/2008 9:24:05 AM
//	功能描述	：
//
// -------------------------------------------------------------------------- //

#pragma once

#include "stdafx.h"

// -------------------------------------------------------------------------- //

enum RunningFlags
{
	fDebug		= 1,
	fPause		= 2,
	fVerbose	= 4,
};

struct RunningArgs
{
	std::string nameModule, nameSuite, nameCase, nameLog;
	DWORD		flags;

	RunningArgs() : flags(0) {}
};

extern RunningArgs	g_RunningArgs;

// -------------------------------------------------------------------------- //

class Runner: ITestReport
{
private:
	HMODULE				_hModule;
	ITestProject*		_pProject;
	interface IReport*	_pReport;

private:
	int __stdcall AssertReport(const char* message, const char* file, int line);

public:
	Runner();
	~Runner() { Close(); }
	bool SetProject(ITestProject* pProject);
	bool	TryLoad();
	void	RunAll();
	void	RunSuite(const char* suiteName);
	void	RunCase(const char* suiteName, const char* caseName);
	void	Close();

private:
	void	_RunSuite(ITestSuite* pSuite);
};

// -------------------------------------------------------------------------- //
