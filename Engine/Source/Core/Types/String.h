#pragma once

namespace Spark
{
	/*
		Dynamic, Heap-allocated Unicode-encoded string. Use the STRING(x) macro to convert to Unicode.
		For example, String str(STRING("Hello")) will work, but String str("Hello") will not.
	*/
	class String
	{
	public:
		// Iterator forward declarations
		class Iterator;
		class ConstIterator;
		class ReverseIterator;
		class ConstReverseIterator;

		friend class Formatter;

	public:
		// Constructs a default string with space for 10 characters
		String() noexcept;
		// Constructs a string with space for size characters + 1 for the terminating null
		String(uint size) noexcept;
		// Constructs a string from a STRING()ified const char*
		String(const Char* charArray) noexcept;
		// Copy constructor
		String(const String& other) noexcept;
		// Move constructor
		String(String&& other) noexcept;
		// Generates a substring from the given indices (both inclusive)
		String(const String& other, uint start, uint end) noexcept;

		~String();

		// Copy and move assignment operators
		String& operator=(const String& other) noexcept;
		String& operator=(String&& other) noexcept;

		// Append operators
		String operator+(Char append) const noexcept;
		String operator+(const String& append) const noexcept;
		String operator+(const Char* append) const noexcept;

		// Removes the last character (not the terminating null!)
		String& operator--() noexcept; // Prefix
		String operator--(int) noexcept; // Postfix

		// Append and assign operators
		String& operator+=(Char append) noexcept;
		String& operator+=(const String& append) noexcept;
		String& operator+=(const Char* append) noexcept;

		// Get the character at the index
		Char& operator[](uint offset) noexcept;

		// Explicitly convert a string to a const Char* (static_cast<const Char*>), can use GetCharPointer() as well
		explicit operator const Char*() const;

		// Iterators
		Iterator begin() const noexcept;
		Iterator end() const noexcept;
		ConstIterator cbegin() const noexcept;
		ConstIterator cend() const noexcept;

		ReverseIterator rbegin() const noexcept;
		ReverseIterator rend() const noexcept;
		ConstReverseIterator crbegin() const noexcept;
		ConstReverseIterator crend() const noexcept;

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

		// Ensure the string is null-terminated. Does not include the ending null character in the length
		static uint GetCharPointerLength(const Char* string);

		// Converts the string to a const Char*, can use the explicit conversion operator as well
		const Char* GetCharPointer() const;
		// Gets access to the raw string data, which can be modified
		Char* GetDataPointer();

		// Finds the specified occurrence of the character and returns a constant iterator to it. Defaults to the first occurrence
		ConstIterator Find(Char character, uint occurence = 1) const noexcept;
		// Finds the specified occurence of the string and returns a constant iterator to the first character of it. Defaults to the first occurence
		ConstIterator Find(const String& string, uint occurence = 1) const noexcept;

		// Finds the character occurrence, starting the search at the iterator given. Defaults to the first occurence
		ConstIterator FindAt(Char character, Iterator start, uint occurence = 1) const noexcept;
		ConstIterator FindAt(Char character, ConstIterator start, uint occurence = 1) const noexcept;
		// Finds the character occurrence, starting the backwards search at the iterator given. Defaults to the first occurence
		ConstIterator FindAt(Char character, ReverseIterator start, uint occurence = 1) const noexcept;
		ConstIterator FindAt(Char character, ConstReverseIterator start, uint occurence = 1) const noexcept;

		// Finds the string occurence, starting the search at the given iterator. No reverse iterator version exists
		ConstIterator FindAt(const String& character, Iterator start, uint occurence = 1) const noexcept;
		ConstIterator FindAt(const String& character, ConstIterator start, uint occurence = 1) const noexcept;

		// Inserts the given character at the iterator position
		void Insert(Char character, Iterator iterator) noexcept;
		void Insert(Char character, ConstIterator iterator) noexcept;
		void Insert(Char character, ReverseIterator iterator) noexcept;
		void Insert(Char character, ConstReverseIterator iterator) noexcept;
		// Inserts the given character at the index
		void Insert(Char character, uint index) noexcept;

		// Inserts the given string at the iterator position
		void Insert(String character, Iterator iterator) noexcept;
		void Insert(String character, ConstIterator iterator) noexcept;
		void Insert(String character, ReverseIterator iterator) noexcept;
		void Insert(String character, ConstReverseIterator iterator) noexcept;
		// Inserts the given string at the index
		void Insert(String character, uint index) noexcept;

