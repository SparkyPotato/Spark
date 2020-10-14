#pragma once

namespace Spark
{
	struct Class
	{
		Class() = default;

		Class(const String& name, bool isAbstract)
			: Name(name), IsAbstract(isAbstract)
		{}

		String Name;
		bool IsAbstract;
	};

	class ClassManager;

	/*
		Base class of all Engine Objects.
		Must be derived from and use the macro REGSITER_CLASS(name, parent) with the name of the class
		and its parent.
	*/
	class Object
	{
	public:
		virtual const Class* GetClass();
		static const Class* GetStaticClass();
	};
}
