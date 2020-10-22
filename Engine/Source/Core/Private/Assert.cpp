// Copyright 2020 SparkyPotato

#include "Core/Assert.h"

#include "Core/Types/String.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogAssert, Trace)

	void Assert(String message, int line, String file)
	{
		String string = IO::FormatString(STRING("{}\n\nFile: {}\nLine: {}"), message, file, line);

		Platform::ShowMessageBox(STRING("Assertion failed"), string);

		SPARK_LOG(LogAssert, Fatal, STRING("{} - Failed assertion at {} : {}"), message, file, line);
	}
}
