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

	void* Memory::AllocateSize(size_t size)
	{
		s_Memory->m_TotalMemoryAllocation += size;
		s_Memory->m_TotalAllocations++;

		return malloc(size);
	}

	void Memory::DeallocSize(void* pointer, size_t size)
	{
		s_Memory->m_TotalMemoryAllocation -= size;
		s_Memory->m_TotalDeallocations++;

		free(pointer);
	}

	Memory* Memory::s_Memory = nullptr;
}

#ifdef IS_DEBUG
// Count all allocations and deallocations if we are in debug mode
void* operator new(size_t size)
{
	return Spark::Memory::AllocateSize(size);
}

void* operator new[](size_t size)
{
	return Spark::Memory::AllocateSize(size);
}

void operator delete(void* pointer, size_t size)
{
	return Spark::Memory::DeallocSize(pointer, size);
}

void operator delete[](void* pointer, size_t size)
{
	return Spark::Memory::DeallocSize(pointer, size);
}
#endif
