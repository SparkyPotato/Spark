#pragma once

namespace Spark
{
	class Module : public Object
	{
	public:
		virtual void RegisterObjects() = 0;

		REGISTER_CLASS_FULL(Module, Object, true);

	protected:
		using Super = Module;
	};
}
