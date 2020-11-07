// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#pragma once

#include "Globals.h"

struct Folder
{
	fs::path Path;

	std::vector<fs::path> Files;
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
	// Name of the module, derived from the folder name
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

private:
	void FindModules(const fs::path& folder);

	std::vector<Module> m_Modules;
};
