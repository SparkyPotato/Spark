#include "Core/Memory/Memory.h"
#include "Core/Types/Container.h"

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

		while (m_SharedRefs.Size() != 0)
		{
			SPARK_LOG(LogMemory, Warning, STRING("Deleting object with %d references"), m_SharedRefs[0]->RefCount);

			delete m_SharedRefs[0]->AllocatedObject;
			delete m_SharedRefs[0];
		}
	}

	void Memory::Initialize()
	{
		GMemory = new Memory();
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
		auto pointer = reinterpret_cast<size_t*>(malloc(size + sizeof(size_t)));
		pointer[0] = size;

		#ifdef IS_DEBUG
		if (GMemory)
		{
			GMemory->m_Stats.CurrentAllocation += size;
			GMemory->m_Stats.AllocationCount++;
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
			if (GMemory)
			{
				GMemory->m_Stats.CurrentAllocation -= ptr[-1];
				GMemory->m_Stats.DeallocationCount++;
			}
			#endif

			free(--ptr);
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
