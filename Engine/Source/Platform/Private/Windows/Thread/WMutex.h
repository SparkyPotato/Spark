// Copyright 2020 SparkyPotato

#pragma once

#include "Core/Thread/Mutex.h"

namespace Spark
{
	class WMutex : public IMutex 
	{
	public:
		WMutex();
		~WMutex();

		void Lock() override;
		void Unlock() override;

	private:
		HANDLE m_Mutex;

		REGISTER_CLASS(WMutex, IMutex);
	};
}
