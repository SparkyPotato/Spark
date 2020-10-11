#include "Core/Memory/Memory.h"

namespace Spark
{
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

		free(s_Memory);
		s_Memory = nullptr;
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
