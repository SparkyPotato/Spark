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

	BuildTree& m_Tree;
	bool m_Rebuild = false;

	using json = nlohmann::json;
};
