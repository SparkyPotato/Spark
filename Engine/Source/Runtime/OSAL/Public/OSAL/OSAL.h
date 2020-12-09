// OSAL.Public.OSAL
// Copyright 2020 SparkyPotato

// Global header of module OSAL

#pragma once

#include "OSAL/Types.h"

#include "OSAL.gen.h"

namespace OSAL
{
	/// Tries to show the error to the user and instantly terminates the application.
	/// Is not available outside the OSAL module
	/// 
	/// \warning If called, OS resource handles may be leaked. Is only a last-resort,
	/// and should only be used if there is an absolutely irrecoverable event.
	void FatalOSError(const char* error);
}
