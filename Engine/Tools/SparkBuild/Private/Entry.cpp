// SparkBuild.Private.Entry
// Copyright 2020 SparkyPotato

// Entry point

#include <fcntl.h>
#include <io.h>

#include "Error.h"
#include "Globals.h"
#include "SourceTree.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int wmain(int argc, wchar_t** argv)
{
	try
	{
		// Setting up the console output mode to UTF-8
		if (_setmode(_fileno(stdout), _O_U8TEXT) == -1)
		{ Warning("Failed to set console output mode, non-ASCII characters may not display correctly");}

		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		// Begin profiling

		Globals::Setup(argc, argv);

		// The source tree will generate itself
		SourceTree sourceTree;

		// End profiling
		QueryPerformanceCounter(&end);
		auto time = static_cast<float>(end.QuadPart - start.QuadPart); // Get ticks between start and end
		time /= frequency.QuadPart; // Divide by frequency to get time in seconds

		BasePlatform::Output("Updated.");
		BasePlatform::Output("Total time: ", std::fixed, std::setprecision(4), time, "s.");
		//                                   --------------------------------
		// This ensures that time shown has 4 decimal places -^

		return EXIT_SUCCESS;
	}
	catch (...)
	{
		BasePlatform::Output("Fatal Error.");
		return EXIT_FAILURE;
	}
}

#endif
