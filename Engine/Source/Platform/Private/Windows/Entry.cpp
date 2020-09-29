int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager and logger
	Memory::Init();
	Logger::Init();
	
	

	// Shut the logger and memory manager down
	Logger::Shutdown();
	Memory::Shutdown();
}
