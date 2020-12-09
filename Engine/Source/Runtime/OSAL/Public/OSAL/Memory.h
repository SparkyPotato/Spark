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
	/// \return A pointer to the first byte of the allocated memory.
	/// 
	/// \throw FatalOSError If allocation of the requested memory failed.
	/// 
	/// \sa ::FatalOSError()
	extern OSAL_API void* RawAllocate(uint64 bytes);
}
