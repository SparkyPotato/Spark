// SparkBuild.Private.Globals
// Copyright 2020 SparkyPotato

// Global definitions

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
	void Setup(int argc, wchar_t** argv);
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

	inline const String& GetProperty(const String& property) { return Properties[property]; }
	inline bool GetSwitch(const String& property) { return std::find(Switches.begin(), Switches.end(), property) != Switches.end(); }
}
