#pragma once

namespace Spark
{
	class Module : public Object
	{
	public:

		virtual void RegisterObjects() = 0;

		REGISTER_CLASS_ABS(Module, Object);
	};
}
