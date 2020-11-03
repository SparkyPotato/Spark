/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#pragma once

#include "ArgParse.h"
#include "BuildTree.h"

#include "json.hpp"

class ModuleParser
{
public:
	ModuleParser(ArgParser& parser, BuildTree& tree, std::filesystem::path startPath);

private:
	using json = nlohmann::json;

	void RebuildModules();

	void ParseModule(Module& module, json& moduleDef);

	void Clean();

	void InitTup();
	void CreateTupfile(Module& module, std::filesystem::path path);
	void CreateTupfiles(Module& module, std::filesystem::path moduleRoot);

	BuildTree& m_Tree;
	ArgParser& m_Parser;
	bool m_Rebuild = false;
	std::filesystem::path m_ModuleCache;
	std::filesystem::path m_TupPath;
	std::filesystem::path m_BinariesPath;
	std::filesystem::path m_ExecPath;
};
