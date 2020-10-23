// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class Object;
	struct RawAllocator;

	struct MemoryStatistics
	{
		size_t CurrentAllocation = 0;
		size_t AllocationCount = 0;
		size_t DeallocationCount = 0;
	};

	class Memory
	{
	public:
		Memory();
		~Memory();

		static void Initialize();
		static void Shutdown();

		static void* Alloc(size_t size, const char* file, int line);
		static void Dealloc(void* pointer);

		static const MemoryStatistics& GetStats();

		struct SharedRef
		{
			SharedRef();

			~SharedRef();

			Object* AllocatedObject = nullptr;
			uint RefCount = 0;
		};

	private:
		struct Allocation
		{
			Allocation(void* ptr, const char* file, int line, size_t size)
				: Pointer(ptr), File(file), Line(line), Size(size)
			{}

			~Allocation()
			{
				s_Memory->m_Stats.DeallocationCount++;
				s_Memory->m_Stats.CurrentAllocation -= Size;

				free(Pointer);
			}

			void* Pointer;
			const char* File;
			int Line;
			size_t Size;
		};

		friend bool operator==(const Memory::Allocation& first, void* pointer);

		static Memory* s_Memory;

		MemoryStatistics m_Stats;
		Array<SharedRef*> m_SharedRefs;
		Array<Allocation, RawAllocator> m_Allocations;
	};

	inline bool operator==(const Memory::Allocation& first, void* pointer)
	{
		return first.Pointer == pointer;
	}

	void MemCopy(void* destination, const void* source, size_t bytes);
	int MemCompare(const void* first, const void* second, size_t bytes);
}

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* pointer, const char* file, int line);
void operator delete[](void* pointer, const char* file, int line);
