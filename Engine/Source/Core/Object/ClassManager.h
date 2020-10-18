// Copyright 2020 SparkyPotato

#pragma once
#include "Core/Object/Object.h"

namespace Spark
{
	struct Class
	{
		Class() = default;
		Class(const String& name, bool isAbstract)
			: Name(name), IsAbstract(isAbstract)
		{}

		String Name; // Name of the class
		Array<Class> Children; // Array of all child classes
		bool IsAbstract = false;
		ArrayPtr<Class> Parent; // Pointer to the parent class

		static bool IsSubclass(const Class& derived, const Class& base);
		
		bool IsSubclassOf(const Class& base) const;

		template<class T>
		bool IsSubclassOf() const { return IsSubclass(*this, T::GetClass()); }

	private:
		friend class ClassManager;

		Class& AddChild(const String& name, bool isAbstract);
	};

	bool operator==(const Class& first, const Class& second);

	class ClassManager
	{
	public:
		ClassManager();

		static void Initialize();
		static void Shutdown();

		ArrayPtr<Class> GetClass(const String& name);
		void RegisterClass(const String& name, const String& parent, bool isAbstract);

		Class& GetBase();

	private:
		ArrayPtr<Class> SearchNode(const String& className, Class* node);

		Array<Class> m_ParentNodes;
	};

	extern ClassManager* GClassManager;

	// Check if the cast is possible, and return a null ObjPtr if not
	template<class To, class From>
	ObjPtr<To> Cast(ObjPtr<From> cast)
	{
		if (To::GetClass().IsSubclassOf<From>() || From::GetClass().IsSubclassOf<To>())
		{
			ObjPtr<To> temp;
			temp.m_Object = reinterpret_cast<To*>(cast.m_Object);
			temp.m_SharedRef = cast.m_SharedRef;
			temp.m_SharedRef->RefCount++;

			return temp;
		}

		return ObjPtr<To>();
	}

	// Unsafely cast - only do if you are sure the cast is possible
	template<class To, class From>
	ObjPtr<To> UnsafeCast(ObjPtr<From> cast)
	{
		ObjPtr<To> temp;
		temp.m_Object = reinterpret_cast<To*>(cast.m_Object);
		temp.m_SharedRef = cast.m_SharedRef;
		temp.m_SharedRef->RefCount++;

		return temp;
	}
}

#define REGISTER_CLASS(name, parent) REGISTER_CLASS_FULL(name, parent, false)

#define REGISTER_CLASS_FULL(name, parent, isAbstract) \
private: \
inline static const String m_ClassName = STRING(#name); \
inline static const String m_ParentName = STRING(#parent); \
public: \
static const Class& GetClass() \
{ \
	static ArrayPtr<Class> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return *classNode; \
} \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, isAbstract); }
