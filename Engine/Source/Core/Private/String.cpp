#include "Core/Types/String.h"

namespace Spark
{
	String::String() noexcept
	{
		Realloc(11);
		m_DataPointer[0] = L'\0';
		m_UsedMemory = 1;
	}

	String::String(uint size) noexcept
	{
		Realloc(size + 1);
		m_DataPointer[0] = L'\0';
		m_UsedMemory = 1;
	}

	String::String(const Char* charArray) noexcept
	{
		// Allocate memory
		uint arraySize = GetCharPointerLength(charArray);

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

	String::String(const String& other, uint start, uint end) noexcept
	{
		SPARK_ASSERT(start <= end);

		Realloc(end - start + 2);
		m_UsedMemory = end - start + 2;

		memcpy(m_DataPointer, other.GetCharPointer() + start, (end - start  + 1) * sizeof(Char));

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

	String String::operator+(const Char* append) const noexcept
	{
		uint size = GetCharPointerLength(append);

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
		uint originalLength = Length();

		// Make sure we have enough space for both strings
		Realloc(originalLength + append.m_UsedMemory);
		m_UsedMemory = originalLength + append.m_UsedMemory;

		// Copy, including the null
		memcpy(m_DataPointer + originalLength, append.m_DataPointer, append.m_UsedMemory * sizeof(Char));

		return *this;
	}

	String& String::operator+=(const Char* append) noexcept
	{
		uint originalLength = Length();
		uint size = GetCharPointerLength(append);

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

	String String::operator--(int) noexcept
	{
		String string = *this;
		m_DataPointer[--m_UsedMemory] = L'\0';

		return string;
	}

	Char& String::operator[](uint offset) noexcept
	{
		SPARK_ASSERT(offset < m_UsedMemory);

		return m_DataPointer[offset];
	}

	String::operator const Char* () const
	{
		return m_DataPointer;
	}

	String::Iterator String::begin() const noexcept
	{
		return Iterator(m_DataPointer);
	}

	String::Iterator String::end() const noexcept
	{
		return Iterator(m_DataPointer + m_UsedMemory);
	}

	String::ConstIterator String::cbegin() const noexcept
	{
		return ConstIterator(m_DataPointer);
	}

	String::ConstIterator String::cend() const noexcept
	{
		return ConstIterator(m_DataPointer + m_UsedMemory);
	}

	String::ReverseIterator String::rbegin() const noexcept
	{
		return ReverseIterator(m_DataPointer + Length());
	}

	String::ReverseIterator String::rend() const noexcept
	{
		return ReverseIterator(m_DataPointer - 1);
	}

	String::ConstReverseIterator String::crbegin() const noexcept
	{
		return ConstReverseIterator(m_DataPointer + Length());
	}

	String::ConstReverseIterator String::crend() const noexcept
	{
		return ConstReverseIterator(m_DataPointer - 1);
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

	String::ConstIterator String::Find(Char character, uint occurrence) const noexcept
	{
		if (occurrence == 0) return cend();

		for (const auto& c : *this)
		{
			if (c == character)
			{
				--occurrence;
				if (occurrence == 0) return ConstIterator(&c);
			}
		}

		return cend();
	}

	String::ConstIterator String::FindAt(Char character, Iterator start, uint occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != end())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return ConstIterator(&(*start));
			}
			++start;
		}

		return cend();
	}

	String::ConstIterator String::FindAt(Char character, ConstIterator start, uint occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != cend())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return ConstIterator(&(*start));
			}
			++start;
		}

		return cend();
	}

	String::ConstIterator String::FindAt(Char character, ReverseIterator start, uint occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != rend())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return ConstIterator(&(*start));
			}
			++start;
		}

		return cend();
	}

	String::ConstIterator String::FindAt(Char character, ConstReverseIterator start, uint occurence) const noexcept
	{
		if (occurence == 0) return cend();

		while (start != crend())
		{
			if (*start == character)
			{
				--occurence;
				if (occurence == 0) return ConstIterator(&(*start));
			}
			++start;
		}

		return cend();
	}

	void String::Insert(Char character, uint index) noexcept
	{
		UNIMPLEMENTED(void);
	}

	Char String::Erase(uint index) noexcept
	{
		Char temp = m_DataPointer[index];

		for (uint i = index; i < Length(); i++)
		{
			m_DataPointer[i] = m_DataPointer[i + 1];
		}

		return temp;
	}

	String String::Erase(uint start, uint end) noexcept
	{
		return String();
	}

	String& String::Reverse() noexcept
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

		auto newPointer = new Char[m_AllocatedSize];
		memcpy(newPointer, m_DataPointer, m_UsedMemory * sizeof(Char));
		delete[] m_DataPointer;

		m_DataPointer = newPointer;
	}

	String::Iterator::Iterator(Char* pointer) noexcept 
		: m_Pointer(pointer)
	{}

	String::Iterator String::Iterator::operator+(uint offset) noexcept
	{
		return Iterator(m_Pointer + offset);
	}

	String::Iterator String::Iterator::operator-(uint offset) noexcept
	{
		return Iterator(m_Pointer - offset);
	}

	String::Iterator String::Iterator::operator++(int) noexcept
	{

		auto self = *this;
		++m_Pointer;
		return self;
	}

	String::Iterator& String::Iterator::operator++() noexcept
	{
		++m_Pointer;
		return *this;
	}

	String::Iterator String::Iterator::operator--(int) noexcept
	{

		auto self = *this;
		--m_Pointer;
		return self;
	}

	String::Iterator& String::Iterator::operator--() noexcept
	{

		--m_Pointer;
		return *this;
	}

	Char& String::Iterator::operator[](uint offset)
	{
		return m_Pointer[offset];
	}

	Char& String::Iterator::operator*() noexcept
	{
		return *m_Pointer;
	}

	Char* String::Iterator::operator->() noexcept
	{
		return m_Pointer;
	}

	String::ConstIterator::ConstIterator(const Char* pointer) noexcept
		: m_Pointer(pointer)
	{}

	String::ConstIterator String::ConstIterator::operator+(uint offset) noexcept
	{
		return ConstIterator(m_Pointer + offset);
	}

	String::ConstIterator String::ConstIterator::operator-(uint offset) noexcept
	{
		return ConstIterator(m_Pointer - offset);
	}

	String::ConstIterator String::ConstIterator::operator++(int) noexcept
	{
		auto self = *this;
		++m_Pointer;
		return self;
	}

	String::ConstIterator& String::ConstIterator::operator++() noexcept
	{
		++m_Pointer;
		return *this;
	}

	String::ConstIterator String::ConstIterator::operator--(int) noexcept
	{
		auto self = *this;
		--m_Pointer;
		return self;
	}

	String::ConstIterator& String::ConstIterator::operator--() noexcept
	{
		--m_Pointer;
		return *this;
	}

	const Char& String::ConstIterator::operator[](uint offset)
	{
		return m_Pointer[offset];
	}

	const Char& String::ConstIterator::operator*() noexcept
	{
		return *m_Pointer;
	}

	const Char* String::ConstIterator::operator->() noexcept
	{
		return m_Pointer;
	}

	String::ReverseIterator::ReverseIterator(Char* pointer) noexcept
		: m_Pointer(pointer)
	{}

	String::ReverseIterator String::ReverseIterator::operator+(uint offset) noexcept
	{
		return ReverseIterator(m_Pointer - offset);
	}

	String::ReverseIterator String::ReverseIterator::operator-(uint offset) noexcept
	{
		return ReverseIterator(m_Pointer + offset);
	}

	String::ReverseIterator String::ReverseIterator::operator++(int) noexcept
	{
		auto self = *this;
		--m_Pointer;
		return self;
	}

	String::ReverseIterator& String::ReverseIterator::operator++() noexcept
	{
		--m_Pointer;
		return *this;
	}

	String::ReverseIterator String::ReverseIterator::operator--(int) noexcept
	{
		auto self = *this;
		++m_Pointer;
		return self;
	}

	String::ReverseIterator& String::ReverseIterator::operator--() noexcept
	{
		++m_Pointer;
		return *this;
	}

	Char& String::ReverseIterator::operator[](uint offset)
	{
		return m_Pointer[offset];
	}

	Char& String::ReverseIterator::operator*() noexcept
	{
		return *m_Pointer;
	}

	Char* String::ReverseIterator::operator->() noexcept
	{
		return m_Pointer;
	}

	String::ConstReverseIterator::ConstReverseIterator(const Char* pointer) noexcept
		: m_Pointer(pointer)
	{}

	String::ConstReverseIterator String::ConstReverseIterator::operator+(uint offset) noexcept
	{
		return ConstReverseIterator(m_Pointer - offset);
	}

	String::ConstReverseIterator String::ConstReverseIterator::operator-(uint offset) noexcept
	{
		return ConstReverseIterator(m_Pointer + offset);
	}

	String::ConstReverseIterator String::ConstReverseIterator::operator++(int) noexcept
	{
		auto self = *this;
		--m_Pointer;
		return self;
	}

	String::ConstReverseIterator& String::ConstReverseIterator::operator++() noexcept
	{
		--m_Pointer;
		return *this;
	}

	String::ConstReverseIterator String::ConstReverseIterator::operator--(int) noexcept
	{
		auto self = *this;
		++m_Pointer;
		return self;
	}

	String::ConstReverseIterator& String::ConstReverseIterator::operator--() noexcept
	{
		++m_Pointer;
		return *this;
	}

	const Char& String::ConstReverseIterator::operator[](uint offset)
	{
		return m_Pointer[offset];
	}

	const Char& String::ConstReverseIterator::operator*() noexcept
	{
		return *m_Pointer;
	}

	const Char* String::ConstReverseIterator::operator->() noexcept
	{
		return m_Pointer;
	}

	const String& StringStream::GetString()
	{
		return m_InternalString;
	}

	StringStream& StringStream::operator<<(Char character) noexcept
	{
		m_InternalString += character;

		return *this;
	}

	StringStream& StringStream::operator<<(const String& string) noexcept
	{
		m_InternalString += string;

		return *this;
	}

	StringStream& StringStream::operator<<(int integer) noexcept
	{
		int i = 0, n = integer;
		while (n != 0)
		{
			i++;
			n /= 10;
		}

		Char* temp = new Char[i + 2];
		swprintf_s(temp, i + 2, STRING("%d"), integer);

		m_InternalString += temp;

		delete[] temp;

		return *this;
	}

	StringStream& StringStream::operator<<(float decimal) noexcept
	{
		return *this;
	}

	StringStream& StringStream::operator<<(double decimal) noexcept
	{
		return *this;
	}

	String operator+(const Char* cstr, const String& string)
	{
		return String(cstr) + string;
	}

	bool operator==(const String::Iterator& first, const String::Iterator& second) noexcept
	{
		return first.m_Pointer == second.m_Pointer;
	}

	bool operator!=(const String::Iterator& first, const String::Iterator& second) noexcept
	{
		return !(first == second);
	}

	bool operator==(const String::ConstIterator& first, const String::ConstIterator& second) noexcept
	{
		return first.m_Pointer == second.m_Pointer;
	}

	bool operator!=(const String::ConstIterator& first, const String::ConstIterator& second) noexcept
	{
		return !(first == second);
	}

	bool operator==(const String::ReverseIterator& first, const String::ReverseIterator& second) noexcept
	{
		return first.m_Pointer == second.m_Pointer;
	}

	bool operator!=(const String::ReverseIterator& first, const String::ReverseIterator& second) noexcept
	{
		return !(first == second);
	}

	bool operator==(const String::ConstReverseIterator& first, const String::ConstReverseIterator& second) noexcept
	{
		return first.m_Pointer == second.m_Pointer;
	}

	bool operator!=(const String::ConstReverseIterator& first, const String::ConstReverseIterator& second) noexcept
	{
		return !(first == second);
	}
}
