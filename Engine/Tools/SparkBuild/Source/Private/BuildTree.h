/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#pragma once

#include "ArgParse.h"

#include <filesystem>

struct Module
{
	Module(const std::filesystem::path& path)
		: DefinitionPath(path)
	{}

	std::wstring Name;
	std::filesystem::path DefinitionPath;

	std::vector<Module> Submodules;
};

class BuildTree
{
public:
	BuildTree(ArgParser& parser);

private:
	void BuildModuleTree(std::filesystem::path source);
	size_t SearchPath(std::filesystem::path path, std::vector<Module>& list);

	std::vector<Module> m_ModuleTree;
};
