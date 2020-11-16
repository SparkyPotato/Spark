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
	void CheckHeaders();
	void Compile();
	void AddHeaderDependencies();
	void Link();
	void CopyDependencies();

private:
	void ParseModule(Module& buildModule);

	void DirtyFolder(Module& buildModule, Folder& folder);

	void CopyModuleDependencies(String& moduleName, fs::path location);

	SourceTree& m_Tree;

	std::map<Module*, std::vector<File*>> m_CompileList;
	std::vector<Module*> m_Executables;
};
