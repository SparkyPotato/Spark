// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	class Object;

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

		static void* AllocSize(size_t size, const char* file, int line);
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

			void* Pointer;
			const char* File;
			int Line;
			size_t Size;
		};

		friend bool operator==(const Memory::Allocation& first, void* pointer);

		static Memory* s_Memory;

		MemoryStatistics m_Stats;
		Array<SharedRef*> m_SharedRefs;

		Allocation* m_Allocations = nullptr;
		uint m_AllocationSize = 100;
		uint m_AllocationHead = 0;
	};

	inline bool operator==(const Memory::Allocation& first, void* pointer)
	{
		return first.Pointer == pointer;
	}
}

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* pointer, const char* file, int line);
