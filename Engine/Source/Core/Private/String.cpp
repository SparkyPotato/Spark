#include "Core/Types/String.h"
#include <iostream>

namespace Spark
{
	String::String() noexcept
	{
		Realloc(10);
	}

	String::String(unsigned int size) noexcept
	{
		Realloc(size + 1);
	}

	String::String(const Char* charArray) noexcept
	{
		// Allocate memory
		unsigned int arraySize = GetCharPointerLength(charArray);

		Realloc(arraySize);
		m_UsedMemory = arraySize;

		// Copy over data
		memcpy(m_DataPointer, charArray, m_UsedMemory * sizeof(Char));
	}

	String::String(const String& other) noexcept
	{
		// Allocate memory
		Realloc(other.m_UsedMemory);
		m_UsedMemory = other.m_UsedMemory;

		// Copy over data
		memcpy(m_DataPointer, other.m_DataPointer, m_UsedMemory * sizeof(Char));
	}

	String::String(String&& other) noexcept
	{
		// Steal from the other string
		m_AllocatedSize = other.m_AllocatedSize;
		m_UsedMemory = other.m_UsedMemory;
		m_DataPointer = other.m_DataPointer;

		// Set the pointer to null so it deletes happily
		other.m_DataPointer = nullptr;
	}

	String::String(const std::wstring& stdString) noexcept
	{
		// Allocate memory
		Realloc((unsigned int) stdString.size() + 1);
		m_UsedMemory = m_AllocatedSize;

		// Copy
		memcpy(m_DataPointer, stdString.data(), (stdString.size() + 1) * sizeof(Char));
	}

	String::~String()
	{
		delete[] m_DataPointer;
	}

	String& String::operator=(const String& other) noexcept
	{
		// If we have less memory than required, reallocate
		Realloc(other.m_UsedMemory);
		m_UsedMemory = other.m_UsedMemory;

		// Copy over data
		memcpy(m_DataPointer, other.m_DataPointer, m_UsedMemory * sizeof(Char));

		return *this;
	}

	String& String::operator=(String&& other) noexcept
	{
		delete[] m_DataPointer; // Release our currently held memory

		// Steal from the other string
		m_AllocatedSize = other.m_AllocatedSize;
		m_UsedMemory = other.m_UsedMemory;
		m_DataPointer = other.m_DataPointer;

		// Set the pointer to null so it deletes happily
		other.m_DataPointer = nullptr;

		return *this;
	}

	String String::operator+(Char append) const noexcept
	{
		String temp(m_UsedMemory + 1);

		memcpy(temp.m_DataPointer, m_DataPointer, Length() * sizeof(Char)); // Copy our data into the buffer
		temp.m_DataPointer[Length()] = append; // Copy the character to append
		temp.m_DataPointer[m_UsedMemory] = '\0'; // Add a null character

		return temp;
	}

	String String::operator+(const String& append) const noexcept
	{
		// Allocate a buffer with the size of the strings + 1 extra for the null
		Char* data = new Char[m_UsedMemory + append.Length()];
		memcpy(data, m_DataPointer, Length() * sizeof(Char)); // Copy our data
		// Copy the other string's data (including null)
		memcpy(data + Length(), append.m_DataPointer, append.m_UsedMemory * sizeof(Char));

		String temp(data); // Create string, deallocate, and return
		delete[] data;

		return temp;
	}

	String String::operator+(const Char* append) noexcept
	{
		unsigned int size = GetCharPointerLength(append);

		// Allocate a buffer with the size of the strings + 1 extra for the null
		Char* data = new Char[Length() + size];
		memcpy(data, m_DataPointer, Length() * sizeof(Char)); // Copy our data
		// Copy the other string's data (including null)
		memcpy(data + Length(), append, size * sizeof(Char));

		String temp(data); // Create string, deallocate, and return
		delete[] data;

		return temp;
	}

	String& String::operator+=(Char append) noexcept
	{
		// Make sure we have space for one more character
		Realloc(++m_UsedMemory);

		// Realloc modifies the value of m_UsedMemory
		m_DataPointer[m_UsedMemory - 2] = append;
		m_DataPointer[m_UsedMemory - 1] = '\0';

		return *this;
	}

	String& String::operator+=(const String& append) noexcept
	{
		unsigned int originalLength = Length();

		// Make sure we have enough space for both strings
		Realloc(originalLength + append.m_UsedMemory);
		m_UsedMemory = originalLength + append.m_UsedMemory;

		// Copy, including the null
		memcpy(m_DataPointer + originalLength, append.m_DataPointer, append.m_UsedMemory * sizeof(Char));

		return *this;
	}

	String& String::operator+=(const Char* append) noexcept
	{
		unsigned int originalLength = Length();
		unsigned int size = GetCharPointerLength(append);

		// Make sure we have enough space for both strings
		Realloc(originalLength + size);
		m_UsedMemory = originalLength + size;

		memcpy(m_DataPointer + originalLength, append, size * sizeof(Char));

		return *this;
	}

	String& String::operator--() noexcept
	{
		m_DataPointer[--m_UsedMemory] = L'\0';

		return *this;
	}

	Char& String::operator[](unsigned int at) noexcept
	{
		SPARK_ASSERT(at < m_UsedMemory);

		return m_DataPointer[at];
	}

	String::operator const Char*() const
	{
		return m_DataPointer;
	}

	String::operator std::wstring() const
	{
		return std::wstring(m_DataPointer);
	}

	unsigned int String::Length() const
	{
		return m_UsedMemory - 1;
	}

	unsigned int String::Capacity() const
	{
		return m_AllocatedSize - 1;
	}

	void String::Reserve(unsigned int characters)
	{
		SPARK_ASSERT(characters >= m_AllocatedSize);
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

	unsigned int String::GetCharPointerLength(const Char* string)
	{
		const Char* pointer = string;
		int i = 0;
		while (*pointer != '\0')
		{
			i++;
			pointer++;
		}

		return i;
	}

	void String::Realloc(unsigned int sizeRequired)
	{
		if (m_AllocatedSize > sizeRequired)
		{
			return;
		}

		while (m_AllocatedSize < sizeRequired) m_AllocatedSize *= 2;

		auto newPointer = new Char[m_AllocatedSize];
		memcpy(newPointer, m_DataPointer, m_UsedMemory * sizeof(Char));
		delete[] m_DataPointer;

		m_DataPointer = newPointer;
	}
}
