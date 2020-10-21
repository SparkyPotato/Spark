// Copyright 2020 SparkyPotato

#pragma once
#include "Module/Module.h"

namespace Spark
{
	DECLARE_LOG_CATEGORY(LogWindowManager, Verbose);

	struct WindowProperties
	{
		String Name;

		struct
		{
			uint Width = 0, Height = 0;
		} Dimensions;
	};

	class IWindowManager : public Module 
	{
	public:
		void RegisterClasses() override;

		REGISTER_CLASS_NO_INS(IWindowManager, Module, true);
		static ObjPtr<IWindowManager> Instantiate();
	};
}
