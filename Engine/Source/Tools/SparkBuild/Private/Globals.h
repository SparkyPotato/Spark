// Copyright 2020 SparkyPotato

/// \file
/// Global definitions

#pragma once

#ifdef _WIN32
#	ifdef _WIN64
#		define ON_WINDOWS
#	else
#		error 32-bit is not supported!
#	endif
#endif

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

#include "json/json.hpp"

using json = nlohmann::json;
using String = std::string;
using StringView = std::string_view;

namespace fs = std::filesystem;

namespace Globals
{
	/// Store all command-line arguments and the build cache.
	///
	/// \param argc Argument count from main.
	/// \param argv Arguments from main.
	void Setup(int argc, wchar_t** argv);

	/// Save the build cache to disk.
	void Save();

	extern fs::path SourcePath;
	extern fs::path IntermediatePath;
	extern fs::path GeneratedPath;
	extern fs::path BinariesPath;

	extern json ModuleRegistry;
	extern json BuildCache;
	extern bool BuildCacheExists;
};

namespace CommandLine
{
	extern std::map<String, String> Properties;
	extern std::vector<String> Switches;

	/// Get the value of the property.
	///
	/// \param property The property to get.
	/// 
	/// \return The value of the property. Is an empty string if it does not exist.
	inline const String& GetProperty(const String& property) { return Properties[property]; }

	/// Get the value of the switch.
	///
	/// \param property The switch to get.
	/// 
	/// \return The value of the switch (on, or off). Returns off if the switch does not exist.
	inline bool GetSwitch(const String& property) { return std::find(Switches.begin(), Switches.end(), property) != Switches.end(); }
}
