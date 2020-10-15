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

			if (!T::GetStaticClass().IsSubclassOf<Module>())
			{
				SPARK_LOG(LogModuleManager, Error, STRING("'%s' does not inherit from module!"), T::GetStaticClass().Name.GetCharPointer());
				return;
			}

			if (T::GetStaticClass().IsAbstract)
			{
				SPARK_LOG(LogModuleManager, Error, STRING("Cannot instantiate abstract module '%s'!"), T::GetStaticClass().Name.GetCharPointer());
				return;
			}

			auto module = m_Modules.Add(Create<Module, T>());
			module->RegisterObjects();
		}

	private:
		Array<ObjPtr<Module>> m_Modules;
	};

	extern ModuleManager* GModuleManager;
}
