// Copyright 2020 SparkyPotato

/// \file
/// Platform-specific interface

#pragma once

#include <iomanip>
#include <iostream>
#include <filesystem>

#include "Globals.h"

struct File;
struct Module;

namespace BasePlatform
{
	/// Set the current working directory of SparkBuild.
	///
	/// \param directory Directory to switch to, relative to the current working directory.
	/// 
	/// \return Whether the directory-change was successful.
	bool SetWorkingDirectory(const String& directory);

	/// Setup the compiler and linker, and all standard options for them.
	void SetupCompiler();

	/// Compile the specific files of the module.
	///
	/// \param buildModule The module that is being compiled.
	/// \param files List of files in the module to compile. Does not check if the files
	///              are in the correct directory.
	void Compile(Module& buildModule, std::vector<File*> files);

	/// Generate module export definition.
	/// Exports are used to enable modules to depend on each other and still get linked.
	///
	/// \param buildModule The module to generate exports for.
	void GenerateExports(Module& buildModule);

	/// Link a module to produce a dynamic library.
	///
	/// \param buildModule The module to link.
	void Link(Module& buildModule);

	/// Generate the entry point for the module.
	/// This could be a dynamic library entry point, or an executable one.
	/// 
	/// \param buildModule The module to generate the entry point for.
	/// \param source Where to output the generated file.
	void EntryRoutine(Module& buildModule, std::ostream& source);

	/// Convert the wide-characters (UTF-16 on Windows) to a UTF-8 encoded string.
	///
	/// \param string Null-terminated C-string to convert.
	String ToUTF8(const wchar_t* string);

	/// Convert a UTF-8 encoded string to a wide-string.
	///
	/// \param string UTF-8 encoded string to convert.
	std::wstring ToUTF16(const String& string);

	/// Output to the console.
	///
	/// \param arg String to output.
	inline void Output(const String& arg)
	{
		std::wcout << ToUTF16(arg) << std::endl;
	}

	/// Output to the console.
	///
	/// \param arg String to output.
	/// \param args Rest of the arguments.
	template<typename ...Args>
	void Output(const String& arg, const Args&... args)
	{
		std::wcout << ToUTF16(arg);
		Output(args...);
	}

	/// Output to the console.
	///
	/// \param arg Variable to output.
	template<typename T>
	void Output(const T& arg)
	{
		std::wcout << arg << std::endl;
	}

	/// Output to the console.
	///
	/// \param arg Variable to output.
	/// \param args Rest of the arguments.
	template<typename T, typename ...Args>
	void Output(const T& arg, const Args&... args)
	{
		std::wcout << arg;
		Output(args...);
	}

	/// Definition for exported symbols (__declspec(dllexport) on Windows).
	extern String ExportDefine;
	/// Definition for imported symbols.
	extern String ImportDefine;
}
