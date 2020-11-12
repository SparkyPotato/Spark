// SparkBuild.Private.BasePlatform
// Copyright 2020 SparkyPotato

// Platform-specific interface

#pragma once

#include <iomanip>
#include <iostream>
#include <filesystem>

#include "Globals.h"

struct File;
struct Module;

namespace BasePlatform
{
	bool SetWorkingDirectory(const String& directory);

	void SetupCompiler();
	void Compile(Module& buildModule, std::vector<File*> files);

	// Convert the wide-characters (UTF-16 on Windows) to a UTF-8 encoded String.
		String ToUTF8(const wchar_t* string);
	// Convert a UTF-8 encoded string to a wide-string - to pass to the Windows API, for example.
	std::wstring ToUTF16(const String& string);

	// Output to the console.
	inline void Output(const String& arg)
	{
		std::wcout << ToUTF16(arg) << std::endl;
	}

	template<typename ...Args>
	void Output(const String& arg, const Args&... args)
	{
		std::wcout << ToUTF16(arg);
		Output(args...);
	}

	template<typename T>
	void Output(const T& arg)
	{
		std::wcout << arg << std::endl;
	}

	template<typename T, typename ...Args>
	void Output(const T& arg, const Args&... args)
	{
		std::wcout << arg;
		Output(args...);
	}
}
