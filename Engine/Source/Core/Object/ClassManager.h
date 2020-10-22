// Copyright 2020 SparkyPotato

#pragma once
#include "Core/Object/Object.h"

DECLARE_LOG_CATEGORY(LogClassManager, Verbose);

namespace Spark
{
	struct Class
	{
		Class() = default;
		Class(const String& name, bool isAbstract, ObjPtr<Object>(*instantiate) ())
			: Name(name), IsAbstract(isAbstract), m_Instantiate(instantiate)
		{}

		String Name; // Name of the class
		Array<Class> Children; // Array of all child classes
		bool IsAbstract = false;
		ArrayPtr<Class> Parent; // Pointer to the parent class

		inline ObjPtr<Object> Instantiate() const { return m_Instantiate(); }

		template<class T>
		inline ObjPtr<T> Instantiate() const { return Cast<T>(m_Instantiate()); }

		static bool IsSubclass(const Class& derived, const Class& base);
		
		bool IsSubclassOf(const Class& base) const;

		template<class T>
		bool IsSubclassOf() const { return IsSubclass(*this, T::GetClass()); }

	private:
		friend class ClassManager;

		Class& AddChild(const String& name, bool isAbstract, ObjPtr<Object>(*instantiate) ());

		ObjPtr<Object>(*m_Instantiate) ();
	};

	bool operator==(const Class& first, const Class& second);

	class ClassManager
	{
	public:
		ClassManager();

		static void Initialize();
		static void Shutdown();

		ArrayPtr<Class> GetClass(const String& name);
		void RegClass(const String& name, const String& parent, bool isAbstract, ObjPtr<Object>(*instantiate) ());

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

	template<class Both>
	ObjPtr<Both> Cast(ObjPtr<Both> cast)
	{
		return cast;
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

	template<class T>
	void AddClass()
	{
		T::RegClass();
	}
}

#define REGISTER_CLASS(name, parent) REGISTER_CLASS_NORM(name, parent, false)

#define REGISTER_CLASS_NORM(name, parent, isAbstract) \
REGISTER_CLASS_NO_INS(name, parent, isAbstract) \
static Spark::ObjPtr<name> Instantiate() \
{ \
	if (isAbstract) { SPARK_LOG(LogClassManager, Error, STRING("Trying to instantiate abstract class!")); }\
	return Create<name>(); \
}

#define REGISTER_CLASS_NO_INS(name, parent, isAbstract) \
protected: \
using Super = parent; \
private: \
	inline static const Spark::String m_ClassName = STRING(#name); \
	inline static const Spark::String m_ParentName = STRING(#parent); \
public: \
	static const Spark::Class& GetClass() \
{ \
	static Spark::ArrayPtr<Class> classNode; \
	if (!classNode) { if (!(classNode = Spark::GClassManager->GetClass(m_ClassName))) \
		{ SPARK_LOG(LogClassManager, Fatal, STRING("Class '{}' has not been registered!"), m_ClassName); } } \
	return *classNode; \
} \
static void RegClass() { Spark::GClassManager->RegClass(m_ClassName, m_ParentName, isAbstract, &name::InstantiateBase); } \
static ObjPtr<Object> InstantiateBase() { return UnsafeCast<Object>(Instantiate()); }
