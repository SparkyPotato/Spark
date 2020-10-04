DEFINE_LOG_CATEGORY_FILE(LogEntry, Verbose);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager and put everything in a scope so that everything is deleted before the manager is shut down
	Memory::Init(); 
	{
		Logger::Init();
		SPARK_LOG(LogEntry, Verbose, STRING("Logger initialized"));
		
		Logger::Shutdown();
	}
	// Shut the memory manager down
	Memory::Shutdown();
}
