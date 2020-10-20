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
			SPARK_LOG(LogPlatform, Error, STRING("Failed to create mutex object! Error: %d"), GetLastError());
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
			SPARK_LOG(LogPlatform, Error, STRING("Failed to lock mutex object! Error: %d"), ret);
		}
	}

	void WMutex::Unlock()
	{
		bool ret = ReleaseMutex(m_Mutex);

		if (ret == false)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to unlock mutex object! Error: %d"), GetLastError());
		}
	}
}
