/*
	SparkBuild.Private.Error
	Copyright 2020 SparkyPotato

	Error emitter
*/

#pragma once

#include "Globals.h"
#include "BasePlatform.h"

class Error
{
public:
	template <typename ...Args>
	Error(const String& errorCode, const Args&... args)
	{
		BasePlatform::Output("Error: ", errorCode, args...);

		throw this;
	}
};

class Warning
{
public:
	template<typename ...Args>
	Warning(const String& warning, const Args&... args)
	{
		BasePlatform::Output("Warning: ", warning, args...);

		if (!s_CheckedSwitch)
		{
			s_FatalWarnings = CommandLine::GetSwitch("fatalwarnings");
		}

		if (s_FatalWarnings) { throw this; }
	}

	inline static bool s_CheckedSwitch = false;
	inline static bool s_FatalWarnings = false;
};

template<typename ...Args>
void Verbose(const String& warning, const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_Verbose = false;

	if (!s_CheckedSwitch)
	{
		s_Verbose = CommandLine::GetSwitch("verbose");
	}

	if (s_Verbose) { BasePlatform::Output("Verbose: ", warning, args...); }
}

template<typename ...Args>
void Verbose(bool identifier, const String& warning, const Args&... args)
{
	static bool s_CheckedSwitch = false;
	static bool s_Verbose = false;

	if (!s_CheckedSwitch)
	{
		s_Verbose = CommandLine::GetSwitch("verbose");
	}

	std::string verbose = identifier ? "Verbose: " : "";
	if (s_Verbose) { BasePlatform::Output(verbose, warning, args...); }
}
