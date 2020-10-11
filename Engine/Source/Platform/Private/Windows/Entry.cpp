DEFINE_LOG_CATEGORY_FILE(LogEntry, Verbose);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	using namespace Spark;

	Logger::Init();
	SPARK_LOG(LogEntry, Trace, STRING("Logger initialized"));

	// Initialize the memory manager and put everything in a scope so that everything is deleted before the manager is shut down
	Memory::Init(); 
	SPARK_LOG(LogEntry, Trace, STRING("Memory manager initialized"));
	{



	}
	MemoryStatistics memStats = Memory::GetStats(); // Get statistics before shutdown
	Memory::Shutdown(); // Shut the memory manager down
	SPARK_LOG(LogEntry, Trace, STRING("Memory manager shut down"));

	if (memStats.CurrentAllocation > 0)
	{
		SPARK_LOG(LogEntry, Warning, STRING("Memory leak detected!"));
		SPARK_LOG(LogEntry, Warning, STRING("Heap allocation on shutdown: %d bytes"), memStats.CurrentAllocation);
		SPARK_LOG(LogEntry, Warning, STRING("Total allocations: %d"), memStats.AllocationCount);
		SPARK_LOG(LogEntry, Warning, STRING("Total deallocations: %d"), memStats.DeallocationCount);
	}
	else
	{
		SPARK_LOG(LogEntry, Verbose, STRING("Total heap allocations: %d"), memStats.AllocationCount);
		SPARK_LOG(LogEntry, Verbose, STRING("Total heap deallocations: %d"), memStats.DeallocationCount);
	}

	SPARK_LOG(LogEntry, Trace, STRING("Exiting..."));
	Logger::Shutdown();
}
