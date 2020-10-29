/*
	SparkBuild.Private.Entry
	Copyright 2020 SparkyPotato

	Entry-point
*/

#include "PlatformDefs.h"

#ifdef PLATFORM_WINDOWS

#include "ArgParse.h"
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
			throw Error(L"NO_ARGS");
		}

		ArgParser parser(args);
	}
	catch (Error& error)
	{
		error.PrintDiagnostic();
		return error.GetReturnValue();
	}
}

#endif
