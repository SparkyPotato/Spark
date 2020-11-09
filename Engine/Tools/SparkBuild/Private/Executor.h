// SparkBuild.Private.Executor
// Copyright 2020 SparkyPotato

// Executes all necessary operations on a source tree

#pragma once

#include <thread>

#include "BasePlatform.h"
#include "SourceTree.h"

class Executor
{
public:
	Executor(SourceTree& tree);

	void Parse();
	void Compile();

private:
	void ParseModule(Module& buildModule);

	void DirtyFolder(Folder& folder);

	void SpawnCompilationThread(File* source);
	static void ThreadWorker(File* source);

	SourceTree& m_Tree;
	std::vector<std::thread> m_CompilationThreads;
};
