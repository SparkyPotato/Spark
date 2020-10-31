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
	std::wstring intermediate = m_Tree.IntermediatePath;
	m_Rebuild = parser.GetSwitch(L"r");

	RebuildModules();
}

void ModuleParser::RebuildModules()
{
	int changeCount = 0;

	std::wstring intermediate = m_Tree.IntermediatePath;
	std::filesystem::path modulesPath = intermediate + L"/Build/Modules.json";
	std::wstring tup = m_Tree.SourcePath;
	tup += L"/.tup/";

	if (m_Rebuild && std::filesystem::exists(modulesPath))
	{
		std::filesystem::remove(modulesPath);
		std::filesystem::remove_all(tup);
	}

	bool haveModuleList = std::filesystem::exists(modulesPath);

	json moduleList;
	if (haveModuleList)
	{
		std::ifstream modules(modulesPath);
		modules >> moduleList;
	}

	if (!std::filesystem::exists(tup))
	{
		SetCurrentDirectoryW(m_Tree.SourcePath.c_str());
		system("tup init");

		SetFileAttributesW(tup.c_str(), FILE_ATTRIBUTE_HIDDEN);
	}

	for (auto& module : m_Tree.GetModules())
	{
		std::ifstream stream(module.DefinitionPath);
		json j;
		stream >> j;

		try
		{
			module.Name = j.at("Module");
		}
		catch (...)
		{
			throw Error(L"UNNAMED_MODULE");
		}

		try
		{
			std::vector<std::string> version = j.at("Version");

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
		if (moduleList[module.Name] < writeTime || !haveModuleList)
		{
			changeCount++;
			RecreateModule(module);
			moduleList[module.Name] = writeTime;
		}
	}

	std::ofstream modules(modulesPath);
	modules << std::setw(4) << moduleList << std::endl;

	wprintf(L"%d module definition/s changed. \n\n", changeCount);
}

void ModuleParser::RecreateModule(Module& module)
{
	std::wstring modulePath = module.DefinitionPath.parent_path();
	modulePath += L"/Private/";

	if (!std::filesystem::exists(modulePath))
	{
		printf("Warning: Module '%s' has no Private folder, creating.\n", module.Name.c_str());
		std::filesystem::create_directory(modulePath);
	}

	std::wstring tupPath = modulePath + L"Tupfile.ini";
	std::ofstream tupfile(tupPath);
	SetFileAttributesW(tupPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
}
