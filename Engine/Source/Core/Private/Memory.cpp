// Copyright 2020 SparkyPotato

#include "Core/Memory/Memory.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogMemory, Verbose);

	Memory* GMemory;

	Memory::Memory()
	{
		
	}

	Memory::~Memory()
	{
		if (m_SharedRefs.Size() > 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("%d object/s not deleted!"), m_SharedRefs.Size());
		}

		for (auto object : m_SharedRefs)
		{
			object->AllocatedObject->~Object();
			SPARK_LOG(LogMemory, Verbose, STRING("Deleting object with %d references"), object->RefCount);
		}
	}

	void Memory::Initialize()
	{
		/*
			Instead of just declaring GMemory = new Memory,
			we do this because we don't want heap allocations by static objects to be counted,
			as the memory leak detection is done before static objects are destroyed.
			We would then have memory leaks all the time, which could not be removed - not very good design!
		*/

		if (GMemory)
		{
			SPARK_LOG(LogMemory, Error, STRING("Memory manager already initialized!"));
			return;
		}

		GMemory = new Memory;
		SPARK_LOG(LogMemory, Trace, STRING("Memory manager initialized"));
	}

	void Memory::Shutdown()
	{
		auto stats = GMemory->m_Stats;
		delete GMemory;
		GMemory = nullptr;

		if (stats.CurrentAllocation > 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("Memory leak detected!"));
			SPARK_LOG(LogMemory, Warning, STRING("Heap allocation on shutdown: %d bytes"), stats.CurrentAllocation);
			SPARK_LOG(LogMemory, Warning, STRING("Total allocations: %d"), stats.AllocationCount);
			SPARK_LOG(LogMemory, Warning, STRING("Total deallocations: %d"), stats.DeallocationCount);
		}
		else
		{
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap allocations: %d"), stats.AllocationCount);
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap deallocations: %d"), stats.DeallocationCount);
		}
	}

	void* Memory::AllocSize(size_t size)
	{
		// Allocate memory + 1 size_t
		auto pointer = reinterpret_cast<size_t*>(malloc(size + sizeof(size_t)));
		pointer[0] = size; // Store requested size in the first size_t

		#ifdef IS_DEBUG
		if (GMemory)
		{
			GMemory->m_Stats.CurrentAllocation += size;
			GMemory->m_Stats.AllocationCount++;
		}
		#endif

		return &pointer[1]; // Return the second size_t, just after the stored size
	}

	void Memory::Dealloc(void* pointer)
	{
		if (pointer)
		{
			auto ptr = reinterpret_cast<size_t*>(pointer); // Cast to a size_t
			ptr--; // Decrement the pointer to point to the stored size in AllocSize

			#ifdef IS_DEBUG
			if (GMemory)
			{
				GMemory->m_Stats.CurrentAllocation -= *ptr; // Retrieve the stored size
				GMemory->m_Stats.DeallocationCount++;
			}
			#endif

			free(ptr); // Free the decremented pointer
		}
	}

	const MemoryStatistics& Memory::GetStats()
	{
		return GMemory->m_Stats;
	}

	Memory::SharedRef::SharedRef()
	{
		GMemory->m_SharedRefs.Add(this);
	}

	Memory::SharedRef::~SharedRef()
	{
		GMemory->m_SharedRefs.Erase(GMemory->m_SharedRefs.Find(this));
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
