// Copyright 2020 SparkyPotato

#include "Module/ModuleManager.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY(LogModuleManager);

	ModuleManager* GModuleManager = nullptr;

	extern void AddEngineModules();
	extern void AddAppModules();

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

		GModuleManager = snew ModuleManager;

		AddEngineModules();
		AddAppModules();

		SPARK_LOG(LogModuleManager, Trace, STRING("Initialized Module Manager"));
	}

	void ModuleManager::Shutdown()
	{
		sdelete GModuleManager;
		GModuleManager = nullptr;

		SPARK_LOG(LogModuleManager, Trace, STRING("Shutdown Module Manager"));
	}

	void ModuleManager::MainThreadTick()
	{
		for (auto& module : m_Modules)
		{
			module->MainThreadTick(0.f);
		}
	}

#ifdef IS_EDITOR
	void AddAppModules()
	{
		// Add editor-only modules here
	}
#endif
}
