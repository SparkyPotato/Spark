/*
	SparkBuild.Private.Entry
	Copyright 2020 SparkyPotato

	Entry-point
*/

#include "Globals.h"

#ifdef PLATFORM_WINDOWS

#include "Globals.h"
#include "Error.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <fcntl.h>
#include <io.h>

int wmain(int argc, wchar_t** argv)
{
	try
	{
		int ret = _setmode(_fileno(stdout), _O_U8TEXT);
		setvbuf(stdout, nullptr, _IOFBF, 1000);

		LARGE_INTEGER frequency, start, end;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		// Begin profiling

		Globals::Setup(argc, argv);

		QueryPerformanceCounter(&end);
		auto time = static_cast<float>(end.QuadPart - start.QuadPart);
		time /= frequency.QuadPart;
		// End profiling

		BasePlatform::Output("Updated modules");
		BasePlatform::Output("Took ", time, "s");

		return 0;
	}
	catch (...)
	{
		return -1;
	}
}

#endif
