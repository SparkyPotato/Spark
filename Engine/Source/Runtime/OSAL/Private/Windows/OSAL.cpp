// Copyright 2020 SparkyPotato

#ifdef PLATFORM_WINDOWS

#include "OSAL/OSAL.h"

#include "OSAL.gen.cpp"

#include <Windows.h>

namespace OSAL
{
	void FatalOSError(const char* error)
	{
		// Ignoring return value because there's nothing we can do about it,
		// and are exiting anyways.
		MessageBoxA(nullptr, error, "Fatal Error", MB_ICONERROR);

		TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
	}
}

#endif
