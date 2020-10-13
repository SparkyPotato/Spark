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
	};

	extern RunLoop GRunLoop;
}
