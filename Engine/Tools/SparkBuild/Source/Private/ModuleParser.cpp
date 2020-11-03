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

	if (!std::filesystem::exists(L"Engine/Registry/")) { std::filesystem::create_directory(L"Engine/Registry"); }

	if (std::filesystem::exists(L"Engine/Registry/ModuleRegistry.json"))
	{
		std::ifstream(L"Engine/Registry/ModuleRegistry.json") >> m_ModuleRegistry;
	}

	LoadModuleCache();
	RebuildModules();
}

ModuleParser::~ModuleParser()
{
	SaveModuleCache();

	std::ofstream(L"Engine/Registry/ModuleRegistry.json") << std::setw(4) << m_ModuleRegistry << std::endl;
}

void ModuleParser::RebuildModules()
{
	Clean();
	m_Generator.InitTup();

	int reparseCount = 0;
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

		std::string modulePath = std::filesystem::absolute(module.DefinitionPath).string();
		auto writeTime = std::filesystem::last_write_time(modulePath).time_since_epoch().count();

		ParseModule(module, moduleDef);

		if (m_ModuleCache[module.Name]["WriteTime"] < writeTime)
		{
			reparseCount++;
			m_ModuleCache[module.Name]["WriteTime"] = writeTime;
			module.DefChanged = true;

			std::replace(modulePath.begin(), modulePath.end(), '\\', '/');
			m_ModuleRegistry[module.Name]["Path"] = modulePath;
		}

		m_Generator.CreateTupfiles(module, module.DefinitionPath.parent_path());

		stream.close();
	}

	wprintf(L"%d module definitions changed \n", reparseCount);
}

void ModuleParser::ParseModule(Module& module, json& moduleDef)
{
	module.Name = moduleDef["Module"];
	if (module.Name.empty())
	{
		throw Error(L"INVALID_MODULE");
	}

	std::vector<std::string> version = moduleDef["Version"];

	if (version.size() < 3 || version.size() > 4) { throw Error(L"ILLEGAL_VERSION_FORMAT"); }

	module.Version.Major = std::stoul(version[0]);
	module.Version.Minor = std::stoul(version[1]);
	module.Version.Patch = std::stoul(version[2]);

	if (version.size() == 4) { module.Version.Prerelease = version[3]; }
}

void ModuleParser::Clean()
{
	if (m_Rebuild)
	{
		wprintf(L"Cleaning up Intermediate and Binaries. \n");
		if (std::filesystem::exists(m_Tree.IntermediatePath)) { std::filesystem::remove_all(m_Tree.IntermediatePath); }
		if (std::filesystem::exists(m_Tree.BinaryPath)) { std::filesystem::remove_all(m_Tree.BinaryPath); }
	}
}

void ModuleParser::LoadModuleCache()
{
	std::filesystem::path moduleCachePath = m_Tree.IntermediatePath.wstring() + L"../Cache/ModuleCache.json";

	if (!std::filesystem::exists(moduleCachePath.parent_path())) { std::filesystem::create_directory(moduleCachePath.parent_path()); }
	if (std::filesystem::exists(moduleCachePath)) { std::ifstream(moduleCachePath) >> m_ModuleCache; }
}

void ModuleParser::SaveModuleCache()
{
	std::filesystem::path moduleCachePath = m_Tree.IntermediatePath.wstring() + L"../Cache/ModuleCache.json";

	std::ofstream(moduleCachePath, std::ofstream::trunc) << std::setw(4) << m_ModuleCache;
}
