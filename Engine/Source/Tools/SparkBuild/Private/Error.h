// Copyright 2020 SparkyPotato

/// \file
/// Error emitter

#pragma once

#include "BasePlatform.h"
#include "Globals.h"

/// A fatal error that will stop building.
///
/// \param args Arguments to be outputted to the console.
///             Will be preceded by 'Error: '.
template <typename ...Args>
void Error(const Args&... args)
{
	BasePlatform::Output("Error: ", args...);

	throw -1;
}

/// A warning.
/// Will stop building if fatal warnings are enabled (-fatalwarnings).
/// 
/// \param args Arguments to be outputted to the console.
///             Will be preceded by 'Warning: '.
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

/// A verbose message.
/// Only outputs if verbose is enabled (-verbose).
/// 
/// \param args Arguments to be outputted to the console.
///             Will be preceded by 'Verbose: '.
template<typename ...Args>
void Verbose(const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_Verbose = false;

	if (!s_CheckedSwitch)
	{
		s_Verbose = CommandLine::GetSwitch("verbose");
		s_CheckedSwitch = true;
	}

	if (s_Verbose) { BasePlatform::Output("Verbose: ", args...); }
}

/// A verbose message.
/// Only outputs if verbose is enabled (-verbose).
/// 
/// \param identifier Whether or not to print the 'Verbose: ' identifier before the message.
/// \param args Arguments to be outputted to the console.
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
