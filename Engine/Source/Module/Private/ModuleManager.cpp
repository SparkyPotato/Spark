// Copyright 2020 SparkyPotato

#include "Module/ModuleManager.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY(LogModuleManager);

	ModuleManager* GModuleManager = nullptr;

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

		for (auto& module : IModule::GetClass().Children)
		{
			GModuleManager->AddModule(module);
		}

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

	ObjPtr<IModule> ModuleManager::GetModule(const Class& module)
	{
		for (auto& mod : m_Modules)
		{
			if (module.IsSubclassOf(mod->GetClass()))
			{
				return mod;
			}
		}

		SPARK_LOG(LogModuleManager, Error, STRING("Module '{}' not found!"), module.Name);
		return ObjPtr<IModule>();
	}

	void ModuleManager::AddModule(const Class& module)
	{
		if (module.IsAbstract && !module.Instantiate())
		{
			SPARK_LOG(LogModuleManager, Error, STRING("Cannot instantiate abstract module '{}'!"), module.Name);
			return;
		}

		SPARK_LOG(LogModuleManager, Verbose, STRING("Adding module '{}'"), module.Name);

		auto mod = m_Modules.Add(module.Instantiate<IModule>());
		SPARK_LOG(LogModuleManager, Verbose, STRING("Registering objects"));
		mod->RegisterClasses();
		SPARK_LOG(LogModuleManager, Verbose, STRING("Starting"));
		mod->Start();
		SPARK_LOG(LogModuleManager, Verbose, STRING("Added module '{}'"), module.Name);
	}
}
