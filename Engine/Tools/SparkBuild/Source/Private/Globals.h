/*
	SparkBuild.Private.Globals
	Copyright 2020 SparkyPotato

	Global definitions
*/

#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS
	#else
		#error 32-bit is not supported!
	#endif
#endif

#include <string>
#include <map>
#include <filesystem>

using String = std::string;
using StringView = std::string_view;

namespace Globals
{
	void Setup(int argc, wchar_t** argv);
};

namespace CommandLine
{
	extern std::map<String, String> Properties;
	extern std::vector<String> Switches;

	inline const String& GetProperty(const String& property) { return Properties[property]; }
	inline bool GetSwitch(const String& property) { return std::find(Switches.begin(), Switches.end(), property) != Switches.end(); }
}
