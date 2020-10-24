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
			m_SharedRef = other.m_SharedRef;
			m_SharedRef->RefCount++;
		}

		~ObjPtr()
		{
			if (!m_SharedRef) return;

			m_SharedRef->RefCount--;

			if (m_SharedRef->RefCount <= 0)
			{
				Memory::RemoveSharedRef(m_SharedRef);
			}
		}

		ObjPtr<Obj>& operator=(const ObjPtr<Obj>& other)
		{
			if (m_SharedRef)
			{
				m_SharedRef->RefCount--;
			}

			if (m_SharedRef->RefCount <= 0)
			{
				Memory::RemoveSharedRef(m_SharedRef);
			}

			m_SharedRef = other.m_SharedRef;
			m_SharedRef->RefCount++;

			return *this;
		}
		
		Obj& operator*()
		{
			return *(reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject));
		}

		const Obj& operator*() const
		{
			return *(reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject));
		}

		Obj* operator->()
		{
			return reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject);
		}

		const Obj* operator->() const
		{
			return reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject);
		}

		Obj* Get()
		{
			return reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject);
		}

		const Obj* Get() const
		{
			return reinterpret_cast<Obj*>(m_SharedRef->AllocatedObject);
		}

		operator bool() const 
		{
			return m_SharedRef;
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
		ArrayPtr<Memory::SharedRef, RawAllocator> m_SharedRef;
	};

	template<class Type>
	ObjPtr<Type> Create()
	{
		if (Type::GetClass().IsAbstract) { return ObjPtr<Type>(); }

		ObjPtr<Type> temp;
		temp.m_SharedRef = Memory::AddSharedRef();
		temp.m_SharedRef->AllocatedObject = snew Type();
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

			temp.m_SharedRef = Memory::AddSharedRef();
			temp.m_SharedRef->AllocatedObject = reinterpret_cast<Cast*>(snew Type);
			temp.m_SharedRef->RefCount = 1;

			return temp;
		}
		else
		{
			return ObjPtr<Cast>();
		}
	}

	template<typename Type, typename Alloc>
	class ArrayPtr
	{
	public:
		ArrayPtr() = default;

		ArrayPtr(Array<Type, Alloc>* array, uint index)
			: m_Array(array), m_ObjectIndex(index)
		{
			if (m_Array) m_Array->RegisterPointer(this);
		}

		ArrayPtr(const ArrayPtr<Type, Alloc>& other)
			: m_Array(other.m_Array), m_ObjectIndex(other.m_ObjectIndex)
		{
			if (m_Array) m_Array->RegisterPointer(this);
		}

		~ArrayPtr()
		{
			if (m_Array) m_Array->DeregisterPointer(this);
		}

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

		template<typename Type, typename Alloc>
		friend bool operator==(ArrayPtr<Type, Alloc> first, ArrayPtr<Type, Alloc> second);

		template<typename Type, typename Alloc>
		friend bool operator!=(ArrayPtr<Type, Alloc> first, ArrayPtr<Type, Alloc> second);

	private:
		template<typename Type, typename Allocator = HeapAllocator>
		friend class Array;

		Array<Type, Alloc>* m_Array = nullptr;
		uint m_ObjectIndex = 0;
	};

	template<typename Type, typename Alloc>
	bool operator==(ArrayPtr<Type, Alloc> first, ArrayPtr<Type, Alloc> second)
	{
		return (first.m_Array == second.m_Array) && (first.m_ObjectIndex == second.m_ObjectIndex);
	}

	template<typename Type, typename Alloc>
	bool operator!=(ArrayPtr<Type, Alloc> first, ArrayPtr<Type, Alloc> second)
	{
		return !(first == second);
	}
}
