// Copyright 2020 SparkyPotato

#include "Runtime/RunLoop/RunLoop.h"

#include "Module/ModuleManager.h"

namespace Spark
{
	RunLoop GRunLoop;

	DEFINE_LOG_CATEGORY_FILE(LogRunLoop, Verbose);

	RunLoop::RunLoop()
		: m_IsRunning(true)
	{

	}

	void RunLoop::Start()
	{
		if (!m_IsRunning)
		{
			SPARK_LOG(LogRunLoop, Error, STRING("Run Loop was quit before starting"));
			return;
		}

		ClassManager::Initialize();
		ModuleManager::Initialize();

		SPARK_LOG(LogRunLoop, Trace, STRING("Started Run Loop"));

		while (!m_IsRunning)
		{
			
		}

		ModuleManager::Shutdown();
		ClassManager::Shutdown();

		SPARK_LOG(LogRunLoop, Trace, STRING("Ended Run Loop"));
	}

	void RunLoop::ForceQuit()
	{
		SPARK_LOG(LogRunLoop, Warning, STRING("Force Quit, Cleaning up"));

		ClassManager::Shutdown();

		Memory::Shutdown();

		SPARK_LOG(LogRunLoop, Trace, STRING("Exiting..."));

		exit(1);
	}
}
