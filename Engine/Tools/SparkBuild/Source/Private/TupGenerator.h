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
	void SaveBuildCache();
	void SetRules(json buildConfig);

	void LoadBuildCache();

	BuildTree& m_Tree;
	ArgParser& m_Parser;

	std::filesystem::path m_TupPath;

	json m_BuildCache;
};
