// Copyright 2020 SparkyPotato

#include "Module/ModuleManager.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY(LogModuleManager);

	ModuleManager* GModuleManager = nullptr;

	extern void AddEngineModules();

	ModuleManager::ModuleManager()
	{
		
	}

	ModuleManager::~ModuleManager()
	{
		
	}

	void ModuleManager::Initialize()
	{
		if (GModuleManager)
		{
			SPARK_LOG(LogModuleManager, Error, STRING("Module Manager already initialized!"));
			return;
		}

		GModuleManager = new ModuleManager;

		SPARK_LOG(LogModuleManager, Trace, STRING("Initialized Module Manager"));

		AddEngineModules();
	}

	void ModuleManager::Shutdown()
	{
		delete GModuleManager;
		GModuleManager = nullptr;

		SPARK_LOG(LogModuleManager, Trace, STRING("Shutdown Module Manager"));
	}
}
