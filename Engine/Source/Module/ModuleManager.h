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
		ObjPtr<T> GetModule() { return UnsafeCast<T>(GetModule(T::GetClass())); }

		ObjPtr<IModule> GetModule(const Class& module);

	private:
		void AddModule(const Class& module);

		Array<ObjPtr<IModule>> m_Modules;
	};

	extern ModuleManager* GModuleManager;
}
