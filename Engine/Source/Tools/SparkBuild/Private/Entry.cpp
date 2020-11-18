// SparkBuild.Private.Entry
// Copyright 2020 SparkyPotato

// Entry point

#include "Error.h"
#include "Executor.h"
#include "SourceTree.h"

#ifdef ON_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <fcntl.h>
#include <io.h>
#include <Windows.h>

int wmain(int argc, wchar_t** argv)
{
	try
	{
		LARGE_INTEGER frequency, start, end;
		QueryPerformanceCounter(&start);
		QueryPerformanceFrequency(&frequency);
		// Begin profiling

		// Setting up the console output mode to UTF-8
		if (_setmode(_fileno(stdout), _O_U8TEXT) == -1)
		{ Warning("Failed to set console output mode, non-ASCII characters may not display correctly"); }

		Globals::Setup(argc, argv);

		SourceTree* lastTree;
		if (Globals::BuildCacheExists) { lastTree = SourceTree::GenerateFromCache(); }
		else { lastTree = new SourceTree; }
		auto currentTree = SourceTree::GenerateFromDirectory();

		currentTree->CompareWithOld(*lastTree);

		Executor executor(*currentTree);
		executor.Parse();

		currentTree->GenerateDirectories();

		executor.CheckHeaders();
		executor.Compile();

		executor.AddHeaderDependencies();

		executor.Link();
		executor.CopyDependencies();

		SourceTree::SaveToCache(*currentTree);
		Globals::Save();

		BasePlatform::Output("Updated.");

		// End profiling
		QueryPerformanceCounter(&end);
		auto time = static_cast<float>(end.QuadPart - start.QuadPart); // Get ticks between start and end
		time /= frequency.QuadPart; // Divide by frequency to get time in seconds

		BasePlatform::Output("Total time: ", std::fixed, std::setprecision(3), time, "s.");
		//                                   --------------------------------
		// This ensures that time shown has 3 decimal places -^

		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		BasePlatform::Output(e.what());
	}
	catch (...)
	{}

	BasePlatform::Output("Fatal Error.");
	return EXIT_FAILURE;
}

#endif
