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
	ModuleParser(ArgParser& parser, BuildTree& tree);

private:
	void RebuildModules();
	void RecreateModule(Module& module);
	void Clean();
	void InitTup();

	void CreateTupfile(std::ostream& file, Module& module);

	BuildTree& m_Tree;
	bool m_Rebuild = false;
	bool m_Clean = false;
	std::filesystem::path m_ModuleCache;
	std::filesystem::path m_TupPath;
	std::filesystem::path m_BinariesPath;

	using json = nlohmann::json;
};
