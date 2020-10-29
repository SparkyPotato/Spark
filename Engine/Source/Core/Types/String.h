// Copyright 2020 SparkyPotato

#pragma once

namespace Spark
{
	struct StringAllocator;
	class StringIterator;

	/*
		Dynamic, heap-allocated Unicode-encoded string. Use the STRING(x) macro to convert to Unicode.
		For example, String str(STRING("Hello")) will work, but String str("Hello") will have to convert from ASCII to Unicode.
		It will also not support any non-ASCII characters
	*/
	class String
	{
	public:
		// Constructs a default string with space for 10 characters
		String();
		// Constructs a string with space for size characters + 1 for the terminating null
		String(uint size);
		// Constructs a string from a STRING()ified const char*
		String(const Char* charArray);
		// Constructs a string from a STRING()ified const char*
		String(Char*& charArray);

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
		String operator+(Char append) const;
		String operator+(const String& append) const;
		String operator+(const Char* append) const;

		// Removes the last character (not the terminating null!)
		String& operator--(); // Prefix
		String operator--(int); // Postfix

		// Append and assign operators
		String& operator+=(Char append);
		String& operator+=(const String& append);
		String& operator+=(const Char* append);

		// Get the character at the index
		Char& operator[](uint offset);

		// Explicitly convert a string to a const Char* (static_cast<const Char*>), can use GetCharPointer() as well
		explicit operator const Char* () const;

		// Iterators
		StringIterator begin() const;
		StringIterator end() const;

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

		// Inserts the given character at the index
		void Insert(Char character, uint index);

		// Erases the character at the index and returns it
		Char Erase(uint index);

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
	};

class StringIterator
{
public:
	StringIterator(Char* pointer);
	
	StringIterator operator+(uint offset);
	
	StringIterator operator-(uint offset);
	
	StringIterator& operator++();
	StringIterator operator++(int);
	StringIterator& operator--();
	StringIterator operator--(int);
	
	Char& operator[](uint offset);
	
	Char& operator*();
	Char* operator->();
	
	friend bool operator==(const StringIterator& first, const StringIterator& second);
	friend bool operator!=(const StringIterator& first, const StringIterator& second);
	
private:
	Char* m_Pointer;
};

	String operator+(const Char* cstr, const String& string);
	bool operator==(const String& first, const String& second);

	bool operator==(const StringIterator& first, const StringIterator& second);
	bool operator!=(const StringIterator& first, const StringIterator& second);
}
