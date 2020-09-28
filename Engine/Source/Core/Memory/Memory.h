#pragma once

namespace Spark
{
	class Memory
	{
	public:
		// Memory manager should not be constructed with new, since new relies on it to keep track of memory allocation and deallocation
		// new and delete do check if the manager exists, but this is faster
		Memory() = delete;
		~Memory() = delete;

		static void Init();
		static void Shutdown();

		static void* AllocSize(size_t size);
		static void Dealloc(void* pointer);

	private:
		static Memory* s_Memory;

		size_t m_MemoryAllocatedBytes;
		size_t m_AllocationCount;
		size_t m_DeallocationCount;
	};
}
