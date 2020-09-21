#pragma once

#ifndef SPARK_MEMORY
#define SPARK_MEMORY

namespace Spark
{
	class Memory
	{
	public:
		// Memory manager should not be constructed with new, since new relies on it to keep track of memory allocation and deallocation
		Memory() = delete;
		~Memory() = delete;

		static void Init();
		static void Shutdown();

		static void* AllocateSize(size_t size);
		static void DeallocSize(void* pointer, size_t size);

	private:
		static Memory* s_Memory;

		size_t m_TotalMemoryAllocation;
		size_t m_TotalAllocations;
		size_t m_TotalDeallocations;
	};
}

#endif
