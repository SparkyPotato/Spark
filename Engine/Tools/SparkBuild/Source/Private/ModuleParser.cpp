/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#include "ModuleParser.h"

#include "Error.h"
#include <fstream>

#include <Windows.h>

ModuleParser::ModuleParser(ArgParser& parser, BuildTree& tree)
	: m_Tree(tree)
{
	m_Rebuild = parser.GetSwitch(L"rebuild");
	m_Clean = parser.GetSwitch(L"clean");

	std::wstring intermediate = m_Tree.IntermediatePath;
	std::wstring binaries = m_Tree.BinaryPath;

	m_ModuleCache = intermediate + L"/Build/Modules.json";
	m_TupPath = intermediate + L"/../.tup";
	m_BinariesPath = binaries + L"/Build/";

	RebuildModules();
}

void ModuleParser::RebuildModules()
{
	Clean();
	if (m_Clean) return;

	int changeCount = 0;

	bool isIncrementalBuild = std::filesystem::exists(m_ModuleCache);

	json moduleCache;
	if (isIncrementalBuild)
	{
		std::ifstream cache(m_ModuleCache);
		cache >> moduleCache;
		cache.close();
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

		auto writeTime = std::filesystem::last_write_time(module.DefinitionPath).time_since_epoch().count();
		if (!isIncrementalBuild || moduleCache[module.Name] < writeTime)
		{
			changeCount++;
			RecreateModule(module);
			moduleCache[module.Name] = writeTime;
		}

		stream.close();
	}

	std::ofstream cache(m_ModuleCache);
	cache << std::setw(4) << moduleCache << std::endl;
	cache.close();

	wprintf(L"%d module definition/s changed. \n", changeCount);
}

void ModuleParser::RecreateModule(Module& module)
{
	std::wstring modulePrivate = module.DefinitionPath.parent_path();
	modulePrivate += L"/Private/";

	if (!std::filesystem::exists(modulePrivate))
	{
		printf("Warning: Module '%s' has no Private folder, skipping.\n", module.Name.c_str());
		return;
	}

	std::wstring tupfilePath = modulePrivate + L"Tupfile";
	if (std::filesystem::exists(tupfilePath)) { std::filesystem::remove(tupfilePath); }
	std::ofstream tupfile(tupfilePath);

	CreateTupfile(tupfile, module);

	tupfile << std::endl;
	tupfile.close();
	SetFileAttributesW(tupfilePath.c_str(), FILE_ATTRIBUTE_HIDDEN);
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

		SetCurrentDirectoryW(intermediate.c_str());
		system("tup init");

		SetFileAttributesW(m_TupPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
	}
}

void ModuleParser::CreateTupfile(std::ostream& file, Module& module)
{
	std::string filePath = std::filesystem::absolute(m_Tree.IntermediatePath).string() + "/Build/" + module.Name + "/";
	std::replace(filePath.begin(), filePath.end(), '\\', '/');

	file << ": foreach *.cpp |> cl /c %f /Fo\"%o\" |> " << filePath << "%B.o";
}
