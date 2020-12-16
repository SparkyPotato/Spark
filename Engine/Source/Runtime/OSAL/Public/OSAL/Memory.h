// Copyright 2020 SparkyPotato

/// \file
/// Abstraction for all Memory related functions

#pragma once

#include "OSAL/OSAL.h"

namespace OSAL
{
	/// Allocates on the heap.
	///
	/// \param bytes The number of bytes to request from the OS.
	///
	/// \return Pointer to the first byte of the allocated memory.
	/// 
	/// \throw FatalOSError If allocation of the requested memory failed.
	/// 
	/// \sa OSAL::FatalOSError()
	OSAL_API void* RawAllocate(uint64 bytes);

	/// Frees heap allocation.
	///
	/// \param pointer Pointer to the first byte of memory returned by RawAllocate().
	/// 
	/// \throw FatalOSError If deallocation of the memory failed.
	/// 
	/// \sa OSAL::FatalOSError()
	OSAL_API void RawFree(void* pointer);
}
