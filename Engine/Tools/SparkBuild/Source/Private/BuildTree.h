/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#pragma once

#include "ArgParse.h"

#include <filesystem>

struct ModuleVersion
{
	unsigned long Major = 0;
	unsigned long Minor = 0;
	unsigned long Patch = 0;
	std::string Prerelease;
};

struct Module
{
	Module(const std::filesystem::path& path)
		: DefinitionPath(path)
	{}

	std::string Name;
	ModuleVersion Version;
	bool DefChanged = false;

	std::filesystem::path DefinitionPath;
};

class BuildTree
{
public:
	BuildTree(ArgParser& parser);

	std::vector<Module>& GetModules() { return m_ModuleList; }

	std::filesystem::path SourcePath;
	std::filesystem::path IntermediatePath;
	std::filesystem::path BinaryPath;

private:
	void BuildModuleTree();

	std::vector<Module> m_ModuleList;
};
