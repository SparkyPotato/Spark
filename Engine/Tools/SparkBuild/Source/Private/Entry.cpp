/*
	SparkBuild.Private.Entry
	Copyright 2020 SparkyPotato

	Entry-point
*/

#include "PlatformDefs.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "ModuleParser.h"

#include "Error.h"

int wmain(int argc, wchar_t** argv)
{
	try
	{
		std::filesystem::path startPath = argv[0]; // Store the path of the executable
		startPath = startPath.parent_path();
		std::filesystem::path execPath = argv[0];
		std::vector<std::wstring> args; // Vector for all other args

		std::wstring engineRoot = startPath;
		engineRoot += L"\\..\\..\\..\\";
		SetCurrentDirectoryW(engineRoot.c_str()); // Start in the engine root folder
		startPath = engineRoot;

		for (int i = 1; i < argc; i++)
		{
			args.emplace_back(argv[i]);
		}

 		ArgParser parser(args);
 
 		LARGE_INTEGER frequency, start, end;
 		QueryPerformanceFrequency(&frequency);
		
		if (parser.GetSwitch(L"rebuild"))
		{
			wprintf(L"Rebuilding. \n\n");
		}
		else if (parser.GetSwitch(L"clean"))
		{
			wprintf(L"Cleaning. \n\n");
		}
		else
		{
			wprintf(L"Starting build. \n\n");
		}

		QueryPerformanceCounter(&start);

 		BuildTree buildTree(parser);
		ModuleParser moduleParser(parser, buildTree, execPath);

		QueryPerformanceCounter(&end);
		auto time = static_cast<float>(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;

		wprintf(L"\nUpdated. Took %.4f seconds. \n", time);

		return 0;
	}
	catch (Error& error)
	{
		error.PrintDiagnostic();
		return -1;
	}
}

#endif
