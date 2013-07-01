#ifndef __RUNNERDLL_H__
#define __RUNNERDLL_H__

extern "C"
{
	int RunTest(const char* pcszDllPath, int nParams, const char* params[]);
	int RunTest2(void* pTestProject, int nParams, const char* params[]);
};

#endif // __RUNNERDLL_H__
