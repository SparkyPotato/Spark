#include "Module/ModuleManager.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY(LogModuleManager);

	ModuleManager* GModuleManager = nullptr;

	ModuleManager::ModuleManager()
	{

	}

	ModuleManager::~ModuleManager()
	{
		while (m_Modules.Size() != 0)
		{
			delete m_Modules[0];
			m_Modules.Erase(0);
		}
	}

	void ModuleManager::Initialize()
	{
		if (GModuleManager)
		{
			SPARK_LOG(LogModuleManager, Error, STRING("Module Manager already initialized!"));
			return;
		}

		GModuleManager = new ModuleManager;
	}

	void ModuleManager::Shutdown()
	{
		delete GModuleManager;
		GModuleManager = nullptr;
	}

}
