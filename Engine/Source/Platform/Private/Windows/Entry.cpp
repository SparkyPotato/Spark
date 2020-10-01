int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager and logger
	Memory::Init();
	Logger::Init();
	
	int a = 500;

	DEFINE_LOG_CATEGORY_FILE(Log, Trace);
	SPARK_LOG(Log, Warning, STRING("This is me %d"), a);

	// Shut the logger and memory manager down
	Logger::Shutdown();
	Memory::Shutdown();
}
