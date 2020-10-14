#pragma once
#include "Core/Object/Object.h"

namespace Spark
{
	struct ClassNode
	{
		ClassNode() = default;

		ClassNode* parent;

		Class node;
		Array<ClassNode> children;

		void AddChild(const Class& c)
		{
			auto& cn = children.Emplace();
			cn.node = c;
			cn.parent = this;
		}
	};

	class ClassManager
	{
	public:
		ClassManager();

		static void Initialize();
		static void Shutdown();

		Class* GetClass(const String& name);
		void RegisterClass(const String& name, const String& parent, bool isAbstract);

		Class* GetBase();

	private:
		ClassNode* SearchNode(const String& className, ClassNode* node);

		ClassNode m_ObjectNode;
	};

	extern ClassManager* GClassManager;
}

#define REGISTER_CLASS(name, parent) \
private: \
inline static const String m_ClassName = STRING(#name); \
inline static const String m_ParentName = STRING(#parent); \
public: \
const Class* GetClass() override { return GClassManager->GetClass(m_ClassName); } \
static const Class* GetStaticClass() { return GClassManager->GetClass(m_ClassName); } \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, false); }

#define REGISTER_CLASS_ABS(name, parent) \
private: \
inline static const String m_ClassName = STRING(#name); \
inline static const String m_ParentName = STRING(#parent); \
public: \
const Class* GetClass() override { return GClassManager->GetClass(m_ClassName); } \
static const Class* GetStaticClass() { return GClassManager->GetClass(m_ClassName); } \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, true); }
