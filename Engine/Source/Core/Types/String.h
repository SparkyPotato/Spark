#pragma once
#include "Platform/PlatformInterface.h" // Character encoding is platform specific and also defines string macros

#include <string>

namespace Spark
{
	// Stores strings internally in Unicode
	class String
	{
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
		String& operator--() noexcept;

		// Append and assign operators
		String& operator+=(Char append) noexcept;
		String& operator+=(const String& append) noexcept;
		String& operator+=(const Char* append) noexcept;

		Char& operator[](unsigned int at) noexcept;

		// Conversion operators
		operator const Char*() const;
		operator std::wstring() const;

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

	private:
		void Realloc(unsigned int sizeRequired);

		Char* m_DataPointer = nullptr;
		unsigned int m_UsedMemory = 0;
		unsigned int m_AllocatedSize = 1;
	};
}
