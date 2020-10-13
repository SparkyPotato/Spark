#include "Runtime/RunLoop/RunLoop.h"

namespace Spark
{
	RunLoop GRunLoop;

	DEFINE_LOG_CATEGORY_FILE(LogRunLoop, Trace);

	RunLoop::RunLoop()
		: m_IsRunning(true)
	{

	}

	void RunLoop::Start()
	{
		if (!m_IsRunning)
		{
			SPARK_LOG(LogRunLoop, Error, STRING("Run Loop was quit before starting"));
			return;
		}

		SPARK_LOG(LogRunLoop, Trace, STRING("Started Run Loop"));

		while (m_IsRunning)
		{

		}

		SPARK_LOG(LogRunLoop, Trace, STRING("Ended Run Loop"));
	}

	void RunLoop::ForceQuit()
	{
		SPARK_LOG(LogRunLoop, Warning, STRING("Force Quit, Cleaning up"));

		Memory::Shutdown(); // Shut the memory manager down
		SPARK_LOG(LogRunLoop, Trace, STRING("Memory manager shut down"));

		SPARK_LOG(LogRunLoop, Trace, STRING("Exiting..."));
		Logger::Shutdown();

		exit(1);
	}
}
