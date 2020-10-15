#include "SparkPCH.h"
#include "Module/ModuleManager.h"

#include "WindowManager/WindowManager.h"

namespace Spark
{
	void AddEngineModules()
	{
		GModuleManager->AddModule<WindowManager>();
	}
}
