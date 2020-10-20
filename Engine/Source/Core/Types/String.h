// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	/*
		Dynamic, heap-allocated Unicode-encoded string. Use the STRING(x) macro to convert to Unicode.
		For example, String str(STRING("Hello")) will work, but String str("Hello") will have to convert from ASCII to Unicode.
		It will also not support any non-ASCII characters
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
		String();
		// Constructs a string with space for size characters + 1 for the terminating null
		String(uint size);
		// Constructs a string from a STRING()ified const char*
		String(const Char* charArray);
		// Constructs a string from a const char*, by converting into unicode
		String(const char* charArray);

		String(const String& other);
		String(String&& other);
		// Generates a substring from the given indices (both inclusive)
		String(const String& other, uint start, uint end);

		~String();

		// Copy and move assignment operators
		String& operator=(const String& other);
		String& operator=(String&& other);

		friend bool operator==(const String& first, const String& second);

		// Append operators
		String operator+(Char append) const ;
		String operator+(const String& append) const;
		String operator+(const Char* append) const;

		// Removes the last character (not the terminating null!)
		String& operator--(); // Prefix
		String operator--(int); // Postfix

		// Append and assign operators
		String& operator+=(Char append) ;
		String& operator+=(const String& append);
		String& operator+=(const Char* append);

		// Get the character at the index
		Char& operator[](uint offset);

		// Explicitly convert a string to a const Char* (static_cast<const Char*>), can use GetCharPointer() as well
		explicit operator const Char*() const;

		// Iterators
		Iterator begin() const;
		Iterator end() const;
		ConstIterator cbegin() const;
		ConstIterator cend() const;

		ReverseIterator rbegin() const;
		ReverseIterator rend() const;
		ConstReverseIterator crbegin() const;
		ConstReverseIterator crend() const;

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
		ConstIterator Find(Char character, uint occurence = 1) const;
		// Finds the specified occurence of the string and returns a constant iterator to the first character of it. Defaults to the first occurence
		ConstIterator Find(const String& string, uint occurence = 1) const;

		// Finds the character occurrence, starting the search at the iterator given. Defaults to the first occurence
		ConstIterator FindAt(Char character, Iterator start, uint occurence = 1) const;
		ConstIterator FindAt(Char character, ConstIterator start, uint occurence = 1) const;
		// Finds the character occurrence, starting the backwards search at the iterator given. Defaults to the first occurence
		ConstIterator FindAt(Char character, ReverseIterator start, uint occurence = 1) const;
		ConstIterator FindAt(Char character, ConstReverseIterator start, uint occurence = 1) const;

		// Finds the string occurence, starting the search at the given iterator. No reverse iterator version exists
		ConstIterator FindAt(const String& character, Iterator start, uint occurence = 1) const;
		ConstIterator FindAt(const String& character, ConstIterator start, uint occurence = 1) const;

		// Inserts the given character at the iterator position
		void Insert(Char character, Iterator iterator);
		void Insert(Char character, ConstIterator iterator);
		void Insert(Char character, ReverseIterator iterator);
		void Insert(Char character, ConstReverseIterator iterator);
		// Inserts the given character at the index
		void Insert(Char character, uint index) ;

		// Inserts the given string at the iterator position
		void Insert(String character, Iterator iterator);
		void Insert(String character, ConstIterator iterator);
		void Insert(String character, ReverseIterator iterator);
		void Insert(String character, ConstReverseIterator iterator);
		// Inserts the given string at the index
		void Insert(String character, uint index);

		// Erases the character at the iterator position and returns it
		Char Erase(Iterator iterator);
		Char Erase(ConstIterator iterator);
		Char Erase(ReverseIterator iterator);
		Char Erase(ConstReverseIterator iterator);
		// Erases the character at the index and returns it
		Char Erase(uint index) ;

		// Erases the substring between the two iterators (both inclusive), and returns the erased string
		String Erase(Iterator start, Iterator end);
		String Erase(ConstIterator start, ConstIterator end);
		String Erase(ReverseIterator start, ReverseIterator end);
		String Erase(ConstReverseIterator start, ConstReverseIterator end);
		// Erases the substring between the two indices (both inclusive), and returns the erased string
		String Erase(uint start, uint end);

		// Reverses the order of the entire string
		String& Reverse();

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
			Iterator(Char* pointer);

			Iterator operator+(uint offset);

			Iterator operator-(uint offset);

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			Char& operator[](uint offset);

			Char& operator*();
			Char* operator->();

			friend bool operator==(const Iterator& first, const Iterator& second);
			friend bool operator!=(const Iterator& first, const Iterator& second);

		private:
			Char* m_Pointer;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const Char* pointer);

			ConstIterator operator+(uint offset);

			ConstIterator operator-(uint offset);

			ConstIterator& operator++();
			ConstIterator operator++(int);
			ConstIterator& operator--();
			ConstIterator operator--(int);

			const Char& operator[](uint offset);

			const Char& operator*();
			const Char* operator->();

			friend bool operator==(const ConstIterator& first, const ConstIterator& second);
			friend bool operator!=(const ConstIterator& first, const ConstIterator& second);

		private:
			const Char* m_Pointer;
		};

		class ReverseIterator
		{
		public:
			ReverseIterator(Char* pointer);

			ReverseIterator operator+(uint offset);

			ReverseIterator operator-(uint offset);

			ReverseIterator& operator++();
			ReverseIterator operator++(int);
			ReverseIterator& operator--();
			ReverseIterator operator--(int);

			Char& operator[](uint offset);

			Char& operator*();
			Char* operator->();

			friend bool operator==(const ReverseIterator& first, const ReverseIterator& second);
			friend bool operator!=(const ReverseIterator& first, const ReverseIterator& second);

		private:
			Char* m_Pointer;
		};

		class ConstReverseIterator
		{
		public:
			ConstReverseIterator(const Char* pointer);

			ConstReverseIterator operator+(uint offset);

			ConstReverseIterator operator-(uint offset);

			ConstReverseIterator& operator++();
			ConstReverseIterator operator++(int);
			ConstReverseIterator& operator--();
			ConstReverseIterator operator--(int);

			const Char& operator[](uint offset);

			const Char& operator*();
			const Char* operator->();

			friend bool operator==(const ConstReverseIterator& first, const ConstReverseIterator& second);
			friend bool operator!=(const ConstReverseIterator& first, const ConstReverseIterator& second);

		private:
			const Char* m_Pointer;
		};
	};

	String operator+(const Char* cstr, const String& string);
	bool operator==(const String& first, const String& second);

	bool operator==(const String::Iterator& first, const String::Iterator& second);
	bool operator!=(const String::Iterator& first, const String::Iterator& second);

	bool operator==(const String::ConstIterator& first, const String::ConstIterator& second);
	bool operator!=(const String::ConstIterator& first, const String::ConstIterator& second);

	bool operator==(const String::ReverseIterator& first, const String::ReverseIterator& second);
	bool operator!=(const String::ReverseIterator& first, const String::ReverseIterator& second);

	bool operator==(const String::ConstReverseIterator& first, const String::ConstReverseIterator& second);
	bool operator!=(const String::ConstReverseIterator& first, const String::ConstReverseIterator& second);

	class StringStream
	{
	public:
		StringStream() = default;

		const String& GetString();

		StringStream& operator<<(Char character);
		StringStream& operator<<(const String& string);

		StringStream& operator<<(int integer);
		StringStream& operator<<(float decimal);
		StringStream& operator<<(double decimal);

	private:
		String m_InternalString;
	};

	class Formatter
	{
	public:
		Formatter() = delete; // We don't construct Formatters yet

		// Format the given string using swprintf_s. Defaults to assuming the string is 500 characters long
		template<typename ...Args>
		static String Format(const String& format, Args... args)
		{
			constexpr uint BUF_LENGTH = 500;

			Char temp[BUF_LENGTH];
			
			swprintf_s(temp, BUF_LENGTH, format.GetCharPointer(), args...);

			return temp;
		}
	};
}
