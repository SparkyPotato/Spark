// Copyright 2020 SparkyPotato

#pragma once

#ifdef _WIN32
#	ifdef _WIN64
#		define PLATFORM_WINDOWS
#	else
#		error 32 bit is not supported!
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error iOS simulator not supported!
#	elif TARGET_OS_IPHONE == 1
#		error iOS is not supported!
#	elif TARGET_OS_MAC == 1
#		define PLATFORM_MAC
#	else
#		error Unknown Apple platform!
#	endif
#elif defined(__ANDROID__)
#	error Android is not supported!
#elif defined(__linux__)
#	define PLATFORM_LINUX
#else
#	error Unknown platform!
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

enum class BuildType
{
	Build, Rebuild, Clean
};

enum class BuildConfig
{
	Debug, Development, Release
};

struct Module
{
	std::string Version;
	std::vector<std::string> Dependencies;
	bool Executable;

	fs::path Definition;
	int64_t WriteTime;
	std::string Hash;
};

namespace Global
{
	extern fs::path ProjectPath;
	extern BuildType Type;
	extern BuildConfig Config;
	extern std::unordered_map<std::string, Module> Registry;

	/// Loads the global module registry into Globals::Registry
	void LoadRegistry();
	/// Saves the module registry into {EngineRoot}/Registry/Modules
	void SaveRegistry();
}
