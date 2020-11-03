/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#include "ModuleParser.h"

#include "Error.h"

#include <fstream>

ModuleParser::ModuleParser(ArgParser& parser, BuildTree& tree, std::filesystem::path startPath)
	: m_Tree(tree), m_ExecPath(startPath), m_Parser(parser), m_Generator(parser, tree)
{
	m_Rebuild = parser.GetSwitch(L"rebuild");

	std::wstring intermediate = m_Tree.IntermediatePath;
	std::wstring binaries = m_Tree.BinaryPath;

	m_ModuleCache = intermediate + L"Build/ModuleCache.json";
	m_BinariesPath = binaries + L"Build/";

	RebuildModules();
}

void ModuleParser::RebuildModules()
{
	Clean();

	m_Generator.InitTup();

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

		m_Generator.CreateTupfiles(module, module.DefinitionPath.parent_path());

		stream.close();
	}
}

void ModuleParser::ParseModule(Module& module, json& moduleDef)
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

void ModuleParser::Clean()
{
	if (m_Rebuild)
	{
		wprintf(L"Cleaning up Intermediate and Binaries. \n");
		if (std::filesystem::exists(m_ModuleCache)) { std::filesystem::remove_all(m_ModuleCache.parent_path()); }
		if (std::filesystem::exists(m_BinariesPath)) { std::filesystem::remove_all(m_BinariesPath); }

		wprintf(L"Cleaning up Build files. \n");
		std::filesystem::recursive_directory_iterator it(m_Tree.SourcePath);

		for (auto& entry : it)
		{
			if (entry.path().filename() == L"Tupfile") std::filesystem::remove_all(entry);
		}
	}
}
