// Copyright 2020 SparkyPotato

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogPlatform, Verbose);

	Mutex::Mutex()
	{
		m_Handle = CreateMutex(nullptr, false, nullptr);

		if (m_Handle == nullptr)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to create mutex object! Error: {}"), static_cast<uint32>(GetLastError()));
		}
	}

	Mutex::~Mutex()
	{
		CloseHandle(m_Handle);
	}

	void Mutex::Lock()
	{
		DWORD ret = WaitForSingleObject(m_Handle, INFINITE);

		if (ret != WAIT_OBJECT_0 && ret != WAIT_ABANDONED)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to lock mutex object! Error: {}"), static_cast<uint32>(ret));
		}
	}

	void Mutex::Unlock()
	{
		bool ret = ReleaseMutex(m_Handle);

		if (ret == false)
		{
			SPARK_LOG(LogPlatform, Error, STRING("Failed to unlock mutex object! Error: {}"), static_cast<uint32>(GetLastError()));
		}
	}
}
