#include "Core/Object/ClassManager.h"

#include "Module/Module.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogClassManager, Verbose);

	ClassManager* GClassManager = nullptr;

	ClassManager::ClassManager()
	{
		auto& obj = m_ParentNode.children.Emplace();
		obj.node = Class(STRING("Object"), true);
		obj.parent = ArrayPtr<ClassNode>(&m_ParentNode.children, 0);
	}

	void ClassManager::Initialize()
	{
		if (GClassManager)
		{
			SPARK_LOG(LogClassManager, Error, STRING("Class Manager already initialized!"));
			return;
		}

		GClassManager = new ClassManager;

		Module::RegisterClass();
	}

	void ClassManager::Shutdown()
	{
		delete GClassManager;
		GClassManager = nullptr;
	}

	ArrayPtr<ClassNode> ClassManager::GetClass(const String& name)
	{
		return SearchNode(name, &m_ParentNode);
	}

	void ClassManager::RegisterClass(const String& name, const String& parent, bool isAbstract)
	{
		auto node = SearchNode(parent, &m_ParentNode);

		if (!node)
		{
			SPARK_LOG(LogClassManager, Error, STRING("Parent class '%s' of class '%s' not found!"), parent.GetCharPointer(), name.GetCharPointer());
			return;
		}

		node->AddChild(Class(name, isAbstract));
	}

	Class& ClassManager::GetBase()
	{
		return m_ParentNode.children[0].node;
	}

	ArrayPtr<ClassNode> ClassManager::SearchNode(const String& className, ClassNode* node)
	{
		for (uint i = 0; i < node->children.Size(); i++)
		{
			if (node->children[i].node.Name == className)
			{
				return ArrayPtr<ClassNode>(&node->children, i);
			}

			return SearchNode(className, &node->children[i]);
		}

		return ArrayPtr<ClassNode>();
	}
}
