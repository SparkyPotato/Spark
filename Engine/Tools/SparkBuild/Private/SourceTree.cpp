// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#include "SourceTree.h"

#include "Error.h"

SourceTree::SourceTree()
{
	fs::path projectDir = CommandLine::GetProperty("dir");

	Verbose("Generating SourceTree for: ", projectDir, ".");

	fs::path sourcePath = projectDir.string() + "/Source/";
	if (!fs::exists(sourcePath))
	{
		Error("Project does not contain a Source folder!");
	}
	FindModules(sourcePath);

	fs::path dependencyPath = projectDir.string() + "/Dependencies";
	if (fs::exists(dependencyPath))
	{
		Verbose("Found project dependencies.");
		FindModules(dependencyPath);
	}

	fs::path toolPath = projectDir.string() + "/Tools";
	if (fs::exists(toolPath))
	{
		Verbose("Found project tools.");
		FindModules(toolPath);
	}
}

void SourceTree::FindModules(const fs::path& folder)
{
	// Check if the folder contains a module definition
	if (fs::exists(folder.string() + "/Module.json"))
	{
		// Cleanup backslashes in the path (Windows annoying)
		std::string cleanPath = folder.string();
		std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

		m_Modules.emplace_back(cleanPath);
		return;
	}

	// If it doesn't, recursively iterate over subfolders to find all modules
	for (auto& entry : fs::directory_iterator(folder))
	{
		if (entry.is_directory()) { FindModules(entry.path()); }
	}
}
