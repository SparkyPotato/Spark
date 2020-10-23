// Copyright 2020 SparkyPotato

#include "SparkPCH.h"

#include "WindowManager/IWindowManager.h"

namespace Spark
{
	void AddEngineClasses()
	{
		AddClass<IModule>();
		AddClass<IWindowManager>();

		AddClass<IMutex>();
	}

#ifdef IS_EDITOR
	void AddAppClasses()
	{
		// Add editor-specific classes here
	}
#endif
}
