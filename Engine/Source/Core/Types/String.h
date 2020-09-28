#pragma once
#include "Platform/PlatformInterface.h" // Character encoding is platform specific and also defines string macros

#include <string>

namespace Spark
{
	// Stores strings internally in Unicode
	class String
	{
	public:
		// Iterator stuff
		template<typename Type>
		class Iterator;
		template<typename Type>
		class ReverseIterator;

		friend class Iterator<Char>;
		friend class Iterator<const Char>;
		friend class ReverseIterator<Char>;
		friend class ReverseIterator<const Char>;

	public:
		// Use the STRING(x) macro to convert to Char
		String() noexcept;
		String(unsigned int size) noexcept;
		String(const Char* charArray) noexcept;
		String(const String& other) noexcept;
		String(const std::wstring& stdString) noexcept;
		String(String&& other) noexcept;

		~String();

		String& operator=(const String& other) noexcept;
		String& operator=(String&& other) noexcept;

		// Append operators
		String operator+(Char append) const noexcept;
		String operator+(const String& append) const noexcept;
		String operator+(const Char* append) noexcept;

		// Remove the last character
		String operator--() noexcept;
		String& operator--(int) noexcept;

		// Append and assign operators
		String& operator+=(Char append) noexcept;
		String& operator+=(const String& append) noexcept;
		String& operator+=(const Char* append) noexcept;

		Char& operator[](unsigned int offset) noexcept;

		// Conversion operators
		operator const Char*() const;
		operator std::wstring() const;

		// Iterators
		Iterator<Char> begin();
		Iterator<Char> end();
		Iterator<const Char> cbegin();
		Iterator<const Char> cend();

		ReverseIterator<Char> rbegin();
		ReverseIterator<Char> rend();
		ReverseIterator<const Char> crbegin();
		ReverseIterator<const Char> crend();

		// Does not include the terminating null character
		unsigned int Length() const;

		// The max amount of characters the string can hold without needing a reallocation
		unsigned int Capacity() const;

		// Reserve characters, so reallocation is not required till the limit is reached
		void Reserve(unsigned int characters);

		// Clear the entire string
		void Clear();

		// Is the string empty?
		bool IsEmpty();

		// Ensure the string is null-terminated. Does not include the ending null character
		unsigned int GetCharPointerLength(const Char* string);

		// Finds the specified occurrence of the character and returns a constant iterator to it. Defaults to the first occurrence
		Iterator<const Char> Find(Char character, unsigned int occurence = 1) noexcept;

		// Finds the character occurrence, starting the search at the iterator given. Defaults to the first occurence
		Iterator<const Char> FindAt(Char character, Iterator<Char> start, unsigned int occurence = 1) noexcept;
		Iterator<const Char> FindAt(Char character, Iterator<const Char> start, unsigned int occurence = 1) noexcept;

		// Inserts the given character at the iterator position
		template<typename T>
		void Insert(Char character, Iterator<T> iterator) noexcept;
		template<typename T>
		void Insert(Char character, ReverseIterator<T> iterator) noexcept;
		// Inserts the given character at the index
		void Insert(Char character, unsigned int index) noexcept;

		// Erases the character at the iterator position and returns it
		template<typename T>
		Char Erase(Iterator<T> iterator) noexcept;
		template<typename T>
		Char Erase(ReverseIterator<T> iterator) noexcept;
		// Erases the character at the index and returns it
		Char Erase(unsigned int index) noexcept;

	private:
		void Realloc(unsigned int sizeRequired);

		Char* m_DataPointer = nullptr;
		unsigned int m_UsedMemory = 0;
		unsigned int m_AllocatedSize = 1;

	public:
		template<typename Type>
		class Iterator
		{
		public:
			Iterator<Type>(Type* pointer, String* owner) noexcept
				: m_Pointer(pointer), m_Owner(owner)
			{}

			Iterator<Type> operator++() noexcept 
			{
				auto self = *this;  
				++m_Pointer; 
				return self; 
			}
			Iterator<Type>& operator++(int) noexcept 
			{ 
				++m_Pointer; 
				return *this; 
			}
			Iterator<Type> operator--() noexcept
			{ 
				auto self = *this; 
				--m_Pointer; 
				return self; 
			}
			Iterator<Type>& operator--(int) noexcept 
			{ 
				--m_Pointer; 
				return *this; 
			}

			Type& operator[](unsigned int offset) { return m_Pointer[offset]; }

			Type& operator*() noexcept { return *m_Pointer; }
			Type* operator->() noexcept { return m_Pointer; }

			bool operator==(const Iterator<Type>& other) noexcept { return m_Pointer == other.m_Pointer; }
			bool operator!=(const Iterator<Type>& other) noexcept { return m_Pointer != other.m_Pointer; }

		private:
			Type* m_Pointer;
			String* m_Owner;
		};

		template<typename Type>
		class ReverseIterator
		{
		public:
			ReverseIterator(Type* pointer, String* owner) noexcept
				: m_Pointer(pointer), m_Owner(owner)
			{}

			ReverseIterator operator++() noexcept
			{
				auto self = *this;
				--m_Pointer;
				return self;
			}
			ReverseIterator& operator++(int) noexcept
			{
				--m_Pointer;
				return *this;
			}
			ReverseIterator operator--() noexcept
			{
				auto self = *this;
				++m_Pointer;
				return self;
			}
			ReverseIterator& operator--(int) noexcept
			{
				++m_Pointer;
				return *this;
			}

			Type& operator[](unsigned int offset) { return m_Pointer[offset]; }

			Type& operator*() noexcept { return *m_Pointer; }
			Type* operator->() noexcept { return m_Pointer; }

			bool operator==(const ReverseIterator& other) noexcept { return m_Pointer == other.m_Pointer; }
			bool operator!=(const ReverseIterator& other) noexcept { return m_Pointer != other.m_Pointer; }

		private:
			Type* m_Pointer;
			String* m_Owner;
		};
	};

	template<typename T>
	Char String::Erase(ReverseIterator<T> iterator) noexcept
	{
		UNIMPLEMENTED;
	}

	template<typename T>
	Char String::Erase(Iterator<T> iterator) noexcept
	{
		UNIMPLEMENTED;
	}

	template<typename T>
	void String::Insert(Char character, ReverseIterator<T> iterator) noexcept
	{
		UNIMPLEMENTED;
	}

	template<typename T>
	void String::Insert(Char character, Iterator<T> iterator) noexcept
	{
		UNIMPLEMENTED;
	}
}
