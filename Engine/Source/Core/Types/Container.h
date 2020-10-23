// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	template<typename Type>
	class ArrayIterator;
	struct HeapAllocator;

	/*
		Dynamic array.
		Does not require template type to have default, copy, or move constructors.
	*/
	template<typename Type, typename Allocator = HeapAllocator>
	class Array
	{
	public:
		// Default constructor
		Array()
		{
			Realloc(2); // Actually allocates for 3 objects
		}
		// Have *atleast* size elements without reallocating
		Array(uint size)
		{
			Realloc(size);
		}
		// Generate array from initializer list
		Array(std::initializer_list<Type> list)
		{
			Realloc((uint)list.size());

			for (uint i = 0; i < list.size(); i++)
			{
				new (m_DataPointer + i) Type(*(list.begin() + i));
				++m_CreatedObjects;
			}
		}
		// Copy constructor
		Array(const Array<Type>& other)
		{
			Realloc(other.m_CreatedObjects);
			m_CreatedObjects = other.m_CreatedObjects;

			for (uint i = 0; i < m_CreatedObjects; i++)
			{
				new (m_DataPointer + i) Type(other.m_DataPointer[i]);
			}
		}
		// Move constructor
		Array(Array<Type>&& other)
		{
			m_DataPointer = other.m_DataPointer;
			m_CreatedObjects = other.m_CreatedObjects;
			m_AllocatedSpace = other.m_AllocatedSpace;

			other.m_DataPointer = nullptr;
		}

		~Array()
		{
			for (uint i = 0; i < m_CreatedObjects; i++)
			{
				// We call destructors manually because we don't allocate with new or use delete
				m_DataPointer[i].~Type();
			}

			Allocator::Deallocate(m_DataPointer);
		}

		// Copy and move assignment operators
		Array<Type>& operator=(const Array<Type>& other)
		{
			~Array<Type>();

			Realloc(other.m_CreatedObjects);
			m_CreatedObjects = other.m_CreatedObjects;

			for (uint i = 0; i < m_CreatedObjects; i++)
			{
				new (m_DataPointer + i) Type(other.m_DataPointer[i]);
			}
		}

		Array<Type>& operator=(Array<Type>&& other)
		{
			m_DataPointer = other.m_DataPointer;
			m_CreatedObjects = other.m_CreatedObjects;
			m_AllocatedSpace = other.m_AllocatedSpace;

			other.m_DataPointer = nullptr;
		}

		// Get object at index
		Type& operator[](uint index)
		{
			SPARK_ASSERT(index < m_CreatedObjects, STRING("Out of index range!"));

			return m_DataPointer[index];
		}

		// Iterators
		ArrayIterator<Type> begin() const { return ArrayIterator(m_DataPointer); }
		ArrayIterator<Type> end() const { return ArrayIterator(m_DataPointer + m_CreatedObjects); }

		// Construct an object in place at the end of the Array
		template<typename ...Args>
		Type& Emplace(Args... args)
		{
			Realloc(m_CreatedObjects + 1);

			// Placement new to create the object in already allocated memory
			new (m_DataPointer + m_CreatedObjects) Type(args...);
			++m_CreatedObjects;

			return *(m_DataPointer + m_CreatedObjects - 1);
		}

		// Copies the object into the end of the Array
		Type& Add(const Type& object)
		{
			Realloc(m_CreatedObjects + 1);

			// Placement new to create the object in memory that was allocated in Realloc
			new (m_DataPointer + m_CreatedObjects) Type(object);
			++m_CreatedObjects;

			return *(m_DataPointer + m_CreatedObjects - 1);
		}

		Type& Insert(const Type& object, uint position)
		{
			Realloc(m_CreatedObjects + 1);
			
			for (uint i = position; i < m_CreatedObjects; i++)
			{
				MemCopy(m_DataPointer + i + 1, m_DataPointer + i, sizeof(Type));
			}

			new (m_DataPointer + position) Type(object);
			++m_CreatedObjects;

			return *(m_DataPointer + position);
		}

		// Erases the object at the index and returns a reference
		void Erase(uint index)
		{
			m_DataPointer[index].~Type();
			for (uint i = index; i < m_CreatedObjects; i++)
			{
				MemCopy(m_DataPointer + i, m_DataPointer + i + 1, sizeof(Type));
			}
			m_CreatedObjects--;
		}

		template<typename T>
		uint Find(const T& object)
		{
			for (uint i = 0; i < m_CreatedObjects; i++)
			{
				if (m_DataPointer[i] == object) return i;
			}

			return -1;
		}

		// Returns the number of objects in the Array
		uint Size()
		{
			return m_CreatedObjects;
		}
		// Returns the total current capacity of the Array
		uint Capacity()
		{
			return m_AllocatedSpace;
		}

	private:
		void Realloc(uint requiredObjects)
		{
			if (m_AllocatedSpace >= requiredObjects) return;

			while (m_AllocatedSpace < requiredObjects) (m_AllocatedSpace *= 2) += 1;

			auto newPointer = reinterpret_cast<Type*>(Allocator::Allocate(m_AllocatedSpace * sizeof(Type)));
			MemCopy(newPointer, m_DataPointer, m_CreatedObjects * sizeof(Type));
			Allocator::Deallocate(m_DataPointer);

			m_DataPointer = newPointer;
		}

		Type* m_DataPointer = nullptr;
		uint m_AllocatedSpace = 0;
		uint m_CreatedObjects = 0;
	};

	template<typename Type>
	class ArrayIterator
	{
	public:
		ArrayIterator(Type* pointer)
			: m_Pointer(pointer)
		{}

		ArrayIterator operator+(uint offset)
		{
			return ArrayIterator(m_Pointer + offset);
		}

		ArrayIterator operator-(uint offset)
		{
			return ArrayIterator(m_Pointer - offset);
		}

		ArrayIterator& operator++()
		{
			m_Pointer++;
			return *this;
		}
		ArrayIterator operator++(int)
		{
			ArrayIterator temp(m_Pointer);
			m_Pointer++;
			return temp;
		}
		ArrayIterator& operator--()
		{
			m_Pointer--;
			return *this;
		}
		ArrayIterator operator--(int)
		{
			ArrayIterator temp(m_Pointer);
			m_Pointer--;
			return temp;
		}

		Type& operator[](uint offset)
		{
			return m_Pointer[offset];
		}

		Type& operator*()
		{
			return *m_Pointer;
		}
		Type* operator->()
		{
			return m_Pointer;
		}

		template<typename T>
		friend bool operator==(ArrayIterator<T> first, ArrayIterator<T> second);

		template<typename T>
		friend bool operator!=(ArrayIterator<T> first, ArrayIterator<T> second);

	private:
		Type* m_Pointer;
	};	

	template<typename T>
	bool operator==(typename ArrayIterator<T> first, typename ArrayIterator<T> second)
	{
		return first.m_Pointer == second.m_Pointer;
	}

	template<typename T>
	bool operator!=(typename ArrayIterator<T> first, typename ArrayIterator<T> second)
	{
		return first.m_Pointer != second.m_Pointer;
	}
}
