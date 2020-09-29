#pragma once
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
		String(uint size) noexcept;
		String(const Char* charArray) noexcept;
		String(const String& other) noexcept;
		String(const std::wstring& stdString) noexcept;
		String(String&& other) noexcept;
		// Generates a substring from the given indices (inclusive)
		String(const String& other, uint start, uint end) noexcept;

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

		Char& operator[](uint offset) noexcept;

		// Conversion operators
		explicit operator const Char*() const;
		explicit operator std::wstring() const;

		// Iterators
		Iterator<Char> begin() const noexcept;
		Iterator<Char> end() const noexcept;
		Iterator<const Char> cbegin() const noexcept;
		Iterator<const Char> cend() const noexcept;

		ReverseIterator<Char> rbegin() const noexcept;
		ReverseIterator<Char> rend() const noexcept;
		ReverseIterator<const Char> crbegin() const noexcept;
		ReverseIterator<const Char> crend() const noexcept;

		// Does not include the terminating null character
		uint Length() const;

		// The max amount of characters the string can hold without needing a reallocation
		uint Capacity() const;

		// Reserve characters, so reallocation is not required till the limit is reached
		void Reserve(uint characters);

		// Clear the entire string
		void Clear();

		// Is the string empty?
		bool IsEmpty();

		// Ensure the string is null-terminated. Does not include the ending null character
		uint GetCharPointerLength(const Char* string);

		const Char* GetCharPointer() const;

		// Finds the specified occurrence of the character and returns a constant iterator to it. Defaults to the first occurrence
		Iterator<const Char> Find(Char character, uint occurence = 1) const noexcept;

		// Finds the character occurrence, starting the search at the iterator given. Defaults to the first occurence
		Iterator<const Char> FindAt(Char character, Iterator<Char> start, uint occurence = 1) const noexcept;
		Iterator<const Char> FindAt(Char character, Iterator<const Char> start, uint occurence = 1) const noexcept;

		// Inserts the given character at the iterator position
		template<typename T>
		void Insert(Char character, Iterator<T> iterator) noexcept;
		template<typename T>
		void Insert(Char character, ReverseIterator<T> iterator) noexcept;
		// Inserts the given character at the index
		void Insert(Char character, uint index) noexcept;

		// Erases the character at the iterator position and returns it
		template<typename T>
		Char Erase(Iterator<T> iterator) noexcept;
		template<typename T>
		Char Erase(ReverseIterator<T> iterator) noexcept;
		// Erases the character at the index and returns it
		Char Erase(uint index) noexcept;

		// Erases the substring between the two iterators (inclusive), and returns the erased string
		template<typename T>
		String Erase(Iterator<T> start, Iterator<T> end) noexcept;
		template<typename T>
		String Erase(ReverseIterator<T> start, ReverseIterator<T> end) noexcept;
		// Erases the substring between the two indices (inclusive), and returns the erased string
		String Erase(uint start, uint end) noexcept;

		String& Reverse() noexcept;

	private:
		void Realloc(uint sizeRequired);

		Char* m_DataPointer = nullptr;
		uint m_UsedMemory = 0;
		uint m_AllocatedSize = 0;

	public:
		template<typename Type>
		class Iterator
		{
		public:
			Iterator<Type>(Type* pointer, const String* owner) noexcept
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

			Type& operator[](uint offset) { return m_Pointer[offset]; }

			Type& operator*() noexcept { return *m_Pointer; }
			Type* operator->() noexcept { return m_Pointer; }

			bool operator==(const Iterator<Type>& other) noexcept { return m_Pointer == other.m_Pointer; }
			bool operator!=(const Iterator<Type>& other) noexcept { return m_Pointer != other.m_Pointer; }

		private:
			Type* m_Pointer;
			const String* m_Owner;
		};

		template<typename Type>
		class ReverseIterator
		{
		public:
			ReverseIterator(Type* pointer, const String* owner) noexcept
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

			Type& operator[](uint offset) { return m_Pointer[offset]; }

			Type& operator*() noexcept { return *m_Pointer; }
			Type* operator->() noexcept { return m_Pointer; }

			bool operator==(const ReverseIterator& other) noexcept { return m_Pointer == other.m_Pointer; }
			bool operator!=(const ReverseIterator& other) noexcept { return m_Pointer != other.m_Pointer; }

		private:
			Type* m_Pointer;
			const String* m_Owner;
		};
	};

	class StringStream
	{
	public:
		StringStream() = default;

		const String& GetString();

		StringStream& operator<<(Char character) noexcept;
		StringStream& operator<<(const String& string) noexcept;

		StringStream& operator<<(int integer) noexcept;
		StringStream& operator<<(float decimal) noexcept;
		StringStream& operator<<(double decimal) noexcept;

	private:
		String m_InternalString;
	};

	class Formatter
	{
	public:
		Formatter() = default;

		template<typename ...Args>
		static String Format(String format, Args... args)
		{
			int i = 100;
			String temp(i);
			
			while (swprintf_s(const_cast<Char*>(temp.GetCharPointer()), i, format.GetCharPointer(), args...) == i)
			{
				i *= 2;
				temp.Reserve(i);
			}

			return temp;
		}
	};

	template<typename T>
	Char String::Erase(ReverseIterator<T> iterator) noexcept
	{
		UNIMPLEMENTED(Char);
	}

	template<typename T>
	Char String::Erase(Iterator<T> iterator) noexcept
	{
		UNIMPLEMENTED(Char);
	}

	template<typename T>
	String String::Erase(Iterator<T> start, Iterator<T> end) noexcept
	{
		UNIMPLEMENTED(String)
	}

	template<typename T>
	String String::Erase(ReverseIterator<T> start, ReverseIterator<T> end) noexcept
	{
		UNIMPLEMENTED(String)
	}

	template<typename T>
	void String::Insert(Char character, ReverseIterator<T> iterator) noexcept
	{
		UNIMPLEMENTED(void);
	}

	template<typename T>
	void String::Insert(Char character, Iterator<T> iterator) noexcept
	{
		UNIMPLEMENTED(void);
	}
}
