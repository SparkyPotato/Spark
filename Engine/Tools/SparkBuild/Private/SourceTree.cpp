// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#include "SourceTree.h"

#include <regex>

#include "Error.h"

SourceTree SourceTree::GenerateFromDirectory()
{
	SourceTree tree;

	fs::path projectDir = CommandLine::GetProperty("dir");

	Verbose("Generating SourceTree for: ", projectDir, ".");

	if (!fs::exists(Globals::SourcePath))
	{
		Error("Project does not contain a Source folder!");
	}
	tree.FindModules(Globals::SourcePath);

	fs::path dependencyPath = projectDir.string() + "/Dependencies";
	if (fs::exists(dependencyPath))
	{
		Verbose("Found project dependencies.");
		tree.FindModules(dependencyPath);
	}

	fs::path toolPath = projectDir.string() + "/Tools";
	if (fs::exists(toolPath))
	{
		Verbose("Found project tools.");
		tree.FindModules(toolPath);
	}

	if (CommandLine::GetSwitch("verbose"))
	{
		Verbose(false, "");
		Verbose("Modules located in:");
		for (auto& buildModule : tree.m_Modules)
		{
			Verbose(false, buildModule.Location.Path.string());
		}
		Verbose(false, "");
	}

	// Would use 'module' here but it's a reserved keyword in C++20
	for (auto& buildModule : tree.m_Modules)
	{
		tree.PopulateFolder(buildModule.Location);
	}

	return tree;
}

SourceTree SourceTree::GenerateFromCache()
{
	return Globals::BuildCache.get<SourceTree>();
}

void SourceTree::SaveToCache(const SourceTree& tree)
{
	Globals::BuildCache = tree;
}

void SourceTree::CompareWithOld(const SourceTree& oldTree)
{
	for (auto& buildModule : oldTree.m_Modules)
	{
		auto it = std::find(m_Modules.begin(), m_Modules.end(), buildModule);
		if (it != m_Modules.end() && it->Definition.WriteTime == buildModule.Definition.WriteTime)
		{
			it->Definition.Dirty = false;
		}
	}
}

void SourceTree::ParseModule(Module& buildModule)
{
	Verbose("Parsing modules");

	// Semver regex - https://regex101.com/r/vkijKf/1/
	static std::regex versionRegex(R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-])"
		R"([0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)");

	json parser;

	// Get the name of the folder containing the module definition
	std::string folderName = buildModule.Location.Path.string();
	folderName = folderName.substr(folderName.rfind('/') + 1);
	//                                  Skips the last '/'-^

	// Check if the Module.json definition file exists
	std::string definitionPath = buildModule.Location.Path.string() + "/Module.json";
	if (!fs::exists(definitionPath))
	{
		Error("Module '", folderName, "' has no module definition.");
	}

	// Catch any parse exceptions
	try { std::ifstream(definitionPath) >> parser; }
	catch (...) { Error("Failed to parse module '", folderName, "'"); }

	// Parse Name and report any mismatches between folder name and module name
	buildModule.Name = parser["Name"];
	if (buildModule.Name.empty()) { Error("Module '", folderName, "' has no Name specifier."); }
	if (buildModule.Name != folderName) { Error("Module '", buildModule.Name, "' does not match with it's folder name."); }

	// Parse Version
	buildModule.Version = parser["Version"];
	if (buildModule.Version.empty()) { Error("Module '", buildModule.Name, "' does not have a version specifier"); }
	if (!std::regex_match(buildModule.Version, versionRegex)) 
	{
		Error("Module '", buildModule.Name, "' does not have a valid version specifier. See https://semver.org.");
	}

	// Parse Dependencies
	try { buildModule.Dependencies = parser.at("Dependencies").get<std::vector<String>>(); }
	catch (...) { Warning("Module '", buildModule.Name, "' has no dependencies specifier, assuming none."); }

	Globals::ModuleRegistry[buildModule.Name] =
	{
		{ "Version", buildModule.Version },
		{ "Path", buildModule.Location.Path.string() }
	};
}

void SourceTree::FindModules(const fs::path& folder)
{
	// Check if the folder contains a module definition
	if (fs::exists(folder.string() + "/Module.json"))
	{
		m_Modules.emplace_back(folder);
		return;
	}

	// If it doesn't, recursively iterate over sub-folders to find all modules
	for (auto& entry : fs::directory_iterator(folder))
	{
		if (entry.is_directory()) { FindModules(entry.path()); }
	}
}

void SourceTree::PopulateFolder(Folder& folder)
{
	for (auto& entry : fs::directory_iterator(folder.Path))
	{
		std::string extension = entry.path().extension().string();

		// SparkBuild only supports .cpp, .cc, and .cxx files
		bool isSource = extension == ".cpp" || extension == ".cc" || extension == ".cxx";
		if (entry.is_regular_file() && isSource) 
		{
			folder.SourceFiles.emplace_back(entry.path());
		}

		bool isHeader = extension == ".h" || extension == ".hpp";
		if (entry.is_regular_file() && isHeader) 
		{
			folder.HeaderFiles.emplace_back(entry.path());
		}

		if (entry.is_directory())
		{
			auto& subfolder = folder.Subfolders.emplace_back(entry.path());
			PopulateFolder(subfolder); // Recursively populate sub-folders
		}
	}
}

void to_json(json& j, const SourceTree& tree)
{
	j = tree.m_Modules;
}

void from_json(const json& j, SourceTree& tree)
{
	tree.m_Modules = j.get<std::vector<Module>>();
}

void to_json(json& j, const Module& buildModule)
{
	j["Name"] = buildModule.Name;
	j["Version"] = buildModule.Version;
	j["Dependencies"] = buildModule.Dependencies;
	j["Definition"] = buildModule.Definition;
	j["Location"] = buildModule.Location;
}

void from_json(const json& j, Module& buildModule)
{
	buildModule.Name = j["Name"];
	buildModule.Version = j["Version"];
	buildModule.Dependencies = j["Dependencies"].get<std::vector<String>>();
	buildModule.Definition = j["Definition"];
	buildModule.Location = j["Location"];
}

void to_json(json& j, const Folder& folder)
{
	j["Path"] = fs::absolute(folder.Path).string();
	j["Headers"] = folder.HeaderFiles;
	j["Source"] = folder.SourceFiles;
	j["Subfolders"] = folder.Subfolders;
}

void from_json(const json& j, Folder& folder)
{
	folder.Path = j["Path"].get<String>();
	folder.HeaderFiles = j["Headers"].get<std::vector<HeaderFile>>();
	folder.SourceFiles = j["Source"].get<std::vector<File>>();
	folder.Subfolders = j["Subfolders"].get<std::vector<Folder>>();
}

void to_json(json& j, const File& file)
{
	j["Path"] = fs::absolute(file.Path).string();
	j["WriteTime"] = file.WriteTime;
}

void from_json(const json& j, File& file)
{
	file.Path = j["Path"].get<String>();
	file.WriteTime = j["WriteTime"];
}

void to_json(json& j, const HeaderFile& file)
{
	j["Path"] = fs::absolute(file.Path).string();
	j["WriteTime"] = file.WriteTime;
	j["DependedOn"] = file.DependedOn;
}

void from_json(const json& j, HeaderFile& file)
{
	file.Path = j["Path"].get<String>();
	file.WriteTime = j["WriteTime"];
	file.DependedOn = j["DependedOn"].get<std::vector<String>>();
}
