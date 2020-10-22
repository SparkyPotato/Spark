// Copyright 2020 SparkyPotato

#pragma once
#include <stdio.h>

namespace Spark
{
namespace IO
{
	template<typename T>
	class Formatter
	{
	public:
		Formatter()
		{
			static_assert(false, "Need to specialize formatter for type T!");
		}

		void Parse(const String& string, String::Iterator parseBegin, String::Iterator parseEnd) {}

		void Format(const T& value, String& outputString) {}
	};

	void FormatIntoString(String& formatInto, const String& formatString, String::Iterator& formatStart);

	template<typename T, typename ...Args>
	void FormatIntoString(String& formatInto, const String& formatString, String::Iterator& formatStart, const T& arg, Args ...args)
	{
		while (formatStart != formatString.end())
		{
			if (*formatStart == L'{')
			{
				formatStart++;
				auto start = formatStart;
				while (*formatStart != L'}') { formatStart++; }

				Formatter<T> formatter;
				formatter.Parse(formatString, start, formatStart);
				formatter.Format(arg, formatInto);

				formatStart++;
				FormatIntoString(formatInto, formatString, formatStart, args...);

				break;
			}
			else
			{
				formatInto += *formatStart;
			}

			formatStart++;
		}
	}

	template<typename ...Args>
	String FormatString(const String& formatString, const Args& ...args)
	{
		String temp;
		FormatIntoString(temp, formatString, formatString.begin(), args...);

		return temp;
	}

	template<typename ...Args>
	void Print(const String& formatString, Args ...args)
	{
		
	}

	template<typename ...Args>
	void Print(File file, const String& formatString, Args ...args)
	{
		
	}

	template<>
	class Formatter<String>
	{
	public:
		void Parse(const String& string, String::Iterator parseBegin, String::Iterator parseEnd) {}

		void Format(const String& value, String& outputString)
		{
			outputString += value;
		}
	};
}
}
