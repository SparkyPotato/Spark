// SparkBuild.Private.BasePlatform
// Copyright 2020 SparkyPotato

// Platform - specific interface

#include "BasePlatform.h"

// SparkBuild is a small tool with very little platform-specific functionality used.
// This is why we put all platform ifdefs in a single-file.

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

namespace BasePlatform
{
	bool SetWorkingDirectory(const String& directory)
	{
		wchar_t dir[5000];
		MultiByteToWideChar(CP_UTF8, 0, directory.c_str(), -1, dir, 5000);

		return SetCurrentDirectoryW(dir);
	}

	String ToUTF8(const wchar_t* string)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, nullptr, 0, nullptr, nullptr);
		String temp(size, 0);

		WideCharToMultiByte(CP_UTF8, 0, string, -1, &temp[0], size, nullptr, nullptr);
		temp.pop_back(); // Fixes a weird issue where we have double nulls

		// The double nulls are a problem because:
		// Consider string L"AB". This would be represented in memory as
		// [ 'A', 'B', '\0' ] - doesn't matter if it was in a std::wstring or a wchar_t array.
		//   ---  ---         : the underlined characters are counted in wstring.size(). 
		// What WideCharToMultiByte is doing is that it is storing L"AB" as
		// [ 'A', 'B', '\0', '\0' ] - inside of the std::string.
		//   ---  ---   ---         : where the underlined characters are also considered in the string.
		// If we now appended 'C' onto the string, it would become
		// [ 'A', 'B', '\0', 'C', '\0' ] - and you can clearly see the problem.
		//   ---  ---  ----  ---         : where (again) the underlined characters are counted.

		return temp;
	}

	std::wstring ToUTF16(const String& string)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(string.c_str()), (int)string.size(), nullptr, 0);
		std::wstring temp(size, 0);

		MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &temp[0], (int)temp.capacity());

		return temp;
	}
}

#endif
