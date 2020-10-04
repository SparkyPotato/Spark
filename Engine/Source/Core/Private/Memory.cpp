#include "Core/Memory/Memory.h"

namespace Spark
{
	Memory* Memory::s_Memory = nullptr;

	void Memory::Init()
	{
		if (s_Memory) return;

		// We use malloc and free here because new relies on the memory manager, which cannot itself rely on new!
		auto object = malloc(sizeof(Memory));
		memset(object, 0, sizeof(Memory));

		s_Memory = reinterpret_cast<Memory*>(object);
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
			s_Memory->m_MemoryAllocatedBytes += size;
			s_Memory->m_AllocationCount++;
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
				s_Memory->m_MemoryAllocatedBytes -= ptr[-1];
				s_Memory->m_DeallocationCount++;
			}
			#endif

			free(--ptr);
		}
	}

	void Memory::AllocString(size_t size)
	{
		#ifdef IS_DEBUG
		if (s_Memory) s_Memory->m_StringAllocation += size;
		#endif
	}

	void Memory::DeallocString(size_t size)
	{
		#ifdef IS_DEBUG
		if (s_Memory) s_Memory->m_StringAllocation -= size;
		#endif
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
