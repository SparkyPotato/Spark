// Launch.Private.Windows.EntryWindows
// Copyright 2020 SparkyPotato

// Entry-point for every Windows Spark-based application

#include "Launch/Launch.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

namespace Launch
{
	int Launch_API EntryPoint(void* launchPayload)
	{
		// Layout of the data passed in by wWinMain
		// DO NOT modify without also modifying SparkBuild.Private.BasePlatform,
		// PLATFORM_WINDOWS::EntryRoutine!
		struct Payload
		{
			HINSTANCE hInstance; 
			LPWSTR lpCmdLine; 
			int nShowCmd;
		};

		
		auto payload = reinterpret_cast<Payload*>(launchPayload);

		return EXIT_SUCCESS;
	}
}

#endif
