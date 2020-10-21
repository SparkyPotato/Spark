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


	private:
		void AddModule(const Class& module);

		Array<ObjPtr<Module>> m_Modules;
	};

	extern ModuleManager* GModuleManager;
}
