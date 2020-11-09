// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#pragma once

#include "Globals.h"

struct File
{
	File() = default;
	File(const fs::path& path)
		: Path(path)
	{
		WriteTime = fs::last_write_time(Path).time_since_epoch().count();
	}

	bool Dirty = true;
	fs::path Path;
	uint64_t WriteTime;
};

struct HeaderFile : File
{
	HeaderFile() = default;

	HeaderFile(const fs::path& path)
		: File(path)
	{}

	std::vector<String> DependedOn;
};

struct Folder
{
	Folder() = default;

	Folder(const fs::path& path)
		: Path(path)
	{}

	fs::path Path;

	std::vector<HeaderFile> HeaderFiles;
	std::vector<File> SourceFiles;
	std::vector<Folder> Subfolders;
};

// Representation of a build module.
// Submodules ARE NOT supported, and will never be,
// as they just don't make sense in the Spark architecture.
struct Module
{
	Module() = default;

	Module(const fs::path& location)
		: Location(location), Definition(location.string() + "/Module.json")
	{}

	// Name of the module, derived from the folder name and name given in the
	// module definition. They both must match or SparkBuild WILL fail.
	String Name;

	// Semantic version (see https://semver.org).
	String Version;

	std::vector<String> Dependencies;

	File Definition;

	// Folder that contains the module definition file and source code.
	Folder Location;
};

class SourceTree
{
public:
	SourceTree() = default;

	std::vector<Module>& GetModules() { return m_Modules; }

	static SourceTree GenerateFromDirectory();
	static SourceTree GenerateFromCache();
	static void SaveToCache(const SourceTree& tree);

	void CompareWithOld(const SourceTree& oldTree);

private:
	friend void to_json(json& j, const SourceTree& tree);
	friend void from_json(const json& j, SourceTree& tree);

	void SourceTree::ParseModule(Module& buildModule);

	// Find all modules within a path
	void FindModules(const fs::path& folder);

	// Populate a folder with source and header files, as well as sub-folders
	void PopulateFolder(Folder& folder);

	std::vector<Module> m_Modules;
	int m_SourceCount = 0, m_HeaderCount = 0, m_DirectoryCount = 0;
};

void to_json(json& j, const SourceTree& tree);
void from_json(const json& j, SourceTree& tree);

void to_json(json& j, const Module& buildModule);
void from_json(const json& j, Module& buildModule);

void to_json(json& j, const Folder& folder);
void from_json(const json& j, Folder& folder);

void to_json(json& j, const File& file);
void from_json(const json& j, File& file);

void to_json(json& j, const HeaderFile& file);
void from_json(const json& j, HeaderFile& file);

inline bool operator==(const File& first, const File& second) { return fs::equivalent(first.Path, second.Path); }
inline bool operator==(const Module& first, const Module& second) { return first.Definition == second.Definition; }
inline bool operator==(const Folder& first, const Folder& second) { return fs::equivalent(first.Path, second.Path); }