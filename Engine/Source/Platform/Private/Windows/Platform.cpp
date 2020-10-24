// Copyright 2020 SparkyPotato

#include "Platform/Platform.h"

#include "WindowManager/WWindowManager.h"

namespace Spark
{
namespace Platform
{
	void RegisterClasses()
	{
		AddClass<WWindowManager>();
	}

	DateTime GetSystemTime()
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		DateTime dateTime;
		dateTime.Year = time.wYear;
		dateTime.Month = time.wMonth;
		dateTime.Day = time.wDay;
		dateTime.Hour = time.wHour;
		dateTime.Minute = time.wMinute;
		dateTime.Second = time.wSecond;
		dateTime.Millisecond = time.wMilliseconds;

		return dateTime;
	}

	void DebugOutput(const String& string) 
	{
		OutputDebugStringW(string.GetCharPointer());
		OutputDebugStringW(STRING("\n"));
	}

	void ShowMessageBox(const String& title, const String& message)
	{
		MessageBoxW(nullptr, message.GetCharPointer(), title.GetCharPointer(), MB_OK);
	}

	Char* ToUnicode(const char* ascii)
	{
		auto size = MultiByteToWideChar(CP_UTF8, 0, ascii, -1, nullptr, 0);

		auto temp = StringAllocator::Allocate(size);
		MultiByteToWideChar(CP_UTF8, 0, ascii, -1, temp, size);

		return temp;
	}
}
}
