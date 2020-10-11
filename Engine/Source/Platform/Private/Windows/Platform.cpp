#include "Platform/Platform.h"

namespace Spark
{
namespace Platform
{
	DateTime GetSystemTime() noexcept
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

	void DebugOutput(const String& string) noexcept
	{
		OutputDebugStringW(string.GetCharPointer());
	}
}
}
