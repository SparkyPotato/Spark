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

		ArrayPtr<Class> Parent;
		Array<Class> Children;

		String Name;
		bool IsAbstract = false;

	private:
		friend class ClassManager;

		Class& AddChild(String name, bool isAbstract);
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

		Class m_ParentNode;
	};

	extern ClassManager* GClassManager;
}

#define REGISTER_CLASS(name, parent) \
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
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, false); }

#define REGISTER_CLASS_ABS(name, parent) \
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
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, true); }
