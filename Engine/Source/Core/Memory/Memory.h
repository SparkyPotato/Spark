#pragma once

namespace Spark
{
	struct MemoryStatistics
	{
		size_t CurrentAllocation = 0;
		size_t AllocationCount = 0;
		size_t DeallocationCount = 0;
	};

	class Memory
	{
	public:
		Memory() = delete;
		~Memory() = delete;

		static void Initialize();
		static void Shutdown();

		static void* AllocSize(size_t size);
		static void Dealloc(void* pointer);

		static const MemoryStatistics& GetStats();

	private:
		static Memory* s_Memory;

		MemoryStatistics m_Stats;
	};
}
