// SparkBuild.Private.Executor
// Copyright 2020 SparkyPotato

// Executes all necessary operations on a source tree

#include "Executor.h"

Executor::Executor(SourceTree& tree)
	: m_Tree(tree)
{}

void Executor::ExecuteParses()
{
	int reparses = 0;

	for (auto& buildModule : m_Tree.GetModules())
	{
		if (buildModule.Definition.Dirty)
		{
			reparses++;
			m_Tree.ParseModule(buildModule);
		}
	}

	BasePlatform::Output("Rebuilt ", reparses, " modules.");
}
