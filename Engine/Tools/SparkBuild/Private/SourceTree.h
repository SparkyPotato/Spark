// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#pragma once

#include "Globals.h"

struct Folder
{
	fs::path Path;

	std::vector<fs::path> HeaderFiles;
	std::vector<fs::path> SourceFiles;
	std::vector<Folder> Subfolders;

	Folder(const fs::path& path)
		: Path(path)
	{}
};

// Representation of a build module.
// Submodules ARE NOT supported, and will never be,
// as they just don't make sense in the Spark architecture.
struct Module
{
	// Name of the module, derived from the folder name and name given in the
	// module definition. They both must match or SparkBuild WILL fail.
	String Name;

	// Semantic version (see https://semver.org).
	String Version;

	// Folder that contains the module definition file and source code.
	Folder Location;

	Module(const fs::path& location)
		: Location(location)
	{}
};

class SourceTree
{
public:
	SourceTree();

	std::vector<Module>& GetModules() { return m_Modules; }

private:
	// Find all modules within a path
	void FindModules(const fs::path& folder);

	// Populate a folder with source and header files, as well as sub-folders
	void PopulateFolder(Folder& folder);

	std::vector<Module> m_Modules;
	int m_SourceCount = 0, m_HeaderCount = 0, m_DirectoryCount = 0;
};
