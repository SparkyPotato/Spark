// Copyright 2020 SparkyPotato

#pragma once
#include "WindowManager/IWindowManager.h"

namespace Spark
{
	class WWindowManager : public IWindowManager
	{
	public:
		void Start() override;

		REGISTER_CLASS_NORM(WWindowManager, IWindowManager, false);
	};
}
