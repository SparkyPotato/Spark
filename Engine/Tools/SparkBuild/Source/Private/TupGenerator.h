/*
	SparkBuild.Private.TupGenerator
	Copyright 2020 SparkyPotato

	Tupfile and Tuprules generator
*/

#pragma once

#include "BuildTree.h"

#include "json.hpp"

class TupGenerator
{
public:
	TupGenerator(ArgParser& parser, BuildTree& tree);
	~TupGenerator();

	void InitTup();
	void CreateTupfile(Module& module, std::filesystem::path path);
	void CreateTupfiles(Module& module, std::filesystem::path moduleRoot);

private:
	using json = nlohmann::json;

	void LoadBuildConfig();
	void SetRules(json buildConfig);

	void LoadBuildCache();
	void SaveBuildCache();

	BuildTree& m_Tree;
	ArgParser& m_Parser;

	std::filesystem::path m_TupPath;

	json m_BuildCache;
};
