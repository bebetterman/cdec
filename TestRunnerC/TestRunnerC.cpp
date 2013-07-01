// TestRunnerC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RunnerDll.h"

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		puts("TestRunnerC [-v] file_name [suite_name [case_name]] [/NODEBUG] [/VERBOSE] [/OUTPUT:output_file] [/PAUSE]");
		return -1;
	}

	return RunTest(argv[1], argc - 2, &argv[2]);
}
