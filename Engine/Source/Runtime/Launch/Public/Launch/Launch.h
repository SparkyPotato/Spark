// Launch.Public.Launch
// Copyright 2020 SparkyPotato

// Global header for the Launch module

#pragma once

#include "Core/Core.h"

#ifdef BUILD_Launch
#	define Launch_API MODULE_BUILD
#else
#	define Launch_API MODULE
#endif
