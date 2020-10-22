// Copyright 2020 SparkyPotato

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
			if (!m_SharedRef) return;

			m_SharedRef->RefCount--;

			if (m_SharedRef->RefCount <= 0)
			{
				sdelete m_Object;
				sdelete m_SharedRef;
			}
		}

		ObjPtr<Obj>& operator=(const ObjPtr<Obj>& other)
		{
			if (m_SharedRef)
			{
				m_SharedRef->RefCount--;

				if (m_SharedRef->RefCount <= 0)
				{
					sdelete m_Object;
					sdelete m_SharedRef;
				}
			}

			m_Object = other.m_Object;
			m_SharedRef = other.m_SharedRef;
			m_SharedRef->RefCount++;

			return *this;
		}
		
		Obj& operator*()
		{
			return *m_Object;
		}

		const Obj& operator*() const
		{
			return *m_Object;
		}

		Obj* operator->()
		{
			return m_Object;
		}

		const Obj* operator->() const
		{
			return m_Object;
		}

		Obj* Get()
		{
			return m_Object;
		}

		const Obj* Get() const
		{
			return m_Object;
		}

		operator bool() const 
		{
			return m_Object;
		}

		template<class Type>
		friend ObjPtr<Type> Create();

		template<class Cast, class Type>
		friend ObjPtr<Cast> Create();

		template<class To, class From>
		friend ObjPtr<To> Cast(ObjPtr<From> cast);

		template<class To, class From>
		friend ObjPtr<To> UnsafeCast(ObjPtr<From> cast);

	private:
		Obj* m_Object = nullptr;
		Memory::SharedRef* m_SharedRef = nullptr;
	};

	template<class Type>
	ObjPtr<Type> Create()
	{
		if (Type::GetClass().IsAbstract) { return ObjPtr<Type>(); }

		ObjPtr<Type> temp;
		temp.m_SharedRef = snew Memory::SharedRef;
		temp.m_SharedRef->AllocatedObject = temp.m_Object = snew Type();
		temp.m_SharedRef->RefCount = 1;

		return temp;
	}

	template<class Cast, class Type>
	ObjPtr<Cast> Create()
	{
		if (Type::GetClass().IsAbstract) { return ObjPtr<Cast>(); }

		if (Cast::GetClass().IsSubclassOf<Type>() || Type::GetClass().IsSubclassOf<Cast>())
		{
			ObjPtr<Cast> temp;

			temp.m_SharedRef = snew Memory::SharedRef;
			temp.m_SharedRef->AllocatedObject = temp.m_Object = reinterpret_cast<Cast*>(snew Type);
			temp.m_SharedRef->RefCount = 1;

			return temp;
		}
		else
		{
			return ObjPtr<Cast>();
		}
	}

	template<typename Type>
	class ArrayPtr
	{
	public:
		ArrayPtr() = default;

		ArrayPtr(Array<Type>* array, uint index)
			: m_Array(array), m_ObjectIndex(index)
		{}

		Type* operator->()
		{
			return &(*m_Array)[m_ObjectIndex];
		}

		const Type* operator->() const
		{
			return &(*m_Array)[m_ObjectIndex];
		}

		Type& operator*()
		{
			return (*m_Array)[m_ObjectIndex];
		}

		const Type& operator*() const
		{
			return (*m_Array)[m_ObjectIndex];
		}

		Type* Get()
		{
			return &(*m_Array)[m_ObjectIndex];
		}

		const Type* Get() const
		{
			return &(*m_Array)[m_ObjectIndex];
		}

		operator bool() const
		{
			return m_Array;
		}

	private:
		Array<Type>* m_Array = nullptr;
		uint m_ObjectIndex = 0;
	};
}
