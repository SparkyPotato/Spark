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

		static void* AllocSize(size_t size);
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
		MemoryStatistics m_Stats;
		Array<SharedRef*> m_SharedRefs;
	};

	extern Memory* GMemory;
}
