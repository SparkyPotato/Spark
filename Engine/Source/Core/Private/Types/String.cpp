// Copyright 2020 SparkyPotato

#include "Core/Types/String.h"

namespace Spark
{
	String::String() 
	{
		Realloc(11);
		m_DataPointer[0] = L'\0';
		m_UsedMemory = 1;
	}

	String::String(uint size) 
	{
		Realloc(size + 1);
		m_DataPointer[0] = L'\0';
		m_UsedMemory = 1;
	}

	String::String(const Char* charArray) 
	{
		// Allocate memory
		uint arraySize = GetCharPointerLength(charArray);

		Realloc(arraySize);
		m_UsedMemory = arraySize;

		// Copy over data
		MemCopy(m_DataPointer, charArray, m_UsedMemory * sizeof(Char));
	}

	String::String(Char*& charArray)
	{
		// Allocate memory
		m_AllocatedSize = m_UsedMemory = GetCharPointerLength(charArray);

		m_DataPointer = charArray;

		charArray = nullptr;
	}

	String::String(const char* charArray)
	{
		Char* temp = Platform::ToUnicode(charArray);

		uint arraySize = GetCharPointerLength(temp);

		m_AllocatedSize = m_UsedMemory = arraySize;
		m_DataPointer = temp;
	}

	String::String(const String& other) 
	{
		// Allocate memory
		Realloc(other.m_UsedMemory);
		m_UsedMemory = other.m_UsedMemory;

		// Copy over data
		MemCopy(m_DataPointer, other.m_DataPointer, m_UsedMemory * sizeof(Char));
	}

	String::String(String&& other) 
	{
		// Steal from the other string
		m_AllocatedSize = other.m_AllocatedSize;
		m_UsedMemory = other.m_UsedMemory;
		m_DataPointer = other.m_DataPointer;

		// Set the pointer to null so it deletes happily
		other.m_DataPointer = nullptr;
	}

	String::String(const String& other, uint start, uint end) 
	{
		SPARK_ASSERT(start <= end, STRING("end > start!"));

		Realloc(end - start + 2);
		m_UsedMemory = end - start + 2;

		MemCopy(m_DataPointer, other.GetCharPointer() + start, (end - start  + 1) * sizeof(Char));

		m_DataPointer[m_UsedMemory - 1] = L'\0';
	}

	String::~String()
	{
		StringAllocator::Deallocate(m_DataPointer);
	}

	String& String::operator=(const String& other)
	{
		// If we have less memory than required, reallocate
		Realloc(other.m_UsedMemory);
		m_UsedMemory = other.m_UsedMemory;

		// Copy over data
		MemCopy(m_DataPointer, other.m_DataPointer, m_UsedMemory * sizeof(Char));

		return *this;
	}

	String& String::operator=(String&& other) 
	{
		StringAllocator::Deallocate(m_DataPointer); // Release our currently held memory

		// Steal from the other string
		m_AllocatedSize = other.m_AllocatedSize;
		m_UsedMemory = other.m_UsedMemory;
		m_DataPointer = other.m_DataPointer;

		// Set the pointer to null so it deletes happily
		other.m_DataPointer = nullptr;

		return *this;
	}

	bool operator==(const String& first, const String& second)
	{
		if (first.m_UsedMemory != second.m_UsedMemory) return false;

		if (memcmp(first.m_DataPointer, second.m_DataPointer, first.m_UsedMemory * sizeof(Char)) == 0)
		{
			return true;
		}

		return false;
	}

	String String::operator+(Char append) const
	{
		String temp(m_UsedMemory + 1);

		MemCopy(temp.m_DataPointer, m_DataPointer, Length() * sizeof(Char)); // Copy our data into the buffer
		temp.m_DataPointer[Length()] = append; // Copy the character to append
		temp.m_DataPointer[m_UsedMemory] = '\0'; // Add a null character

		return temp;
	}

	String String::operator+(const String& append) const 
	{
		// Allocate a buffer with the size of the strings + 1 extra for the null
		Char* data = StringAllocator::Allocate(m_UsedMemory + append.Length());
		MemCopy(data, m_DataPointer, Length() * sizeof(Char)); // Copy our data
		// Copy the other string's data (including null)
		MemCopy(data + Length(), append.m_DataPointer, append.m_UsedMemory * sizeof(Char));

		return String(data);
	}

	String String::operator+(const Char* append) const 
	{
		uint size = GetCharPointerLength(append);

		// Allocate a buffer with the size of the strings + 1 extra for the null
		Char* data = new Char[Length() + size];
		MemCopy(data, m_DataPointer, Length() * sizeof(Char)); // Copy our data
		// Copy the other string's data (including null)
		MemCopy(data + Length(), append, size * sizeof(Char));

		String temp(data); // Create string, deallocate, and return
		delete[] data;

		return temp;
	}

	String& String::operator+=(Char append) 
	{
		// Make sure we have space for one more character
		Realloc(++m_UsedMemory);

		// Realloc modifies the value of m_UsedMemory
		m_DataPointer[m_UsedMemory - 2] = append;
		m_DataPointer[m_UsedMemory - 1] = '\0';

		return *this;
	}

