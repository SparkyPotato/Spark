// Copyright 2020 SparkyPotato

/// \file
/// Tree of all source files and modules in the project

#pragma once

#include "Globals.h"

/// Represents a single file in te source-tree.
struct File
{
	File() = default;
	File(const fs::path& path)
	{
		std::string cleanPath = path.string();
		std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');
		Path = cleanPath;

		WriteTime = fs::last_write_time(Path).time_since_epoch().count();
	}

	fs::path Path;
	uint64_t WriteTime = 0;

	explicit operator String()
	{
		return fs::absolute(Path).string();
	}
};

/// Represents a header file,
/// and holds a list of all files that depend on it.
struct HeaderFile : File
{
	HeaderFile() = default;

	HeaderFile(const fs::path& path)
		: File(path)
	{}

	std::vector<String> DependedOn;
};

/// Represents a folder in the source-tree.
/// Contains all the files and folders in it.
struct Folder
{
	Folder() = default;

	Folder(const fs::path& path)
	{
		std::string cleanPath = path.string();
		std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');
		Path = cleanPath;

		WriteTime = fs::last_write_time(Path).time_since_epoch().count();
	}

	fs::path Path;

	std::vector<HeaderFile> HeaderFiles;
	std::vector<File> SourceFiles;
	std::vector<Folder> Subfolders;

	uint64_t WriteTime = 0;
};

/// Representation of a build module.
/// Submodules ARE NOT supported, and will never be,
/// as they just don't make sense in the Spark architecture.
struct Module
{
	Module() = default;

	Module(const fs::path& location)
		: Location(location)
	{
		Definition = File(Location.Path.string() + "/Module.json");
	}

	// Name of the module, derived from the folder name and name given in the
	// module definition. They both must match or SparkBuild WILL fail.
	String Name;

	// Semantic version (see https://semver.org).
	String Version;

	std::vector<String> Dependencies;

	bool Executable = false;

	File Definition;

	// Folder that contains the module definition file and source code.
	Folder Location;
};

using SourcePair = std::pair<Module*, File*>;
using HeaderPair = std::pair<Module*, HeaderFile*>;

/// The entire source-tree of the project.
class SourceTree
{
public:
	SourceTree() = default;

	/// Get all the modules discovered.
	///
	/// \return The list of modules.
	std::vector<Module>& GetModules() { return m_Modules; }

	/// Get all the source files discovered.
	///
	/// \return The list of source files.
	std::vector<SourcePair>& GetSources() { return m_Sources; }

	/// Get all the header files discovered.
	///
	/// \return The list of header files.
	std::vector<HeaderPair>& GetHeaders() { return m_Headers; }

	/// Get all the modules marked for recompilation.
	///
	/// \return The list of modules.
	std::vector<Module*>& GetDirtyModules() { return m_DirtyModules; }

	/// Get all the source files marked for recompilation.
	///
	/// \return The list of source files.
	std::vector<SourcePair>& GetDirtySourceFiles() { return m_DirtySourceFiles; }

	/// Get all the header files marked for recompilation.
	///
	/// \return The list of header files.
	std::vector<HeaderPair>& GetDirtyHeaders() { return m_DirtyHeaders; }

	/// Mark a source file for recompilation.
	///
	/// \param buildModule The module the file belongs to.
	/// \param source The source file.
	void AddDirtySource(Module* buildModule, File* source);

	/// Generate a source-tree from a directory.
	///
	/// \return The generated source-tree.
	static SourceTree* GenerateFromDirectory();

	/// Generate a source-tree from the build cache.
	///
	/// \return The generated source-tree.
	static SourceTree* GenerateFromCache();

	/// Save the source-tree to the build cache.
	///
	/// \param tree The tree to save.
	static void SaveToCache(const SourceTree& tree);

	/// Compares the source tree with the old tree, 
	/// and ensures that changes are marked dirty.
	/// 
	/// \param oldTree The tree generated from the previous compilation's build cache.
	void CompareWithOld(const SourceTree& oldTree);

	/// Generate all required directories.
	void GenerateDirectories();

private:
	friend void to_json(json& j, const SourceTree& tree);
	friend void from_json(const json& j, SourceTree& tree);

	/// Find all modules within a path.
	///
	/// \param folder The folder to search.
	void FindModules(const fs::path& folder);

	/// Populate a folder with source and header files, as well as sub-folders.
	/// Recursively populates subfolders.
	///
	/// \param Folder the populate.
	void PopulateFolder(Folder& folder);

	/// Add the folder into the global list of source files and header files.
	///
	/// \param buildModule The module the folder belongs to.
	/// \param folder The folder to push into the vector.
	void Vectorize(Module& buildModule, Folder& folder);

	/// Compare two folders and mark changes dirty.
	///
	/// \param buildModule The module the folders belong to.
	/// \param newFolder The new folder, generated from the directory.
	/// \param oldFolder The old folder, generated from the build cache.
	void CompareFolders(Module& buildModule, Folder& newFolder, const Folder& oldFolder);

	std::vector<Module> m_Modules;

	std::vector<SourcePair> m_Sources;
	std::vector<HeaderPair> m_Headers;

	std::vector<Module*> m_DirtyModules;
	std::vector<SourcePair> m_DirtySourceFiles;
	std::vector<HeaderPair> m_DirtyHeaders;
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

inline bool operator==(const File& first, const File& second) { std::error_code ec; return fs::equivalent(first.Path, second.Path, ec); }
inline bool operator==(const Module& first, const Module& second) { return first.Definition == second.Definition; }
inline bool operator==(const Folder& first, const Folder& second) { std::error_code ec; return fs::equivalent(first.Path, second.Path, ec); }
