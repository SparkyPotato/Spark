#pragma once
#include "Core/Object/Object.h"
#include "Core/Types/Pointer.h"

namespace Spark
{
	struct Class
	{
		Class() = default;
		Class(const String& name, bool isAbstract)
			: Name(name), IsAbstract(isAbstract)
		{}

		String Name;
		Array<Class> Children;
		bool IsAbstract = false;
		ArrayPtr<Class> Parent;

		static bool IsSubclass(const Class& derived, const Class& base);
		
		bool IsSubclassOf(const Class& base) const;

		template<class T>
		bool IsSubclassOf() const { return IsSubclass(*this, T::GetStaticClass()); }

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

	template<class To, class From>
	To* Cast(From* cast)
	{
		if (To::GetStaticClass().IsSubclassOf<From>() || From::GetStaticClass().IsSubclassOf<To>())
		{
			return reinterpret_cast<To*>(cast);
		}

		return nullptr;
	}
}

#define REGISTER_CLASS(name, parent) REGISTER_CLASS_FULL(name, parent, false)

#define REGISTER_CLASS_FULL(name, parent, isAbstract) \
private: \
inline static const String m_ClassName = STRING(#name); \
inline static const String m_ParentName = STRING(#parent); \
public: \
const Class& GetClass() override \
{ \
	static ArrayPtr<Class> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return *classNode; \
} \
static const Class& GetStaticClass() \
{ \
	static ArrayPtr<Class> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return *classNode; \
} \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, isAbstract); }
