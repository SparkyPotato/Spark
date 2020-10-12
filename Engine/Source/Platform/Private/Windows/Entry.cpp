#include <DirectXMath.h>

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
	Memory::Shutdown(); // Shut the memory manager down
	SPARK_LOG(LogEntry, Trace, STRING("Memory manager shut down"));
	SPARK_LOG(LogEntry, Trace, STRING("Exiting..."));
	Logger::Shutdown();

	return 0;
}
