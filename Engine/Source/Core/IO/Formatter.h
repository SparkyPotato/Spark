// Copyright 2020 SparkyPotato

#pragma once
#include <stdio.h>

namespace Spark
{
namespace IO
{
	template<typename T>
	struct Formatter
	{
	public:
		Formatter()
		{
			
		}

		void Parse(const String& string, StringIterator parseBegin, StringIterator parseEnd) {}

		void Format(const T& value, String& outputString) {}
	};

	void FormatIntoString(String& formatInto, const String& formatString, StringIterator& formatStart);

	template<typename T, typename ...Args>
	void FormatIntoString(String& formatInto, const String& formatString, StringIterator& formatStart, const T& arg, const Args&... args)
	{
		while (formatStart != formatString.end())
		{
			if (*formatStart == L'{' && *(++formatStart) != L'{')
			{
				while (!(*formatStart == L':' || *formatStart == L'}')) { formatStart++; }
				if (*formatStart == L':') { formatStart++; }
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
		StringIterator begin = temp.begin();
		FormatIntoString(temp, formatString, begin, args...);

		return temp;
	}

	template<typename ...Args>
	void Print(const String& formatString, const Args& ...args)
	{
		String temp = FormatString(formatString, args...);
		Platform::Print(temp);
	}

	template<>
	struct Formatter<String>
	{
		void Parse(const String& string, StringIterator parseBegin, StringIterator parseEnd) {}

		void Format(const String& value, String& outputString)
		{
			outputString += value;
		}
	};

	template<>
	struct Formatter<uint64>
	{
		void Parse(const String& string, StringIterator parseBegin, StringIterator parseEnd)
		{
			auto start = parseEnd - 1;
			auto end = parseBegin - 1;

			uint multiplier = 1;
			while (start != end)
			{
				minDigits += multiplier * ((*start) - L'0');
				multiplier *= 10;

				start--;
			}
		}

		void Format(uint64 value, String& outputString)
		{
			if (value == 0) { outputString += L'0'; return; }

			Char temp[22];

			uint i = 0;
			while (value != 0)
			{
				uint mod = value % 10;
				value /= 10;
				temp[i] = mod + L'0';
				i++;
			}

			i--;
			if (minDigits == 0) { minDigits = i; }
			else { minDigits--; }

			while (minDigits != -1)
			{
				if (minDigits == i)
				{
					outputString += temp[i];
					i--;
					minDigits--;
				}
				else
				{
					outputString += L'0';
					minDigits--;
				}
			}
		}

	private:
		uint minDigits = 0;
	};

	template<>
	struct Formatter<uint8> : Formatter<uint64>
	{
		void Format(uint8 value, String& outputString)
		{
			Formatter<uint64>::Format(static_cast<uint64>(value), outputString);
		}
	};

	template<>
	struct Formatter<uint16> : Formatter<uint64>
	{
		void Format(uint16 value, String& outputString)
		{
			Formatter<uint64>::Format(static_cast<uint64>(value), outputString);
		}
	};

	template<>
	struct Formatter<uint32> : Formatter<uint64>
	{
		void Format(uint32 value, String& outputString)
		{
			Formatter<uint64>::Format(static_cast<uint64>(value), outputString);
		}
	};

	template<>
	struct Formatter<int64>
	{
		void Parse(const String& string, StringIterator parseBegin, StringIterator parseEnd)
		{
			auto start = parseEnd - 1;
			auto end = parseBegin - 1;

			uint multiplier = 1;
			while (start != end)
			{
				minDigits += multiplier * ((*start) - L'0');

				start--;
			}
		}

		void Format(int64 value, String& outputString)
		{
			if (value == 0) { outputString += L'0'; return; }
			if (value < 0)
			{
				outputString += L'-';
			}

			Char temp[22];

			uint i = 0;
			while (value != 0)
			{
				uint mod = value % 10;
				value /= 10;
				temp[i] = mod + L'0';
				i++;
			}

			i--;
			if (minDigits == 0) { minDigits = i; }
			else { minDigits--; }

			while (minDigits != -1)
			{
				if (minDigits == i)
				{
					outputString += temp[i];
					i--;
					minDigits--;
				}
				else
				{
					outputString += L'0';
					minDigits--;
				}
			}
		}

	private:
		uint minDigits = 0;
	};

	template<>
	struct Formatter<int8> : Formatter<int64>
	{
		void Format(int8 value, String& outputString)
		{
			Formatter<int64>::Format(static_cast<int64>(value), outputString);
		}
	};

	template<>
	struct Formatter<int16> : Formatter<int64>
	{
		void Format(int16 value, String& outputString)
		{
			Formatter<int64>::Format(static_cast<int64>(value), outputString);
		}
	};

	template<>
	struct Formatter<int32> : Formatter<int64>
	{
		void Format(int32 value, String& outputString)
		{
			Formatter<int64>::Format(static_cast<int64>(value), outputString);
		}
	};
}
}
