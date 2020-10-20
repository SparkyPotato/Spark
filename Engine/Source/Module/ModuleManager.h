// Copyright 2020 SparkyPotato

#pragma once

#include "Module/Module.h"

namespace Spark
{
	DECLARE_LOG_CATEGORY(LogModuleManager, Verbose);

	class ModuleManager
	{
	public:
		ModuleManager();
		~ModuleManager();

		static void Initialize();
		static void Shutdown();

		void MainThreadTick();

		template<class T>
		void AddModule()
		{
			for (auto& module : m_Modules)
			{
				if (Class::IsSubclass(T::GetClass(), module->GetClass()))
				{
					SPARK_LOG(LogModuleManager, Error, STRING("Module '%s' already exists (or another variant of it does)!"), T::GetClass().Name.GetCharPointer());
				}
			}

			if (!T::GetClass().IsSubclassOf<Module>())
			{
				SPARK_LOG(LogModuleManager, Error, STRING("'%s' does not inherit from module!"), T::GetClass().Name.GetCharPointer());
				return;
			}

			if (T::GetClass().IsAbstract && !T::Instantiate())
			{
				SPARK_LOG(LogModuleManager, Error, STRING("Cannot instantiate abstract module '%s'!"), T::GetClass().Name.GetCharPointer());
				return;
			}

			SPARK_LOG(LogModuleManager, Verbose, STRING("Adding module '%s'"), T::GetClass().Name.GetCharPointer());

			auto module = m_Modules.Add(UnsafeCast<Module>(T::Instantiate()));
			SPARK_LOG(LogModuleManager, Verbose, STRING("Registering objects"));
			module->RegisterClasses();
			SPARK_LOG(LogModuleManager, Verbose, STRING("Starting"));
			module->Start();
			SPARK_LOG(LogModuleManager, Verbose, STRING("Added module '%s'"), T::GetClass().Name.GetCharPointer());
		}

	private:
		Array<ObjPtr<Module>> m_Modules;
	};

	extern ModuleManager* GModuleManager;

	template<class T>
	void AddModule()
	{
		GModuleManager->AddModule<T>();
	}
}
