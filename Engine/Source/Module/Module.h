// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class Module : public Object
	{
	public:
		virtual ~Module() {}

		virtual void RegisterObjects() {}

		virtual void Start() = 0;

		REGISTER_CLASS_FULL(Module, Object, true);

	protected:
		using Super = Module;
	};
}
