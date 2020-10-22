// Copyright 2020 SparkyPotato

#include "Core/IO/IO.h"

namespace Spark
{
namespace IO
{
	void FormatIntoString(String& formatInto, const String& formatString, String::Iterator& formatStart)
	{
		while (formatStart != formatString.end())
		{
			formatInto += *formatStart;
			formatStart++;
		}
	}

	File CreateFile(const String& file)
	{
		File temp;

		_wfopen_s(&temp, file.GetCharPointer(), STRING("w+, ccs=UNICODE"));

		return temp;
	}


	File OpenFile(const String& file)
	{
		File temp;

		_wfopen_s(&temp, file.GetCharPointer(), STRING("r+, ccs=UNICODE"));

		return temp;
	}

	void CloseFile(File file)
	{
		fclose(file);
	}
}
}
