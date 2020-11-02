/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#include "ModuleParser.h"

#include "Error.h"
#include <fstream>

#include <Windows.h>

ModuleParser::ModuleParser(ArgParser& parser, BuildTree& tree, std::wstring startPath)
	: m_Tree(tree), m_ExecPath(startPath)
{
	m_Rebuild = parser.GetSwitch(L"rebuild");
	m_Clean = parser.GetSwitch(L"clean");

	std::wstring intermediate = m_Tree.IntermediatePath;
	std::wstring binaries = m_Tree.BinaryPath;

	m_ModuleCache = intermediate + L"/Build/ModuleCache.json";
	m_TupPath = intermediate + L"/../.tup";
	m_BinariesPath = binaries + L"/Build/";

	RebuildModules();
}

void ModuleParser::RebuildModules()
{
	Clean();
	if (m_Clean) return;

	bool isIncrementalBuild = std::filesystem::exists(m_ModuleCache) && !(m_Clean || m_Rebuild);

	json moduleCacheIn;
	json moduleCacheOut;
	if (isIncrementalBuild)
	{
		std::ifstream cache(m_ModuleCache);
		cache >> moduleCacheIn;
		cache.close();

		std::filesystem::remove(m_ModuleCache);
	}

	auto writeTime = std::filesystem::last_write_time(m_ExecPath).time_since_epoch().count();
	if (!isIncrementalBuild || moduleCacheIn["SparkBuild"] < writeTime)
	{
		wprintf(L"SparkBuild was updated, rebuilding. \n");
		isIncrementalBuild = false;
		moduleCacheOut["SparkBuild"] = writeTime;
	}

	InitTup();

	for (auto& module : m_Tree.GetModules())
	{
		std::ifstream stream(module.DefinitionPath);
		json moduleDef;
		
		try
		{
			stream >> moduleDef;
		}
		catch (...)
		{
			throw Error(L"PARSE_FAIL: %s", module.DefinitionPath.c_str());
		}

		ParseModule(module, moduleDef);

		auto writeTime = std::filesystem::last_write_time(module.DefinitionPath).time_since_epoch().count();

		std::wstring modulePrivate = module.DefinitionPath.parent_path();
		CheckFolderStructure(module, modulePrivate, moduleCacheOut);

		stream.close();
	}

	std::ofstream cache(m_ModuleCache);
	cache << std::setw(4) << moduleCacheOut << std::endl;
	cache.close();
}

void ModuleParser::ParseModule(Module& module, nlohmann::json& moduleDef)
{
	try
	{
		module.Name = moduleDef.at("Module");
	}
	catch (...)
	{
		throw Error(L"UNNAMED_MODULE");
	}

	try
	{
		std::vector<std::string> version = moduleDef.at("Version");

		if (version.size() < 3 || version.size() > 4) { throw Error(L"ILLEGAL_VERSION_FORMAT"); }

		module.Version.Major = std::stoul(version[0]);
		module.Version.Minor = std::stoul(version[1]);
		module.Version.Patch = std::stoul(version[2]);

		if (version.size() == 4)
		{
			module.Version.Prerelease = version[3];
		}
	}
	catch (...)
	{
		throw Error(L"NO_MODULE_VERSION");
	}
}


void ModuleParser::CheckFolderStructure(const Module& module, const std::filesystem::path& path, nlohmann::json& moduleCache)
{
	if (!std::filesystem::exists(path)) { moduleCache[module.Name]["Directories"] = nullptr; return; }

	std::filesystem::directory_iterator privatePath(path);

	std::vector<json> directories;
	for (auto& entry : privatePath)
	{
		if (entry.is_directory()) 
		{
			Folder folder(entry.path().string(), GetSubfolders(entry.path()));
			directories.emplace_back(folder); 
		}
	}

	moduleCache[module.Name]["Directories"] = directories;
}

std::vector<ModuleParser::Folder> ModuleParser::GetSubfolders(const std::filesystem::path path)
{
	if (!std::filesystem::exists(path)) return std::vector<ModuleParser::Folder>();

	std::filesystem::directory_iterator it(path);

	std::vector<ModuleParser::Folder> directories;
	for (auto& entry : it)
	{
		if (entry.is_directory())
		{
			directories.emplace_back(entry.path().string(), GetSubfolders(entry.path()));
		}
	}

	return directories;
}

void ModuleParser::Clean()
{
	if (m_Rebuild || m_Clean)
	{
		wprintf(L"Cleaning up Intermediate and Binaries. \n");
		if (std::filesystem::exists(m_ModuleCache)) { std::filesystem::remove_all(m_ModuleCache.parent_path()); }
		if (std::filesystem::exists(m_TupPath)) { std::filesystem::remove_all(m_TupPath); }
		if (std::filesystem::exists(m_BinariesPath)) { std::filesystem::remove_all(m_BinariesPath); }

		wprintf(L"Cleaning up Build files. \n");
		std::filesystem::recursive_directory_iterator it(m_Tree.SourcePath);

		for (auto& entry : it)
		{
			if (entry.path().filename() == L"Tupfile") std::filesystem::remove_all(entry);
		}
	}
}

void ModuleParser::InitTup()
{
	if (!std::filesystem::exists(m_TupPath))
	{
		std::wstring intermediate = m_Tree.IntermediatePath;
		intermediate += L"/../";

		wchar_t directory[10000];
		GetCurrentDirectoryW(1000, directory);

		SetCurrentDirectoryW(intermediate.c_str());
		system("tup init");

		SetFileAttributesW(m_TupPath.c_str(), FILE_ATTRIBUTE_HIDDEN);

		SetCurrentDirectoryW(directory);
	}
}

void ModuleParser::CreateTupfile(std::ostream& file, Module& module)
{
	std::string filePath = std::filesystem::absolute(m_Tree.IntermediatePath).string() + "/Build/" + module.Name + "/";
	std::replace(filePath.begin(), filePath.end(), '\\', '/');

	file << ": foreach *.cpp |> cl /c %f /Fo\"%o\" |> " << filePath << "%B.o";
}

void from_json(const nlohmann::json& j, ModuleParser::Folder& folder)
{
	j.at("Name").get_to(folder.Name);
	j.at("Subdirectories").get_to(folder.Subfolders);
}

void to_json(nlohmann::json& j, const ModuleParser::Folder& folder)
{
	j["Name"] = folder.Name;
	j["Subdirectories"] = folder.Subfolders;
}

ModuleParser::Folder::Folder(std::string name, const std::vector<Folder> subfolders)
	: Name(name), Subfolders(subfolders)
{
	std::filesystem::path tupfilePath = name + "/Tupfile";
	if (std::filesystem::exists(tupfilePath)) return;

	std::ofstream tupfile(tupfilePath);

	// CreateTupfile(tupfile, module);

	tupfile << std::endl;
	tupfile.close();
	SetFileAttributesW(tupfilePath.c_str(), FILE_ATTRIBUTE_HIDDEN);
}
