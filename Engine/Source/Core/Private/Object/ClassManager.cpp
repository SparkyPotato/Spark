#include "Core/Object/ClassManager.h"

#include "Module/Module.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogClassManager, Verbose);

	ClassManager* GClassManager = nullptr;

	ClassManager::ClassManager()
	{
		m_ObjectNode.node = Class(STRING("Object"), true);
		m_ObjectNode.parent = nullptr;
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

	Class* ClassManager::GetClass(const String& name)
	{
		return &SearchNode(name, &m_ObjectNode)->node;
	}

	void ClassManager::RegisterClass(const String& name, const String& parent, bool isAbstract)
	{
		auto node = SearchNode(parent, &m_ObjectNode);
		if (!node)
		{
			SPARK_LOG(LogClassManager, Error, STRING("Parent class '%s' of class '%s' not found!"), parent.GetCharPointer(), name.GetCharPointer());
			return;
		}

		node->AddChild(Class(name, isAbstract));
	}

	Class* ClassManager::GetBase()
	{
		return &m_ObjectNode.node;
	}

	ClassNode* ClassManager::SearchNode(const String& className, ClassNode* node)
	{
		if (node->node.Name == className) return node;

		for (auto& classNode : node->children)
		{
			if (classNode.node.Name == className) return &classNode;

			return SearchNode(className, &classNode);
		}
		return nullptr;
	}
}
