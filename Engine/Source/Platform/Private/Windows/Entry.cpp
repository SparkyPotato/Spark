// Copyright 2020 SparkyPotato

#include "Runtime/RunLoop/RunLoop.h"

DEFINE_LOG_CATEGORY_FILE(LogEntry, Verbose);

inline void SetSignalHandler();

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	using namespace Spark;

	Memory::Initialize(); // Initialize memory manager so that static heap allocations are NOT counted

	SetSignalHandler(); // Set the signal handler for console interrupts

	GRunLoop.Start(); // Start the Engine Run Loop, where everything actually happens
	
	Memory::Shutdown(); // Shutdown memory manager and report memory stats on exit

	SPARK_LOG(LogEntry, Trace, STRING("Exiting..."));

	Logger::Shutdown();

	return 0; // Yay we did not have any problems and could exit cleanly
}

bool WINAPI SignalHandler(DWORD type)
{
	switch (type)
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_C_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		// Force quit the engine with a fatal log if the application is closed in the console
		SPARK_LOG(LogEntry, Fatal, STRING("Fatal interrupt!"));
		return true;
	}

	return false;
}

void SetSignalHandler()
{
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, true))
	{
		SPARK_LOG(LogEntry, Fatal, STRING("Failed to set console signal handler!"));
	}
	else
	{
		SPARK_LOG(LogEntry, Trace, STRING("Set console signal handler"));
	}
}
