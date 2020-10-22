// Copyright 2020 SparkyPotato

#include "Core/Memory/Memory.h"

#include <stdlib.h>
#include <string.h>

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogMemory, Verbose);

	Memory* Memory::s_Memory = nullptr;

	Memory::Memory()
	{
		// We don't use MemAlloc for the same reason we don't use Array<>
		m_Allocations = reinterpret_cast<Allocation*>(malloc(sizeof(Allocation) * m_AllocationSize));
	}

	Memory::~Memory()
	{
		if (m_SharedRefs.Size() > 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("{} object/s not deleted!"), m_SharedRefs.Size());
		}

		for (auto object : m_SharedRefs)
		{
			object->AllocatedObject->~Object();
			SPARK_LOG(LogMemory, Verbose, STRING("Deleting object with {} references"), object->RefCount);
		}

#ifdef IS_DEBUG
		auto stats = m_Stats;
		if (stats.CurrentAllocation > 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("Memory leak detected!"));
			SPARK_LOG(LogMemory, Warning, STRING("Heap allocation on shutdown: {} bytes"), stats.CurrentAllocation);
			SPARK_LOG(LogMemory, Warning, STRING("Total allocations: {}"), stats.AllocationCount);
			SPARK_LOG(LogMemory, Warning, STRING("Total deallocations: {}"), stats.DeallocationCount);
		}
		else
		{
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap allocations: {}"), stats.AllocationCount);
			SPARK_LOG(LogMemory, Verbose, STRING("Total heap deallocations: {}"), stats.DeallocationCount);
		}

		for (uint i = 0; i < m_AllocationHead; i++)
		{
			SPARK_LOG(LogMemory, Verbose, STRING("Freeing block with size {} bytes"), m_Allocations[i].Size);
			free(m_Allocations[i].Pointer);
		}
#endif

		free(m_Allocations);
	}

	void Memory::Initialize()
	{
		/*
			Instead of just declaring s_Memory = new Memory,
			we do this because we don't want heap allocations by static objects to be counted,
			as the memory leak detection is done before static objects are destroyed.
			We would then have memory leaks all the time, which could not be removed - not very good design!
		*/

		if (s_Memory)
		{
			SPARK_LOG(LogMemory, Error, STRING("Memory manager already initialized!"));
			return;
		}

		s_Memory = snew Memory;
		SPARK_LOG(LogMemory, Trace, STRING("Memory manager initialized"));
	}

	void Memory::Shutdown()
	{
		SPARK_LOG(LogMemory, Trace, STRING("Shutting down Memory manager"));

		sdelete s_Memory;
		s_Memory = nullptr;

		SPARK_LOG(LogMemory, Trace, STRING("Memory manager shutdown"));
	}

	void* Memory::AllocSize(size_t size, const char* file, int line)
	{
		auto pointer = malloc(size);

#ifdef IS_DEBUG
		if (s_Memory)
		{
			if (s_Memory->m_AllocationHead >= s_Memory->m_AllocationSize)
			{
				s_Memory->m_AllocationSize *= 2;
				Allocation* temp = s_Memory->m_Allocations;

				s_Memory->m_Allocations = reinterpret_cast<Allocation*>(malloc(sizeof(Allocation) * s_Memory->m_AllocationSize));
				MemCopy(s_Memory->m_Allocations, temp, sizeof(Allocation) * s_Memory->m_AllocationHead);

				free(temp);
			}

			new(s_Memory->m_Allocations + s_Memory->m_AllocationHead) Allocation(pointer, file, line, size);
			s_Memory->m_AllocationHead++;

			s_Memory->m_Stats.AllocationCount++;
			s_Memory->m_Stats.CurrentAllocation += size;
		}
#endif

		return pointer;
	}

	void Memory::Dealloc(void* pointer)
	{
#ifdef IS_DEBUG
		if (s_Memory)
		{
			uint i = 0;
			bool found = false;

			for (; i < s_Memory->m_AllocationHead; i++)
			{
				if (s_Memory->m_Allocations[i] == pointer) { found = true; break; }
			}

			if (found)
			{
				s_Memory->m_Stats.CurrentAllocation -= s_Memory->m_Allocations[i].Size;
				s_Memory->m_Stats.DeallocationCount++;
				
				for (; i < s_Memory->m_AllocationHead; i++)
				{
					MemCopy(s_Memory->m_Allocations + i, s_Memory->m_Allocations + i + 1, sizeof(Allocation));
				}
				s_Memory->m_AllocationHead--;
			}
		}
#endif

		free(pointer);
	}

	const MemoryStatistics& Memory::GetStats()
	{
		return s_Memory->m_Stats;
	}

	Memory::SharedRef::SharedRef()
	{
		s_Memory->m_SharedRefs.Add(this);
	}

	Memory::SharedRef::~SharedRef()
	{
		s_Memory->m_SharedRefs.Erase(s_Memory->m_SharedRefs.Find(this));
	}

	void MemCopy(void* destination, const void* source, size_t bytes)
	{
		memcpy(destination, source, bytes);
	}

	int MemCompare(const void* first, const void* second, size_t bytes)
	{
		return memcmp(first, second, bytes);
	}

}

// Route allocations to the memory manager
void* operator new(size_t size, const char* file, int line)
{
	return Spark::Memory::AllocSize(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
	return Spark::Memory::AllocSize(size, file, line);
}

void operator delete(void* pointer)
{
	Spark::Memory::Dealloc(pointer);
}

void operator delete[](void* pointer)
{
	Spark::Memory::Dealloc(pointer);
}

void* operator new(size_t size)
{
	return Spark::Memory::AllocSize(size, "Illegal new", 0);
}

void* operator new[](size_t size)
{
	return Spark::Memory::AllocSize(size, "Illegal new", 0);
}

void operator delete(void* pointer, const char* file, int line)
{
	Spark::Memory::Dealloc(pointer);
}

void operator delete[](void* pointer, const char* file, int line)
{
	Spark::Memory::Dealloc(pointer);
}
