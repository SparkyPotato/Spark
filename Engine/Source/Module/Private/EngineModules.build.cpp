// Copyright 2020 SparkyPotato

#include "SparkPCH.h"

#include "Module/ModuleManager.h"

#include "WindowManager/IWindowManager.h"

namespace Spark
{
	void AddEngineClasses()
	{
		AddClass<Module>();
		AddClass<IWindowManager>();

		AddClass<IMutex>();
	}

	void AddEngineModules()
	{
		SPARK_LOG(LogModuleManager, Trace, STRING("Adding all static Engine Modules"));

		AddModule<IWindowManager>();

		SPARK_LOG(LogModuleManager, Trace, STRING("Added all Engine Modules"));
	}
}
