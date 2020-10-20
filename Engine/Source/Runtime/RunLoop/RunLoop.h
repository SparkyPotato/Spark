// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class RunLoop
	{
	public:
		RunLoop();

		void Start();

		void ForceQuit();

	private:
		bool m_IsRunning;

		ObjPtr<IMutex> m_RunMutex;
	};

	extern RunLoop GRunLoop;
}
