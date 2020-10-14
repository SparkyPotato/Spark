#pragma once

namespace Spark
{
	template<typename Type>
	class Ptr
	{
	public:
		Ptr()
		{

		}

		~Ptr()
		{

		}

		Type& operator[](uint index)
		{
			return m_Pointer[index];
		}

		Type* Get() { return m_Pointer; }
		const Type* Get() const { return m_Pointer; }

	private:
		Type* m_Pointer;
	};
}