	String& String::operator+=(const String& append) 
	{
		uint originalLength = Length();

		// Make sure we have enough space for both strings
		Realloc(originalLength + append.m_UsedMemory);
		m_UsedMemory = originalLength + append.m_UsedMemory;

		// Copy, including the null
		MemCopy(m_DataPointer + originalLength, append.m_DataPointer, append.m_UsedMemory * sizeof(Char));

		return *this;
	}

	String& String::operator+=(const Char* append) 
	{
		uint originalLength = Length();
		uint size = GetCharPointerLength(append);

		// Make sure we have enough space for both strings
		Realloc(originalLength + size);
		m_UsedMemory = originalLength + size;

		MemCopy(m_DataPointer + originalLength, append, size * sizeof(Char));

		return *this;
	}

	String& String::operator--() 
	{
		m_DataPointer[--m_UsedMemory] = L'\0';

		return *this;
	}

	String String::operator--(int) 
	{
		String string = *this;
		m_DataPointer[--m_UsedMemory] = L'\0';

		return string;
	}

	Char& String::operator[](uint offset) 
	{
		SPARK_ASSERT(offset < m_UsedMemory, STRING("Out of index range!"));

		return m_DataPointer[offset];
	}

	String::operator const Char* () const
	{
		return m_DataPointer;
	}

	String::Iterator String::begin() const 
	{
		return Iterator(m_DataPointer);
	}

	String::Iterator String::end() const 
	{
		return Iterator(m_DataPointer + m_UsedMemory);
	}

	uint String::Length() const
	{
		return m_UsedMemory - 1;
	}

	uint String::Capacity() const
	{
		return m_AllocatedSize - 1;
	}

	void String::Reserve(uint characters)
	{
		Realloc(characters + 1);
	}

	void String::Clear()
	{
		m_UsedMemory = 0;
		*m_DataPointer = L'\0';
	}

	bool String::IsEmpty()
	{
		return m_UsedMemory == 0;
	}

	uint String::GetCharPointerLength(const Char* string)
	{
		const Char* pointer = string;
		int i = 0;
		while (*pointer != '\0')
		{
			i++;
			pointer++;
		}

		return i + 1;
	}

	const Char* String::GetCharPointer() const
	{
		return m_DataPointer;
	}

	Char* String::GetDataPointer()
	{
		return m_DataPointer;
	}

	void String::Insert(Char character, uint index) 
	{
		UNIMPLEMENTED(void);
	}

	Char String::Erase(uint index) 
	{
		Char temp = m_DataPointer[index];

		for (uint i = index; i < Length(); i++)
		{
			m_DataPointer[i] = m_DataPointer[i + 1];
		}

		return temp;
	}

	String& String::Reverse() 
	{
		for (uint i = 0; i < Length() / 2; ++i)
		{
			Char temp = m_DataPointer[i];
			m_DataPointer[i] = m_DataPointer[Length() - 1 - i];
			m_DataPointer[Length() - 1 - i] = temp;
		}

		return *this;
	}

	void String::Realloc(uint sizeRequired)
	{
		if (m_AllocatedSize > sizeRequired)
		{
			return;
		}

		while (m_AllocatedSize < sizeRequired) (m_AllocatedSize *= 2) += 1;

		auto newPointer = StringAllocator::Allocate(m_AllocatedSize);
		MemCopy(newPointer, m_DataPointer, m_UsedMemory * sizeof(Char));
		StringAllocator::Deallocate(m_DataPointer);

		m_DataPointer = newPointer;
	}

	String::Iterator::Iterator(Char* pointer)  
		: m_Pointer(pointer)
	{}

	String::Iterator String::Iterator::operator+(uint offset) 
	{
		return Iterator(m_Pointer + offset);
	}

	String::Iterator String::Iterator::operator-(uint offset) 
	{
		return Iterator(m_Pointer - offset);
	}

	String::Iterator String::Iterator::operator++(int) 
	{
		auto self = *this;
		++m_Pointer;
		return self;
	}

	String::Iterator& String::Iterator::operator++() 
	{
		++m_Pointer;
		return *this;
	}

	String::Iterator String::Iterator::operator--(int) 
	{

		auto self = *this;
		--m_Pointer;
		return self;
	}

	String::Iterator& String::Iterator::operator--() 
	{

		--m_Pointer;
		return *this;
	}

	Char& String::Iterator::operator[](uint offset)
	{
		return m_Pointer[offset];
	}

	Char& String::Iterator::operator*() 
	{
		return *m_Pointer;
	}

	Char* String::Iterator::operator->() 
	{
		return m_Pointer;
	}

	String operator+(const Char* cstr, const String& string)
	{
		return String(cstr) + string;
	}

	bool operator==(const String::Iterator& first, const String::Iterator& second)
	{
		return first.m_Pointer == second.m_Pointer;
	}

	bool operator!=(const String::Iterator& first, const String::Iterator& second) 
	{
		return !(first == second);
	}
}
