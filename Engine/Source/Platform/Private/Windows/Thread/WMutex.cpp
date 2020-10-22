// Copyright 2020 SparkyPotato

#include "WMutex.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogPlatform, Verbose);

	ObjPtr<IMutex> IMutex::Instantiate()
	{
		return Create<IMutex, WMutex>();
	}

	WMutex::WMutex()
	{
		m_Mutex = CreateMutex(nullptr, false, nullptr);

		if (m_Mutex == nullptr)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to create mutex object! Error: {}"), static_cast<uint32>(GetLastError()));
		}
	}

	WMutex::~WMutex()
	{
		CloseHandle(m_Mutex);
	}

	void WMutex::Lock()
	{
		DWORD ret = WaitForSingleObject(m_Mutex, INFINITE);

		if (ret != WAIT_OBJECT_0 && ret != WAIT_ABANDONED)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to lock mutex object! Error: {}"), static_cast<uint32>(ret));
		}
	}

	void WMutex::Unlock()
	{
		bool ret = ReleaseMutex(m_Mutex);

		if (ret == false)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to unlock mutex object! Error: {}"), static_cast<uint32>(GetLastError()));
		}
	}
}