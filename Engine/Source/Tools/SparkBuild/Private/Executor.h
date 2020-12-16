// Copyright 2020 SparkyPotato

/// \file
/// Executes all necessary operations on a source tree

#pragma once

#include <thread>

#include "BasePlatform.h"
#include "SourceTree.h"

/// Does all the actual building. 
/// Takes care of deciding which files to recompile and which modules to relink.
/// Also does the compiling and redirecting by forwarding it to BasePlatform::Compile,
/// and BasePlatform::Link.
class Executor
{
public:
	Executor(SourceTree& tree);

	/// Parse module definitions that have changed, 
	/// and mark the entire module for recompilation.
	void Parse();

	/// Mark all source files depending on a changed header for recompilation.
	void CheckHeaders();

	/// Compile all source files which were marked previously.
	void Compile();

	/// Add header dependencies to each source file.
	void AddHeaderDependencies();

	/// Generate auto-generated files.
	void GenerateFiles();

	/// Link modules.
	void Link();

	/// Copy executable dependencies into the executable directory.
	void CopyDependencies();

private:
	/// Parse the module and update its data.
	///
	/// \param buildModule The module to be parsed.
	void ParseModule(Module& buildModule);

	/// Mark and entire folder to be recompiled.
	///
	/// \param buildModule The module to which the folder belongs to.
	/// \param folder The folder to dirty.
	void DirtyFolder(Module& buildModule, Folder& folder);

	/// Copy the module to the provided location, and recursively copy all dependencies.
	///
	/// \param moduleName Name of the module to copy.
	/// \param location The location where to copy the module binary to.
	void CopyModule(String& moduleName, fs::path location);

	/// Discover the dependencies of the module, and add them into the dependency list.
	///
	/// \param buildModule The name of the module.
	/// \param dependencies The dependency list to insert into.
	void DiscoverDependencies(const String& buildModule, std::vector<String>& dependencies);

	SourceTree& m_Tree;

	std::map<Module*, std::vector<File*>> m_CompileList;

	// We abuse the fact that only one key can exist in a map,
	// and checking is fast
	std::map<Module*, int> m_ModulesToRelink;
	std::vector<std::thread> m_LinkThreads;
};
