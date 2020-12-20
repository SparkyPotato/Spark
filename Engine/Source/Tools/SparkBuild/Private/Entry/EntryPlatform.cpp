// Copyright 2020 SparkyPotato

#include "Global/Global.h"

int Entry(std::vector<std::string>& args);

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

std::string ToUTF8(const wchar_t* utf16)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, nullptr, 0, nullptr, nullptr);
	std::string temp(size, 0);

	WideCharToMultiByte(CP_UTF8, 0, utf16, -1, temp.data(), size, nullptr, nullptr);
	temp.pop_back(); // Fixes a weird issue where we have double nulls

	return temp;
}

std::wstring ToUTF16(const std::string& string)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(string.c_str()), (int)string.size(), nullptr, 0);
	std::wstring temp(size, 0);

	MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &temp[0], (int)temp.capacity());

	return temp;
}

int wmain(int argc, wchar_t** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		args.emplace_back(ToUTF8(argv[i]));
	}
	
	return Entry(args);
}

#else

int main(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		args.emplace_back(argv[i]);
	}
	
	return Entry(args);
}

#endif
