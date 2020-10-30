/*
	SparkBuild.Private.Entry
	Copyright 2020 SparkyPotato

	Entry-point
*/

#include "PlatformDefs.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "ArgParse.h"
#include "BuildTree.h"

#include "Error.h"

int wmain(int argc, wchar_t** argv)
{
	try
	{
		std::wstring startPath = argv[0]; // Store the path of the executable
		std::vector<std::wstring> args; // Vector for all other args

		for (int i = 1; i < argc; i++)
		{
			args.emplace_back(argv[i]);
		}

		if (args.empty())
		{
			wprintf(L"Warning: Empty Arguments. Building Spark with defaults. \n\n");
		}

 		ArgParser parser(args);
 
 		LARGE_INTEGER frequency, start, end;
 		QueryPerformanceFrequency(&frequency);
 
 		wprintf(L"Starting build \n\n");
 		QueryPerformanceCounter(&start);
 
 		BuildTree buildTree(parser);

		QueryPerformanceCounter(&end);
		float time = static_cast<float>(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;

		wprintf(L"Build finished. \nTook %.4f seconds. \n", time);

	}
	catch (Error& error)
	{
		error.PrintDiagnostic();
		return error.GetReturnValue();
	}

	return 0;
}

#endif
