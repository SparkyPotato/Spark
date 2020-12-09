// Copyright 2020 SparkyPotato

/// \file
/// Global header of module Launch

#pragma once

#include "Launch.gen.h"

namespace Launch
{
	/// Startup the Engine. 
	/// You shouldn't be calling this ever.
	/// 
	/// \param launchPayload Platform-specific payload to pass to the entry-point.
	/// 
	/// \return Exit-code of the Engine.
	extern Launch_API int EntryPoint(void* launchPayload);
}
