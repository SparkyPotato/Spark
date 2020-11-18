// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#include "SourceTree.h"

#include "Error.h"

void SourceTree::AddDirtySource(Module* buildModule, File* source)
{
	if (std::find(m_DirtySourceFiles.begin(), m_DirtySourceFiles.end(), SourcePair(buildModule, source))
		!= m_DirtySourceFiles.end()) { return; }

	m_DirtySourceFiles.emplace_back(buildModule, source);
}

SourceTree* SourceTree::GenerateFromDirectory()
{
	auto tree = new SourceTree;

	fs::path projectDir = CommandLine::GetProperty("dir");

	Verbose("Generating SourceTree for: ", projectDir, ".");

	if (!fs::exists(Globals::SourcePath))
	{
		Error("Project does not contain a Source folder!");
	}
	tree->FindModules(Globals::SourcePath);

	if (CommandLine::GetSwitch("verbose"))
	{
		Verbose(false, "");
		Verbose("Modules located in:");
		for (auto& buildModule : tree->m_Modules)
		{
			Verbose(false, buildModule.Location.Path.string());
		}
		Verbose(false, "");
	}

	// Would use 'module' here but it's a reserved keyword in C++20
	for (auto& buildModule : tree->m_Modules)
	{
		tree->PopulateFolder(buildModule.Location);
	}

	for (auto& buildModule : tree->m_Modules)
	{
		tree->Vectorize(buildModule, buildModule.Location);
	}

	return tree;
}

SourceTree* SourceTree::GenerateFromCache()
{
	auto tree = new SourceTree;
	*tree = Globals::BuildCache.get<SourceTree>();
	
	for (auto& buildModule : tree->m_Modules)
	{
		tree->Vectorize(buildModule, buildModule.Location);
	}

	return tree;
}

void SourceTree::SaveToCache(const SourceTree& tree)
{
	Globals::BuildCache = tree;
}

void SourceTree::CompareWithOld(const SourceTree& oldTree)
{
	const std::vector<Module>& oldModules = oldTree.m_Modules;
	for (auto& buildModule : m_Modules)
	{
		// Comparing module definitions
		auto oldModule = std::find(oldModules.begin(), oldModules.end(), buildModule);
		if (oldModule != oldModules.end()) // Both modules exist
		{
			// Module definition was changed
			if (oldModule->Definition.WriteTime != buildModule.Definition.WriteTime) 
			{
				m_DirtyModules.emplace_back(&buildModule);
				continue; // If a module is dirty, we're going to recompile it anyways
			}
			else
			{
				buildModule.Name = oldModule->Name;
				buildModule.Version = oldModule->Version;
				buildModule.Dependencies = oldModule->Dependencies;
				buildModule.Executable = oldModule->Executable;
			}
		}
		else
		{
			m_DirtyModules.emplace_back(&buildModule);
			continue;
		}

		CompareFolders(buildModule, buildModule.Location, oldModule->Location);
	}
}

void SourceTree::GenerateDirectories()
{
	for (auto& buildModule : m_Modules)
	{
		std::string path = Globals::IntermediatePath.string() + "/" +
			CommandLine::GetProperty("config") + "/" + buildModule.Name;
		if (!fs::exists(path)) 
		{
			fs::create_directories(path);
		}

		path = Globals::BinariesPath.string() + "/" +
			CommandLine::GetProperty("config") + "/" + buildModule.Name;
		if (!fs::exists(path))
		{
			fs::create_directories(path);
		}

		if (buildModule.Executable)
		{
			if (!fs::exists(path + "/Executable"))
			{
				fs::create_directories(path + "/Executable");
			}
		}
	}

	std::string path = Globals::IntermediatePath.string() + "/DependencyGraph";
	if (!fs::exists(path)) { fs::create_directories(path); }
}

void SourceTree::CompareFolders(Module& buildModule, Folder& newFolder, const Folder& oldFolder)
{
	// If both folders have the same write time, no modifications have taken place
	// Easy way to skive off some work and save time
	// if (newFolder.WriteTime == oldFolder.WriteTime) return;

	// Compare headers
	for (auto& newHeader : newFolder.HeaderFiles)
	{
		auto oldHeader = std::find(oldFolder.HeaderFiles.begin(), oldFolder.HeaderFiles.end(), newHeader);
		if (oldHeader != oldFolder.HeaderFiles.end())
		{
			if (oldHeader->WriteTime != newHeader.WriteTime)
			{
				m_DirtyHeaders.emplace_back(&buildModule, &newHeader);
				newHeader.DependedOn = oldHeader->DependedOn;
			}
			// If not dirty we just skip it
		}
		else
		{
			m_DirtyHeaders.emplace_back(&buildModule, &newHeader);
		}
	}

	// Compare source files
	for (auto& newSource : newFolder.SourceFiles)
	{
		auto oldSource = std::find(oldFolder.SourceFiles.begin(), oldFolder.SourceFiles.end(), newSource);
		if (oldSource != oldFolder.SourceFiles.end())
		{
			if (oldSource->WriteTime != newSource.WriteTime)
			{
				m_DirtySourceFiles.emplace_back(&buildModule, &newSource);
			}
		}
		else
		{
			m_DirtySourceFiles.emplace_back(&buildModule, &newSource);
		}
	}

	for (auto& newSub : newFolder.Subfolders)
	{
		auto oldSub = std::find(oldFolder.Subfolders.begin(), oldFolder.Subfolders.end(), newSub);
		if (oldSub != oldFolder.Subfolders.end())
		{
			// If the old version of the subfolder exists, compare them
			CompareFolders(buildModule, newSub, *oldSub);
		}
	}
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

void SourceTree::Vectorize(Module& buildModule, Folder& folder)
{
	for (auto& source : folder.SourceFiles)
	{
		m_Sources.emplace_back(&buildModule, &source);
	}

	for (auto& header : folder.HeaderFiles)
	{
		m_Headers.emplace_back(&buildModule, &header);
	}

	for (auto& sub : folder.Subfolders)
	{
		Vectorize(buildModule, sub);
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
	j["Executable"] = buildModule.Executable;
	j["Definition"] = buildModule.Definition;
	j["Location"] = buildModule.Location;
}

void from_json(const json& j, Module& buildModule)
{
	buildModule.Name = j["Name"];
	buildModule.Version = j["Version"];
	buildModule.Dependencies = j["Dependencies"].get<std::vector<String>>();
	buildModule.Executable = j["Executable"];
	buildModule.Definition = j["Definition"];
	buildModule.Location = j["Location"];
}

void to_json(json& j, const Folder& folder)
{
	j["Path"] = fs::absolute(folder.Path).string();
	j["Headers"] = folder.HeaderFiles;
	j["Source"] = folder.SourceFiles;
	j["Subfolders"] = folder.Subfolders;
	j["WriteTime"] = folder.WriteTime;
}

void from_json(const json& j, Folder& folder)
{
	folder.Path = j["Path"].get<String>();
	folder.HeaderFiles = j["Headers"].get<std::vector<HeaderFile>>();
	folder.SourceFiles = j["Source"].get<std::vector<File>>();
	folder.Subfolders = j["Subfolders"].get<std::vector<Folder>>();
	folder.WriteTime = j["WriteTime"];
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
