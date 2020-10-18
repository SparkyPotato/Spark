#include "Core/Assert.h"

#include "Core/Types/String.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogAssert, Trace)

	void Assert(String message, int line, String file)
	{
		String string = Formatter::Format(STRING("%s\n\nFile: %s\nLine: %d"), message.GetCharPointer(), file.GetCharPointer(), line);

		Platform::ShowMessageBox(STRING("Assertion failed"), string);

		SPARK_LOG(LogAssert, Fatal, STRING("%s - Failed assertion at %s : %d"), message.GetCharPointer(), file.GetCharPointer(), line);
	}
}
