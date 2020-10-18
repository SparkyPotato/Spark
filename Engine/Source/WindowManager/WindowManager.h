// Copyright 2020 SparkyPotato

#pragma once
#include "Module/Module.h"

namespace Spark
{
	DECLARE_LOG_CATEGORY(LogWindowManager, Verbose);

	class WindowManager : public Module 
	{
	public:
		void Start() override;

		void RegisterObjects() override;

		REGISTER_CLASS(WindowManager, Module);
	};
}
