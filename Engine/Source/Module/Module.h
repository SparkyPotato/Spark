// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class IModule : public Object
	{
	public:
		virtual ~IModule() {}

		virtual void RegisterClasses() {}

		virtual void Start() {}
		virtual void MainThreadTick(float mainThreadDeltaTime) {}

		REGISTER_CLASS_NORM(IModule, Object, true);
	};
}
