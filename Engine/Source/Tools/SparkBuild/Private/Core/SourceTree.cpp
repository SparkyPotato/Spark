// Copyright 2020 SparkyPotato

#include "Core/SourceTree.h"

#include "Hash/md5.h"
#include "json/json.hpp"

using json = nlohmann::json;

SourceTree::SourceTree()
{
	fs::path sourcePath = Global::ProjectPath.string() + "/Source";
	if (!fs::exists(sourcePath)) { throw std::exception("Project does not have a Source folder!"); }
	int modules = FindModules(sourcePath);

	fs::path dependencyPath = Global::ProjectPath.string() + "/Dependencies";
	if (fs::exists(dependencyPath)) { modules += FindModules(dependencyPath); }

	Console("Parsed ", modules, " modules.");
}

int SourceTree::FindModules(const fs::path& folder)
{
	MD5 hasher;
	int found = 0;

	// Check if the folder has a module
	fs::path definition = folder.string() + "/Module.json";
	if (fs::exists(definition))
	{
		std::string parent = folder.parent_path().string();
		std::string name = folder.string().substr(parent.size() + 1);
		
		Module& m = Global::Registry[name]; // Relying on default construction if it doesn't exist
		if (fs::last_write_time(definition).time_since_epoch().count() > m.WriteTime)
		{
			std::stringstream buffer;
			buffer << std::ifstream(definition).rdbuf();
			hasher(buffer.str());
			if (hasher.getHash() != m.Hash)
			{
				ParseModule(definition, buffer.str(), m, hasher.getHash());
				found++;
			}
		}
	}

	// If it doesn't, recursively iterate over sub-folders to find all modules
	for (auto& entry : fs::directory_iterator(folder))
	{
		if (entry.is_directory()) { found += FindModules(entry.path()); }
	}

	return found;
}

void SourceTree::ParseModule(const fs::path& path, const std::string text, Module& m, const std::string& hash)
{
	json j = json::parse(text);
	m.Definition = path.string();
	m.Dependencies = j["Dependencies"].get<std::vector<std::string>>();
	m.Executable = j["Executable"].get<bool>();
	m.Hash = hash;
	m.Version = j["Version"].get<std::string>();
	m.WriteTime = fs::last_write_time(path).time_since_epoch().count();
}
