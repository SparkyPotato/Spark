/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#pragma once

#include "ArgParse.h"

#include <filesystem>

class BuildTree
{
public:
	BuildTree(ArgParser& parser);

private:
	void BuildModuleList(std::filesystem::path source);

	std::vector<std::filesystem::path> m_ModuleDefs;
};
