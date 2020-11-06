/*
	SparkBuild.Private.BasePlatform
	Copyright 2020 SparkyPotato

	Platform-specific interface
*/

#pragma once

#include "Globals.h"

#include <iostream>
#include <iomanip>

namespace BasePlatform
{
	bool SetWorkingDirectory(const String& directory);

	String ToUTF8(const wchar_t* string);
	std::wstring ToUTF16(const String& string);

	template<typename T, typename ...Args>
	void Output(const T& arg, const Args&... args)
	{
		std::wcout << arg;
		Output(args...);
	}

	template<typename T>
	void Output(const T& arg)
	{
		std::wcout << arg << std::endl;
	}

	template<typename ...Args>
	void Output(const String& arg, const Args&... args)
	{
		std::wcout << ToUTF16(arg);
		Output(args...);
	}

	inline void Output(const String& arg)
	{
		std::wcout << ToUTF16(arg) << std::endl;
	}

	template<typename ...Args>
	void Output(const char* arg, const Args&... args)
	{
		std::wcout << ToUTF16(arg);
		Output(args...);
	}

	inline void Output(const char* arg)
	{
		std::wcout << ToUTF16(arg) << std::endl;
	}
}
