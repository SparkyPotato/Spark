// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	struct HeapAllocator
	{
		static void* Allocate(size_t size)
		{
			return MemAlloc(size);
		}

		static void Deallocate(void* pointer)
		{
			MemFree(pointer);
		}
	};

	struct RawAllocator
	{
		static void* Allocate(size_t size)
		{
			return malloc(size);
		}

		static void Deallocate(void* pointer)
		{
			free(pointer);
		}
	};

	struct StringAllocator
	{
		static Char* Allocate(uint count)
		{
			return reinterpret_cast<Char*>(MemAlloc(sizeof(Char) * count));
		}

		static void Deallocate(Char* pointer)
		{
			MemFree(pointer);
		}

	private:
		static Array<Char*, RawAllocator> m_Allocations;
		static Char* m_StringBuffer;
		static size_t m_Size;
	};
}
