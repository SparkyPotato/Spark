// Launch.Private.Windows.EntryWindows
// Copyright 2020 SparkyPotato

// Entry-point for every Windows Spark-based application

#include "Launch/Launch.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

namespace Launch
{
	int Launch_API EntryPoint()
	{
		return EXIT_SUCCESS;
	}
}

#endif
