#include "Core/Memory/Memory.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogMemory, Verbose);

	Memory* Memory::s_Memory = nullptr;

	void Memory::Init()
	{
		if (s_Memory) return;

		// We use malloc and free here because new relies on the memory manager, which cannot itself rely on new!
		s_Memory = reinterpret_cast<Memory*>(malloc(sizeof(Memory)));
		memset(s_Memory, 0, sizeof(Memory));
	}

	void Memory::Shutdown()
	{
		if (!s_Memory) return;

		auto temp = s_Memory;
		s_Memory = nullptr;

		if (temp->m_Stats.CurrentAllocation > 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("Memory leak detected!"));
			SPARK_LOG(LogMemory, Warning, STRING("Heap allocation on shutdown: %d bytes"), temp->m_Stats.CurrentAllocation);
			SPARK_LOG(LogMemory, Warning, STRING("Total allocations: %d"), temp->m_Stats.AllocationCount);
			SPARK_LOG(LogMemory, Warning, STRING("Total deallocations: %d"), temp->m_Stats.DeallocationCount);
		}
		else
		{
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap allocations: %d"), temp->m_Stats.AllocationCount);
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap deallocations: %d"), temp->m_Stats.DeallocationCount);
		}

		free(temp);
	}

	void* Memory::AllocSize(size_t size)
	{
		auto pointer = reinterpret_cast<size_t*>(malloc(size + sizeof(size_t)));
		pointer[0] = size;

		#ifdef IS_DEBUG
		if (s_Memory)
		{
			s_Memory->m_Stats.CurrentAllocation += size;
			s_Memory->m_Stats.AllocationCount++;
		}
		#endif

		return &pointer[1];
	}

	void Memory::Dealloc(void* pointer)
	{
		if (pointer)
		{
			auto ptr = reinterpret_cast<size_t*>(pointer);

			#ifdef IS_DEBUG
			if (s_Memory)
			{
				s_Memory->m_Stats.CurrentAllocation -= ptr[-1];
				s_Memory->m_Stats.DeallocationCount++;
			}
			#endif

			free(--ptr);
		}
	}

	const MemoryStatistics& Memory::GetStats()
	{
		return s_Memory->m_Stats;
	}
}

// Route allocations to the memory manager
void* operator new(size_t size)
{
	return Spark::Memory::AllocSize(size);
}

void* operator new[](size_t size)
{
	return Spark::Memory::AllocSize(size);
}

void operator delete(void* pointer) noexcept
{
	Spark::Memory::Dealloc(pointer);
}

void operator delete[](void* pointer) noexcept
{
	Spark::Memory::Dealloc(pointer);
}
