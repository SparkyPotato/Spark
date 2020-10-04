int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager and put everything in a scope so that everything is deleted before the manager is shut down
	Memory::Init(); 
	{
		Logger::Init();

		DEFINE_LOG_CATEGORY_FILE(Temp, Verbose);
		for (int i = 0; i < 10000; i++)
		{
			SPARK_LOG(Temp, Log, STRING("Iteration %d"), i);
		}
		
		Logger::Shutdown();
	}
	// Shut the memory manager down
	Memory::Shutdown();
}
