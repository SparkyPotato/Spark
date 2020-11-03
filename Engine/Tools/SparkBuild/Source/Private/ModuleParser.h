/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#pragma once

#include "ArgParse.h"
#include "BuildTree.h"

#include "json.hpp"
#include "TupGenerator.h"

class ModuleParser
{
public:
	ModuleParser(ArgParser& parser, BuildTree& tree, std::filesystem::path startPath);
	~ModuleParser();

private:
	using json = nlohmann::json;

	void RebuildModules();
	void ParseModule(Module& module, json& moduleDef);
	void Clean();

	void LoadModuleCache();
	void SaveModuleCache();

	TupGenerator m_Generator;

	BuildTree& m_Tree;
	ArgParser& m_Parser;
	bool m_Rebuild = false;
	json m_ModuleCache;
	std::filesystem::path m_ExecPath;

	json m_ModuleRegistry;
};
