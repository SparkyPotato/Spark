// Copyright 2020 SparkyPotato

/// \file
/// Global header of module Launch

#pragma once

#include "Launch.gen.h"

namespace Launch
{
	/// Startup the Engine. 
	/// FOR INTERNAL ENGINE USE ONLY.
	/// Is called by the entry-point of the executable module.
	/// SparkBuild takes care of everything with generated files.
	/// 
	/// \param launchPayload Platform-specific payload to pass to the entry-point.
	/// 
	/// \return Exit-code of the Engine.
	extern Launch_API int EntryPoint(void* launchPayload);
}
