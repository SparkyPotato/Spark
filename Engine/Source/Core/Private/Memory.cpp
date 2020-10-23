// Copyright 2020 SparkyPotato

#include "Core/Memory/Memory.h"
#include "Core/Memory/Allocator.h"

#include <stdlib.h>
#include <string.h>

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogMemory, Verbose);

	Memory* Memory::s_Memory = nullptr;

	Memory::Memory()
	{
		
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
#endif
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

	void* Memory::Alloc(size_t size, const char* file, int line)
	{
		auto pointer = malloc(size);

#ifdef IS_DEBUG
		if (s_Memory)
		{
			s_Memory->m_Allocations.Emplace(pointer, file, line, size);

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
			uint i = s_Memory->m_Allocations.Find(pointer);

			if (i != -1) { s_Memory->m_Allocations.Erase(i); }
			else { free(pointer); }
		}
#else
		free(pointer);
#endif
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

	Array<Char*, RawAllocator> StringAllocator::m_Allocations;
	Char* StringAllocator::m_StringBuffer;
	size_t StringAllocator::m_Size;
}

// Route allocations to the memory manager
void* operator new(size_t size, const char* file, int line)
{
	return Spark::Memory::Alloc(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
	return Spark::Memory::Alloc(size, file, line);
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
	return Spark::Memory::Alloc(size, "Illegal new", 0);
}

void* operator new[](size_t size)
{
	return Spark::Memory::Alloc(size, "Illegal new", 0);
}

void operator delete(void* pointer, const char* file, int line)
{
	Spark::Memory::Dealloc(pointer);
}

void operator delete[](void* pointer, const char* file, int line)
{
	Spark::Memory::Dealloc(pointer);
}
