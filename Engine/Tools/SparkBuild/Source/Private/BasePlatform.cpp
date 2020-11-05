/*
	SparkBuild.Private.BasePlatform
	Copyright 2020 SparkyPotato

	Platform-specific interface
*/

#include "BasePlatform.h"

/* 
	SparkBuild is a small tool with very little platform-specific functionality used.
	This is why we put all platform ifdefs in a single-file.
*/

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

		WideCharToMultiByte(CP_UTF8, 0, string, -1, temp.data(), size, nullptr, nullptr);

		return temp;
	}

	std::wstring ToUTF16(const String& string)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(string.c_str()), string.size(), nullptr, 0);
		std::wstring temp(size, 0);

		MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string.size(), temp.data(), temp.capacity());

		return temp;
	}
}

#endif
