#pragma once

namespace Spark
{
	template<typename Obj>
	class ObjPtr
	{
	public:
		ObjPtr() = default;

		ObjPtr(const ObjPtr<Obj>& other)
		{
			m_Object = other.m_Object;
			m_SharedRef = other.m_SharedRef;
			m_SharedRef->RefCount++;
		}

		~ObjPtr()
		{
			m_SharedRef->RefCount--;

			if (m_SharedRef->RefCount <= 0)
			{
				delete m_Object;
				delete m_SharedRef;
			}
		}

		ObjPtr<Obj>& operator=(const ObjPtr<Obj>& other)
		{
			~ObjPtr();

			m_Object = other.m_Object;
			m_SharedRef = other.m_SharedRef;
			m_SharedRef->RefCount++;
		}
		
		Obj& operator*()
		{
			return *m_Object;
		}

		Obj* operator->()
		{
			return m_Object;
		}

		template<class Type>
		friend ObjPtr<Type> Create();

		template<class Cast, class Type>
		friend ObjPtr<Cast> Create();

	private:
		Obj* m_Object;
		Memory::Object* m_SharedRef;
	};

	template<class Type>
	ObjPtr<Type> Create()
	{
		static_assert(Type::GetStaticClass(), "Template needs to be an Object!");

		ObjPtr<Type> temp;
		temp.m_SharedRef = new Memory::Object;
		temp.m_SharedRef->AllocatedObject = temp.m_Object = new Type();
		temp.m_SharedRef->RefCount = 1;

		return temp;
	}

	template<class Cast, class Type>
	ObjPtr<Cast> Create()
	{
		ObjPtr<Cast> temp;
		temp.m_SharedRef = new Memory::Object;
		temp.m_SharedRef->AllocatedObject = temp.m_Object = new Type();
		temp.m_SharedRef->RefCount = 1;

		return temp;
	}
}
