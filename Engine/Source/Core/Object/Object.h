#pragma once

namespace Spark
{
	struct Class;

	/*
		Base class of all Engine Objects.
		Must be derived from and use the macro REGISTER_CLASS(name, parent) with the name of the class
		and its parent.
	*/
	class Object
	{
	public:
		virtual const Class& GetClass();
		static const Class& GetStaticClass();

	protected:
		using Super = Object;
	};
}
