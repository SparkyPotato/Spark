int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager and put everything in a scope so that everything is deleted before the manager is shut down
	Memory::Init(); 
	{
		Logger::Init();

		// Shut the logger and memory manager down
		Logger::Shutdown();
	} 
	Memory::Shutdown();
}
