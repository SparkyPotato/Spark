#pragma once
#include "Core/Object/Object.h"
#include "Core/Types/Pointer.h"

namespace Spark
{
	struct ClassNode
	{
		ClassNode() = default;

		ArrayPtr<ClassNode> parent;

		Class node;
		Array<ClassNode> children;

		ClassNode& AddChild(const Class& c)
		{
			auto& cn = children.Emplace();
			cn.node = c;
			
			if (this->parent)
			{
				uint index = 0;
				for (; index < this->parent->children.Size(); index++)
				{
					if (this->parent->children[index].node == this->node) break;
				}

				cn.parent = ArrayPtr<ClassNode>(&this->parent->children, index);
			}

			return cn;
		}
	};

	class ClassManager
	{
	public:
		ClassManager();

		static void Initialize();
		static void Shutdown();

		ArrayPtr<ClassNode> GetClass(const String& name);
		void RegisterClass(const String& name, const String& parent, bool isAbstract);

		Class& GetBase();

	private:
		ArrayPtr<ClassNode> SearchNode(const String& className, ClassNode* node);

		ClassNode m_ParentNode;
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
	static ArrayPtr<ClassNode> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return classNode->node; \
} \
static const Class& GetStaticClass() \
{ \
	static ArrayPtr<ClassNode> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return classNode->node; \
} \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, false); }

#define REGISTER_CLASS_ABS(name, parent) \
private: \
inline static const String m_ClassName = STRING(#name); \
inline static const String m_ParentName = STRING(#parent); \
public: \
const Class& GetClass() override \
{ \
	static ArrayPtr<ClassNode> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return classNode->node; \
} \
static const Class& GetStaticClass() \
{ \
	static ArrayPtr<ClassNode> classNode; \
	if (!classNode) { classNode = GClassManager->GetClass(m_ClassName); } \
	return classNode->node; \
} \
static void RegisterClass() { GClassManager->RegisterClass(m_ClassName, m_ParentName, true); }
