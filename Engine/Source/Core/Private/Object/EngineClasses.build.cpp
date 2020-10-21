// Copyright 2020 SparkyPotato

#include "SparkPCH.h"

#include "WindowManager/IWindowManager.h"

namespace Spark
{
	void AddEngineClasses()
	{
		AddClass<Module>();
		AddClass<IWindowManager>();

		AddClass<IMutex>();
	}

#ifdef IS_EDITOR
	extern void AddAppClasses()
	{
		// Add editor-specific classes here
	}
#endif
}
