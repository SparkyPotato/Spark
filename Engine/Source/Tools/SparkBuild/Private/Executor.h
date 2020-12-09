// Copyright 2020 SparkyPotato

/// \file
/// Executes all necessary operations on a source tree

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
	void GenerateFiles();
	void Link();
	void CopyDependencies();

private:
	void ParseModule(Module& buildModule);

	void DirtyFolder(Module& buildModule, Folder& folder);

	void CopyModule(String& moduleName, fs::path location);

	void DiscoverDependencies(const String& buildModule, std::vector<String>& dependencies);

	SourceTree& m_Tree;

	std::map<Module*, std::vector<File*>> m_CompileList;

	// We abuse the fact that only one key can exist in a map,
	// and checking is fast
	std::map<Module*, int> m_ModulesToRelink;
	std::vector<std::thread> m_LinkThreads;
};
