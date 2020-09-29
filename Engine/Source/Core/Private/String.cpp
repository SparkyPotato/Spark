#include "Core/Types/String.h"
#include <iostream>

namespace Spark
{
	String::String() noexcept
	{
		Realloc(10);
		m_DataPointer[0] = L'\0';
	}

	String::String(unsigned int size) noexcept
	{
		Realloc(size + 1);
		m_DataPointer[0] = L'\0';
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

	String::String(const String& other, unsigned int start, unsigned int end) noexcept
	{
		SPARK_ASSERT(start <= end);

		Realloc(end - start + 2);
		m_UsedMemory = end - start + 2;

		memcpy(m_DataPointer, &other[start], (end - start  + 1) * sizeof(Char));

		m_DataPointer[m_UsedMemory - 1] = L'\0';
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

	String String::operator--() noexcept
	{
		String string = *this;
		m_DataPointer[--m_UsedMemory] = L'\0';

		return string;
	}

	String& String::operator--(int) noexcept
	{
		m_DataPointer[--m_UsedMemory] = L'\0';

		return *this;
	}

	Char& String::operator[](unsigned int offset) noexcept
	{
		SPARK_ASSERT(offset < m_UsedMemory);

		return m_DataPointer[offset];
	}

	String::operator const Char*() const
	{
		return m_DataPointer;
	}

	String::operator std::wstring() const
	{
		return std::wstring(m_DataPointer);
	}

	String::Iterator<Char> String::begin() const noexcept
	{
		return Iterator<Char>(m_DataPointer, this);
	}

	String::Iterator<Char> String::end() const noexcept
	{
		return Iterator<Char>(m_DataPointer + m_UsedMemory, this);
	}

	String::Iterator<const Char> String::cbegin() const noexcept
	{
		return Iterator<const Char>(m_DataPointer, this);
	}

	String::Iterator<const Char> String::cend() const noexcept
	{
		return Iterator<const Char>(m_DataPointer + m_UsedMemory, this);
	}

	String::ReverseIterator<Char> String::rbegin() const noexcept
	{
		return ReverseIterator<Char>(m_DataPointer + Length(), this);
	}

	String::ReverseIterator<Char> String::rend() const noexcept
	{
		return ReverseIterator<Char>(m_DataPointer - 1, this);
	}

	String::ReverseIterator<const Char> String::crbegin() const noexcept
	{
		return ReverseIterator<const Char>(m_DataPointer + Length(), this);
	}

	String::ReverseIterator<const Char> String::crend() const noexcept
	{
		return ReverseIterator<const Char>(m_DataPointer - 1, this);
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
		if (characters > m_AllocatedSize)
		{
			Realloc(characters + 1);
		}
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

		return i + 1;
	}

	String::Iterator<const Char> String::Find(Char character, unsigned int occurrence) const noexcept
	{
		if (occurrence == 0) return cend();

		for (const auto& c : *this)
		{
			if (c == character)
			{
				--occurrence;
				if (occurrence == 0) return Iterator<const Char>(&c, this);
			}
		}

		return cend();
	}

	String::Iterator<const Char> String::FindAt(Char character, Iterator<Char> start, unsigned int occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != end())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return Iterator<const Char>(&(*start), this);
			}
			++start;
		}

		return cend();
	}

	String::Iterator<const Char> String::FindAt(Char character, Iterator<const Char> start, unsigned int occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != cend())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return Iterator<const Char>(&(*start), this);
			}
			++start;
		}

		return cend();
	}

	void String::Insert(Char character, unsigned int index) noexcept
	{
		UNIMPLEMENTED(void);
	}

	Char String::Erase(unsigned int index) noexcept
	{
		Char temp = m_DataPointer[index];

		for (unsigned int i = index; i < Length(); i++)
		{
			m_DataPointer[i] = m_DataPointer[i + 1];
		}

		return temp;
	}

	String String::Erase(unsigned int start, unsigned int end) noexcept
	{
		return String();
	}

	void String::Realloc(unsigned int sizeRequired)
	{
		if (m_AllocatedSize > sizeRequired)
		{
			return;
		}

		Memory::DeallocString(m_AllocatedSize * sizeof(Char));

		while (m_AllocatedSize < sizeRequired) (m_AllocatedSize *= 2) += 1;

		auto newPointer = new Char[m_AllocatedSize];
		memcpy(newPointer, m_DataPointer, m_UsedMemory * sizeof(Char));
		delete[] m_DataPointer;
		Memory::AllocString(m_AllocatedSize * sizeof(Char));

		m_DataPointer = newPointer;
	}
}
