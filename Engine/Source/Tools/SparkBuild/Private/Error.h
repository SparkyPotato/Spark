// SparkBuild.Private.Error
// Copyright 2020 SparkyPotato

// Error emitter

#pragma once

#include "BasePlatform.h"
#include "Globals.h"

// A fatal error that will stop building IMMEDIATELY
template <typename ...Args>
void Error(const Args&... args)
{
	BasePlatform::Output("Error: ", args...);

	throw -1;
}

// A warning which will stop building if the "fatalwarnings" switch is turned on.
template<typename ...Args>
void Warning(const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_FatalWarnings = false;

	BasePlatform::Output("Warning: ", args...);

	// Getting a command-line switch is slow, so we cache the result
	if (!s_CheckedSwitch)
	{
		s_FatalWarnings = CommandLine::GetSwitch("fatalwarnings");
		s_CheckedSwitch = true;
	}

	if (s_FatalWarnings) { BasePlatform::Output("Fatal Warning - cannot proceed."); throw -1; }
}

// Verbose message that only outputs if the switch "verbose" is set.
template<typename ...Args>
void Verbose(const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_Verbose = false;

	// Getting a command-line switch is slow, so we cache the result
	if (!s_CheckedSwitch)
	{
		s_Verbose = CommandLine::GetSwitch("verbose");
		s_CheckedSwitch = true;
	}

	if (s_Verbose) { BasePlatform::Output("Verbose: ", args...); }
}

template<typename ...Args>
void Verbose(bool identifier, const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_Verbose = false;

	if (!s_CheckedSwitch)
	{
		s_Verbose = CommandLine::GetSwitch("verbose");
	}

	std::string verbose = identifier ? "Verbose: " : "";
	if (s_Verbose) { BasePlatform::Output(verbose, args...); }
}
