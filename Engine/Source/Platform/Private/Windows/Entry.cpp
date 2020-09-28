int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	using namespace Spark;

	// Initialize the memory manager
	Memory::Init();

	

	// Shut the memory manager down
	Memory::Shutdown();
}
