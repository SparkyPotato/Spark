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
	ModuleParser(ArgParser& parser, BuildTree& tree, std::wstring startPath);

private:
	struct Folder
	{
		std::string Name;
		std::vector<Folder> Subfolders;
	};

	friend void from_json(const nlohmann::json& j, Folder& folder);
	friend void to_json(nlohmann::json& j, const Folder& folder);

	void RebuildModules();

	void ParseModule(Module& module, nlohmann::json& moduleDef);
	void CheckFolderStructure(const Module& module, const std::filesystem::path& path, nlohmann::json& moduleCache);
	std::vector<Folder> GetSubfolders(const std::filesystem::path path);

	void Clean();
	void RecreateModule(Module& module);

	void InitTup();
	void CreateTupfile(std::ostream& file, Module& module);

	BuildTree& m_Tree;
	bool m_Rebuild = false;
	bool m_Clean = false;
	std::filesystem::path m_ModuleCache;
	std::filesystem::path m_TupPath;
	std::filesystem::path m_BinariesPath;
	std::filesystem::path m_ExecPath;

	using json = nlohmann::json;
};
