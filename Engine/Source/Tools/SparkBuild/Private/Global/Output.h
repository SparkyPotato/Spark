// Copyright 2020 SparkyPotato

#pragma once

#include "Global/Global.h"

#ifdef PLATFORM_WINDOWS

/// Converts from a UTF-8 encoded string to a UTF-16 one.
/// Only used on Windows.
///
/// \param string The UTF-8 encoded string to convert.
/// 
/// \return The UTF-16 encoded string.
std::wstring ToUTF16(const std::string& string);

/// Outputs to the console. Terminates with a newline.
/// 
/// \param arg The string to output.
inline void Console(const std::string& arg)
{
	std::wcout << ToUTF16(arg) << L"\n";
}

/// Outputs to the console. Terminates with a newline.
/// 
/// \param arg The variable to output.
template<typename T>
void Console(const T& arg)
{
	std::wcout << arg << "\n";
}

/// Outputs to the console.
/// 
/// \param arg The string to output.
/// \param args Other arguments to output next.
template<typename... Args>
void Console(const std::string& arg, Args&&... args)
{
	std::wcout << ToUTF16(arg);
	Console(std::forward<Args>(args)...);
}

/// Outputs to the console.
/// 
/// \param arg The variable to output.
/// \param args Other arguments to output next.
template<typename T, typename... Args>
void Console(const T& arg, Args&&... args)
{
	std::wcout << arg;
	Console(std::forward<Args>(args)...);
}

#else

template<typename T>
void Console(const T& arg)
{
	std::cout << arg << "\n";
}

template<typename T, typename... Args>
void Console(const T& arg, Args&&... args)
{
	std::cout << arg;
	Console(std::forward<Args>(args)...);
}

#endif
