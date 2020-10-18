// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	struct Class;

	/*
		Base class of all Engine Objects.
		Must be derived from and use the macro REGISTER_CLASS(name, parent),
		with the name of the class and its parent.
		Spark does NOT support multiple inheritance for Objects yet
	*/
	class Object
	{
	public:
		virtual ~Object() {}

		static const Class& GetClass();

	protected:
		using Super = Object;
	};
}
