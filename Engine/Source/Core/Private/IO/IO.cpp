// Copyright 2020 SparkyPotato

#include "Core/IO/IO.h"

namespace Spark
{
namespace IO
{
	void FormatIntoString(String& formatInto, const String& formatString, StringIterator& formatStart)
	{
		while (formatStart != formatString.end())
		{
			formatInto += *formatStart;
			formatStart++;
		}
	}
}
}