		// Erases the character at the iterator position and returns it
		Char Erase(Iterator iterator) noexcept;
		Char Erase(ConstIterator iterator) noexcept;
		Char Erase(ReverseIterator iterator) noexcept;
		Char Erase(ConstReverseIterator iterator) noexcept;
		// Erases the character at the index and returns it
		Char Erase(uint index) noexcept;

		// Erases the substring between the two iterators (both inclusive), and returns the erased string
		String Erase(Iterator start, Iterator end) noexcept;
		String Erase(ConstIterator start, ConstIterator end) noexcept;
		String Erase(ReverseIterator start, ReverseIterator end) noexcept;
		String Erase(ConstReverseIterator start, ConstReverseIterator end) noexcept;
		// Erases the substring between the two indices (both inclusive), and returns the erased string
		String Erase(uint start, uint end) noexcept;

		// Reverses the order of the entire string
		String& Reverse() noexcept;

	private:
		// Ensure the string has enough space for sizeRequired characters (including terminating null), and reallocate if required
		void Realloc(uint sizeRequired);

		Char* m_DataPointer = nullptr;
		// Amount of memory used in the allocated region. m_DataPointer[m_UsedMemory - 1] is always null ('\0')
		uint m_UsedMemory = 0;
		// Total allocated memory
		uint m_AllocatedSize = 0;

	public:
		class Iterator
		{
		public:
			Iterator(Char* pointer) noexcept;

			Iterator operator+(uint offset) noexcept;

			Iterator operator-(uint offset) noexcept;

			Iterator& operator++() noexcept;
			Iterator operator++(int) noexcept;
			Iterator& operator--() noexcept;
			Iterator operator--(int) noexcept;

			Char& operator[](uint offset);

			Char& operator*() noexcept;
			Char* operator->() noexcept;

			bool operator==(const Iterator& other) noexcept;
			bool operator!=(const Iterator& other) noexcept;

		private:
			Char* m_Pointer;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const Char* pointer) noexcept;

			ConstIterator operator+(uint offset) noexcept;

			ConstIterator operator-(uint offset) noexcept;

			ConstIterator& operator++() noexcept;
			ConstIterator operator++(int) noexcept;
			ConstIterator& operator--() noexcept;
			ConstIterator operator--(int) noexcept;

			const Char& operator[](uint offset);

			const Char& operator*() noexcept;
			const Char* operator->() noexcept;

			bool operator==(const ConstIterator& other) noexcept;
			bool operator!=(const ConstIterator& other) noexcept;

		private:
			const Char* m_Pointer;
		};

		class ReverseIterator
		{
		public:
			ReverseIterator(Char* pointer) noexcept;

			ReverseIterator operator+(uint offset) noexcept;

			ReverseIterator operator-(uint offset) noexcept;

			ReverseIterator& operator++() noexcept;
			ReverseIterator operator++(int) noexcept;
			ReverseIterator& operator--() noexcept;
			ReverseIterator operator--(int) noexcept;

			Char& operator[](uint offset);

			Char& operator*() noexcept;
			Char* operator->() noexcept;

			bool operator==(const ReverseIterator& other) noexcept;
			bool operator!=(const ReverseIterator& other) noexcept;

		private:
			Char* m_Pointer;
		};

		class ConstReverseIterator
		{
		public:
			ConstReverseIterator(const Char* pointer) noexcept;

			ConstReverseIterator operator+(uint offset) noexcept;

			ConstReverseIterator operator-(uint offset) noexcept;

			ConstReverseIterator& operator++() noexcept;
			ConstReverseIterator operator++(int) noexcept;
			ConstReverseIterator& operator--() noexcept;
			ConstReverseIterator operator--(int) noexcept;

			const Char& operator[](uint offset);

			const Char& operator*() noexcept;
			const Char* operator->() noexcept;

			bool operator==(const ConstReverseIterator& other) noexcept;
			bool operator!=(const ConstReverseIterator& other) noexcept;

		private:
			const Char* m_Pointer;
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
		Formatter() = delete; // We don't construct Formatters yet

		// Format the given string using swprintf_s. Defaults to assuming the string is 100 characters long,
		// but doubles the size every failed attempt and tries to format again
		template<typename ...Args>
		static String Format(const String& format, Args... args)
		{
			int i = format.Length() * 2 + 100;
			String temp(i);
			
			uint written = swprintf_s(temp.GetDataPointer(), i, format.GetCharPointer(), args...);
			temp.m_UsedMemory = written + 1;

			return temp;
		}
	};
}
