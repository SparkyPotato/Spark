// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class Module : public Object
	{
	public:
		virtual ~Module() {}

		virtual void RegisterClasses() {}

		virtual void Start() {}
		virtual void MainThreadTick(float mainThreadDeltaTime) {}

		REGISTER_CLASS_NORM(Module, Object, true);

	protected:
		using Super = Module;
	};
}
