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

		template<class T>
		void AddModule()
		{
			T::RegisterClass();

			if (!T::GetClass().IsSubclassOf<Module>())
			{
				SPARK_LOG(LogModuleManager, Error, STRING("'%s' does not inherit from module!"), T::GetClass().Name.GetCharPointer());
				return;
			}

			if (T::GetClass().IsAbstract)
			{
				SPARK_LOG(LogModuleManager, Error, STRING("Cannot instantiate abstract module '%s'!"), T::GetClass().Name.GetCharPointer());
				return;
			}

			SPARK_LOG(LogModuleManager, Verbose, STRING("Adding module '%s'"), T::GetClass().Name.GetCharPointer());

			auto module = m_Modules.Add(UnsafeCast<Module>(Create<T>()));
			SPARK_LOG(LogModuleManager, Verbose, STRING("Registering objects"));
			module->RegisterObjects();
			SPARK_LOG(LogModuleManager, Verbose, STRING("Starting"));
			module->Start();
			SPARK_LOG(LogModuleManager, Verbose, STRING("Added module '%s'"), T::GetClass().Name.GetCharPointer());
		}

	private:
		Array<ObjPtr<Module>> m_Modules;
	};

	extern ModuleManager* GModuleManager;
}
