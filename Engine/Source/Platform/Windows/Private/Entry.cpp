#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	// Initialize the memory manager
	Spark::Memory::Init();

	Spark::Memory::Shutdown();
}
