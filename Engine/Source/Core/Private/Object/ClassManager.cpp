#include "Core/Object/ClassManager.h"

#include "Module/Module.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogClassManager, Verbose);

	ClassManager* GClassManager = nullptr;

	ClassManager::ClassManager()
	{
		auto& obj = m_ParentNode.Children.Emplace(STRING("Object"), true);
		obj.Parent = ArrayPtr<Class>(&m_ParentNode.Children, 0);
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

	ArrayPtr<Class> ClassManager::GetClass(const String& name)
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

		node->AddChild(name, isAbstract);
	}

	Class& ClassManager::GetBase()
	{
		return m_ParentNode.Children[0];
	}

	ArrayPtr<Class> ClassManager::SearchNode(const String& className, Class* node)
	{
		for (uint i = 0; i < node->Children.Size(); i++)
		{
			if (node->Children[i].Name == className)
			{
				return ArrayPtr<Class>(&node->Children, i);
			}

			return SearchNode(className, &node->Children[i]);
		}

		return ArrayPtr<Class>();
	}

	Class& Class::AddChild(String name, bool isAbstract)
	{
		auto& c = Children.Emplace(name, isAbstract);

		if (this->Parent)
		{
			uint index = 0;
			for (; index < this->Parent->Children.Size(); index++)
			{
				if (this->Parent->Children[index].Name == this->Name) break;
			}

			c.Parent = ArrayPtr<Class>(&this->Parent->Children, index);
		}

		return c;
	}
}
