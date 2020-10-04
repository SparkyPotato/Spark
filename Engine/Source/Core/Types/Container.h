#pragma once
#include <initializer_list>

namespace Spark
{
	/*
		Dynamic array
	*/
	template<typename Type>
	class Array
	{
	public:
		class Iterator;
		class ConstIterator;
		class ReverseIterator;
		class ConstReverseIterator;

	public:
		// Default constructor
		Array() noexcept;
		// Have *atleast* size elements without reallocating
		Array(uint size) noexcept;
		// Generate array from initializer list
		Array(std::initializer_list<Type> list) noexcept;
		// Copy constructor
		Array(const Array<Type>& other) noexcept;
		// Move constructor
		Array(Array<Type>&& other) noexcept;

		~Array() noexcept;

		// Copy and move assignment operators
		Array<Type>& operator=(const Array<Type>& other) noexcept;
		Array<Type>& operator=(Array<Type>&& other) noexcept;

		// Get object at index
		Type& operator[](uint index) noexcept;

		// Iterators
		Iterator begin() const noexcept { return Iterator(m_DataPointer); }
		Iterator end() const noexcept { return Iterator(m_DataPointer + m_CreatedObjects); }

		// Construct an object in place at the end of the Array
		template<typename ...Args>
		Type& Emplace(Args... args) noexcept;

		// Copies the object into the end of the Array
		Type& Add(const Type& object) noexcept;

		// Returns the number of objects in the Array
		uint Size();
		// Returns the total current capacity of the Array
		uint Capacity();

	private:
		void Realloc(uint requiredObjects) noexcept;

		Type* m_DataPointer = nullptr;
		uint m_AllocatedSpace = 0;
		uint m_CreatedObjects = 0;

	public:
		class Iterator
		{
		public:
			Iterator(Type* pointer)
				: m_Pointer(pointer)
			{}

			Iterator operator+(uint offset) noexcept
			{
				return Iterator(m_Pointer + offset, m_Owner);
			}

			Iterator operator-(uint offset) noexcept
			{
				return Iterator(m_Pointer - offset, m_Owner);
			}

			Iterator& operator++() noexcept
			{
				m_Pointer++;
				return *this;
			}
			Iterator operator++(int) noexcept
			{
				Iterator temp(m_Pointer, m_Owner);
				m_Pointer++;
				return temp;
			}
			Iterator& operator--() noexcept
			{
				m_Pointer--;
				return *this;
			}
			Iterator operator--(int) noexcept
			{
				Iterator temp(m_Pointer, m_Owner);
				m_Pointer--;
				return temp;
			}

			Type& operator[](uint offset)
			{
				return m_Pointer[offset];
			}

			Type& operator*() noexcept
			{
				return *m_Pointer;
			}
			Type* operator->() noexcept
			{
				return m_Pointer;
			}

			bool operator==(const Iterator& other) noexcept
			{
				return m_Pointer == other.m_Pointer;
			}
			bool operator!=(const Iterator& other) noexcept
			{
				return m_Pointer != other.m_Pointer;
			}

		private:
			Type* m_Pointer;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const Type* pointer)
				: m_Pointer(pointer)
			{}

		private:
			const Type* m_Pointer;
		};

		class ReverseIterator
		{
		public:
			ReverseIterator(Type* pointer)
				: m_Pointer(pointer)
			{}

		private:
			Type* m_Pointer;
		};

		class ConstReverseIterator
		{
		public:
			ConstReverseIterator(const Type* pointer)
				: m_Pointer(pointer)
			{}

		private:
			const Type* m_Pointer;
		};
	};

	template<typename Type>
	Array<Type>::Array() noexcept
	{
		Realloc(2); // Actually allocates for 3 objects
	}

	template<typename Type>
	Array<Type>::Array(uint size) noexcept
	{
		Realloc(size);
	}

	template<typename Type>
	Array<Type>::Array(std::initializer_list<Type> list) noexcept
	{
		Realloc((uint) list.size());

		for (uint i = 0; i < list.size(); i++)
		{
			new (m_DataPointer + i) Type(*(list.begin() + i));
			++m_CreatedObjects;
		}
	}

	template<typename Type>
	Array<Type>::Array(const Array<Type>& other) noexcept
	{
		Realloc(other.m_CreatedObjects);
		m_CreatedObjects = other.m_CreatedObjects;

		for (uint i = 0; i < m_CreatedObjects; i++)
		{
			new (m_DataPointer + i) Type(other.m_DataPointer[i]);
		}
	}

	template<typename Type>
	Array<Type>::Array(Array<Type>&& other) noexcept
	{
		m_DataPointer = other.m_DataPointer;
		m_CreatedObjects = other.m_CreatedObjects;
		m_AllocatedSpace = other.m_AllocatedSpace;

		other.m_DataPointer = nullptr;
	}

	template<typename Type>
	Array<Type>::~Array() noexcept
	{
		for (uint i = 0; i < m_CreatedObjects; i++)
		{
			// We call destructors manually because we don't allocate with new or use delete
			m_DataPointer[i].~Type();
		}

		Memory::Dealloc(m_DataPointer);
	}

	template<typename Type>
	Type& Array<Type>::operator[](uint index) noexcept
	{
		SPARK_ASSERT(index < m_CreatedObjects);

		return m_DataPointer[index];
	}

	template<typename Type> template<typename ...Args>
	Type& Array<Type>::Emplace(Args... args) noexcept
	{
		Realloc(m_CreatedObjects + 1);

		// Placement new to create the object in already allocated memory
		new (m_DataPointer + m_CreatedObjects) Type(args...);
		++m_CreatedObjects;

		return *(m_DataPointer + m_CreatedObjects - 1);
	}

	template<typename Type>
	Type& Array<Type>::Add(const Type& object) noexcept
	{
		Realloc(m_CreatedObjects + 1);

		// Placement new to create the object in memory that was allocated in Realloc
		new (m_DataPointer + m_CreatedObjects) Type(object);
		++m_CreatedObjects;

		return *(m_DataPointer + m_CreatedObjects - 1);
	}

	template<typename Type>
	uint Array<Type>::Capacity()
	{
		return m_AllocatedSpace;
	}

	template<typename Type>
	uint Array<Type>::Size()
	{
		return m_CreatedObjects;
	}

	template<typename Type>
	void Array<Type>::Realloc(uint requiredObjects) noexcept
	{
		if (m_AllocatedSpace >= requiredObjects) return;

		while (m_AllocatedSpace < requiredObjects) (m_AllocatedSpace *= 2) += 1;

		auto newPointer = (Type*) Memory::AllocSize(m_AllocatedSpace * sizeof(Type));
		memcpy(newPointer, m_DataPointer, m_CreatedObjects * sizeof(Type));
		Memory::Dealloc(m_DataPointer);

		m_DataPointer = newPointer;
	}

}
