// OSAL.Private.Memory
// Copyright 2020 SparkyPotato

// Abstraction for all Memory related functions

#ifdef PLATFORM_WINDOWS

#include "OSAL/Memory.h"

#include <Windows.h>

OSAL_API void* OSAL::RawAllocate(uint64 bytes)
{
	void* ptr = HeapAlloc(GetProcessHeap(), 0, bytes);
	if (!ptr)
	{
		FatalOSError("Heap allocation failed!");
	}
	return ptr;
}

#endif
