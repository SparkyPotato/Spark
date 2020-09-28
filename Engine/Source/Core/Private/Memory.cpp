#include "Core/Memory/Memory.h"

namespace Spark
{
	void Memory::Init()
	{
		if (s_Memory) return;

		// We use malloc and free here because new relies on the memory manager, which cannot itself rely on new!
		auto object = malloc(sizeof(Memory));
		memset(object, 0, sizeof(Memory));

		s_Memory = (Memory*) object;
	}

	void Memory::Shutdown()
	{
		if (!s_Memory) return;

		free(s_Memory);
		s_Memory = nullptr;
	}

	void* Memory::AllocSize(size_t size)
	{
		auto pointer = (size_t*) malloc(size + sizeof(size_t));
		pointer[0] = size;

		if (s_Memory)
		{
			s_Memory->m_MemoryAllocatedBytes += size;
			s_Memory->m_AllocationCount++;
		}

		return &pointer[1];
	}

	void Memory::Dealloc(void* pointer)
	{
		if (pointer)
		{
			auto ptr = (size_t*) pointer;

			if (s_Memory)
			{
				s_Memory->m_MemoryAllocatedBytes -= ptr[-1];
				s_Memory->m_DeallocationCount++;
			}

			free(--ptr);
		}
	}

	Memory* Memory::s_Memory = nullptr;
}

#ifdef IS_DEBUG
// Count all allocations and deallocations if we are in debug mode
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
#endif
