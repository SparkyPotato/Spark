// SparkBuild.Private.Executor
// Copyright 2020 SparkyPotato

// Executes all necessary operations on a source tree

#pragma once

#include "BasePlatform.h"
#include "SourceTree.h"

class Executor
{
public:
	Executor(SourceTree& tree);

	void ExecuteParses();

private:
	SourceTree& m_Tree;
};
