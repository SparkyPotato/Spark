// Copyright 2020 SparkyPotato

#include "SparkPCH.h"

#include "Module/ModuleManager.h"

#include "WindowManager/WindowManager.h"

namespace Spark
{
	void AddEngineModules()
	{
		SPARK_LOG(LogModuleManager, Trace, STRING("Adding all static Engine Modules"));

		GModuleManager->AddModule<WindowManager>();

		SPARK_LOG(LogModuleManager, Trace, STRING("Added all Engine Modules"));
	}
}
