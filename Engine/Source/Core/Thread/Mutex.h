// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class IMutex : public Object
	{
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		REGISTER_CLASS_NO_INS(IMutex, Object, true);
		static ObjPtr<IMutex> Instantiate();
	};
}
