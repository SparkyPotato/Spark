/*
	SparkBuild.Private.Entry
	Copyright 2020 SparkyPotato

	Entry-point
*/

#include "PlatformDefs.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Error.h"
#include "ModuleParser.h"

int wmain(int argc, wchar_t** argv)
{
	try
	{
		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency); 
		QueryPerformanceCounter(&start); // Get start time

		std::filesystem::path execPath = argv[0]; // Path of executable
		std::vector<std::wstring> args; // Vector for all other args

		std::wstring engineRoot = execPath.parent_path();
		engineRoot += L"\\..\\..\\..\\";
		SetCurrentDirectoryW(engineRoot.c_str()); // Start in the engine root folder

		for (int i = 1; i < argc; i++)
		{
			args.emplace_back(argv[i]);
		} // Place args into args vector

 		ArgParser parser(args);
		
		if (!parser.GetSwitch(L"rebuild"))
		{
			wprintf(L"Starting build. \n\n");
		}
		else
		{
			wprintf(L"Rebuilding. \n\n");
		}

 		BuildTree buildTree(parser); // Generate build tree
		ModuleParser moduleParser(parser, buildTree, execPath); // Parse module files and generate tupfiles

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
