// Copyright 2020 SparkyPotato

#include "WWindowManager.h"

namespace Spark
{
	ObjPtr<IWindowManager> IWindowManager::Instantiate()
	{
		return Create<IWindowManager, WWindowManager>();
	}

	void WWindowManager::Start()
	{
		Super::Start();

		WNDCLASSEX windowClass;
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

		windowClass.style = CS_DBLCLKS;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(nullptr);
		windowClass.hIcon = nullptr;
		windowClass.hCursor = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = STRING("Spark Default Window Class");

		RegisterClassEx(&windowClass);
	}
}
