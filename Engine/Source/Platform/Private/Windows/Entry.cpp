#include "Runtime/RunLoop/RunLoop.h"

DEFINE_LOG_CATEGORY_FILE(LogEntry, Verbose);

inline void SetSignalHandler();

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	using namespace Spark;

	Logger::Initialize();
	SPARK_LOG(LogEntry, Trace, STRING("Logger initialized"));

	Memory::Initialize();

	SetSignalHandler(); // Set the signal handler for console interrupts

	SPARK_LOG(LogEntry, Trace, STRING("Starting Run Loop"));
	GRunLoop.Start();
	
	Memory::Shutdown();

	SPARK_LOG(LogEntry, Trace, STRING("Exiting..."));
	Logger::Shutdown();

	return 0;
}

bool WINAPI SignalHandler(DWORD type)
{
	switch (type)
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_C_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		SPARK_LOG(LogEntry, Fatal, STRING("Fatal interrupt!"));
		return true;
	}

	return false;
}

void SetSignalHandler()
{
	SPARK_LOG(LogEntry, Trace, STRING("Setting console signal handler"));

	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, true))
	{
		SPARK_LOG(LogEntry, Fatal, STRING("Failed to set console signal handler!"));
	}
	else
	{
		SPARK_LOG(LogEntry, Trace, STRING("Set signal handler"));
	}
}
