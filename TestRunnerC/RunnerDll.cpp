// RunnerDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "runner.h"
#include "RunnerDll.h"

inline bool CompareArg(const char* v1, const char* v2)
{
#ifdef X_OS_WINDOWS
	return _stricmp(v1, v2) == 0;
#else
	return strcasecmp(v1, v2) == 0;
#endif
}

inline bool CompareArg(const char* v1, const char* v2, int len)
{
#ifdef X_OS_WINDOWS
	return _strnicmp(v1, v2, len) == 0;
#else
	return strncasecmp(v1, v2, len) == 0;
#endif
}

bool CheckArgs(int argc, const char* argv[])
{
	int i = 0;
	if (i < argc && argv[i][0] != '/')
	{
		g_RunningArgs.nameSuite	= argv[i];
		++i;

		if (i < argc && argv[i][0] != '/')
		{
			g_RunningArgs.nameCase = argv[i];
			++i;
		}
	}

	static const char keyOutput[] = "OUTPUT:";
	static int lenOutput = sizeof(keyOutput) / sizeof(keyOutput[0]) - 1;

	for (; i < argc; ++i)
	{
		const char* p = argv[i];
		if (*p != '/')
		{
			printf("Unexpected option: %s\n", p);
			return false;
		}

		++p;
		if (CompareArg(p, "DEBUG"))
			g_RunningArgs.flags |= fDebug;
		else if (CompareArg(p, "PAUSE"))
			g_RunningArgs.flags |= fPause;
		else if (CompareArg(p, "VERBOSE"))
			g_RunningArgs.flags |= fVerbose;
		else if (CompareArg(p, keyOutput, lenOutput))
			g_RunningArgs.nameLog = p + lenOutput;
		else
		{
			printf("Unknown option: /%s\n", p);
			return false;
		}
	}

	return true;
}

int DoRun(Runner& runner)
{
	if (g_RunningArgs.nameSuite.empty())
		runner.RunAll();
	else if (g_RunningArgs.nameCase.empty())
		runner.RunSuite(g_RunningArgs.nameSuite.c_str());
	else
		runner.RunCase(g_RunningArgs.nameSuite.c_str(), g_RunningArgs.nameCase.c_str());

	runner.Close();

	if (g_RunningArgs.flags & fPause)
	{
		puts("Press any key to exit...");

#ifdef _WINDOWS
		getch();
#else
		getchar();
#endif
		puts("\r\n");
	}

	return 0;
}

// -------------------------------------------------------------------------
// Export

int RunTest(const char* pcszDllPath, int nParams, const char* params[])
{
	bool fOk = CheckArgs(nParams, params);
	if (!fOk)
		return 1;

	Runner runner;

	g_RunningArgs.nameModule = pcszDllPath;

	if (!runner.TryLoad())
		return 2;

	return DoRun(runner);
}

int RunTest2(void* pTestProject, int nParams, const char* params[])
{
	if (pTestProject == NULL)
	{
		ASSERT(false);
		return 2;
	}

	bool fOk = CheckArgs(nParams, params);
	if (!fOk)
		return 1;

	Runner	runner;
	runner.SetProject((ITestProject*)pTestProject);

	return DoRun(runner);
}

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif

// -------------------------------------------------------------------------- //
