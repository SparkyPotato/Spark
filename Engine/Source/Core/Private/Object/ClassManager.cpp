// Copyright 2020 SparkyPotato

#include "Core/Object/ClassManager.h"

#include "Module/Module.h"

namespace Spark
{
	DEFINE_LOG_CATEGORY_FILE(LogClassManager, Verbose);

	ClassManager* GClassManager = nullptr;

	ClassManager::ClassManager()
	{
		// Create the base Object node
		m_ParentNodes.Emplace();
		auto& obj = m_ParentNodes[0].Children.Emplace(STRING("Object"), true);
		obj.Parent = ArrayPtr<Class>(&m_ParentNodes, 0);
	}

	void ClassManager::Initialize()
	{
		if (GClassManager)
		{
			SPARK_LOG(LogClassManager, Error, STRING("Class Manager already initialized!"));
			return;
		}

		GClassManager = snew ClassManager;

		SPARK_LOG(LogClassManager, Trace, STRING("Initialized Class Manager"));

		Module::RegisterClass(); // Register the Engine Module class
	}

	void ClassManager::Shutdown()
	{
		sdelete GClassManager;
		GClassManager = nullptr;

		SPARK_LOG(LogClassManager, Trace, STRING("Shutdown Class Manager"));
	}

	ArrayPtr<Class> ClassManager::GetClass(const String& name)
	{
		return SearchNode(name, &m_ParentNodes[0]);
	}

	void ClassManager::RegisterClass(const String& name, const String& parent, bool isAbstract)
	{
		auto node = SearchNode(parent, &m_ParentNodes[0]);

		if (!node)
		{
			SPARK_LOG(LogClassManager, Error, STRING("Parent class '%s' of class '%s' not found!"), parent.GetCharPointer(), name.GetCharPointer());
			return;
		}

		node->AddChild(name, isAbstract);

		SPARK_LOG(LogClassManager, Verbose, STRING("Registered class '%s' deriving from '%s'"), name.GetCharPointer(), parent.GetCharPointer());
	}

	Class& ClassManager::GetBase()
	{
		return m_ParentNodes[0].Children[0];
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

	Class& Class::AddChild(const String& name, bool isAbstract)
	{
		auto& c = Children.Emplace(name, isAbstract);

		if (this->Parent)
		{
			uint index = this->Parent->Children.Find(*this);

			c.Parent = ArrayPtr<Class>(&this->Parent->Children, index);
		}

		return c;
	}

	bool Class::IsSubclass(const Class& derived, const Class& base)
	{
		const Class* d = &derived;

		// Climb up the class tree checking if the parent exists
		while (auto p = d->Parent)
		{
			if (*p == base) return true;

			d = d->Parent.Get();
		}

		return false;
	}

	bool Class::IsSubclassOf(const Class& base) const
	{
		return IsSubclass(*this, base);
	}
}
