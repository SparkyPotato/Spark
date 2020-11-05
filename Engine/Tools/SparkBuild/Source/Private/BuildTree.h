/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#pragma once

#include <filesystem>

// struct ModuleVersion
// {
// 	unsigned long Major = 0;
// 	unsigned long Minor = 0;
// 	unsigned long Patch = 0;
// 	std::string Prerelease;
// };
// 
// struct Module
// {
// 	Module(const std::filesystem::path& defpath, const std::filesystem::path& incpath, const std::filesystem::path& srcpath)
// 	{
// 		DefinitionPath = std::filesystem::absolute(defpath);
// 		IncludePath = std::filesystem::absolute(incpath);
// 		SourcePath = std::filesystem::absolute(srcpath);
// 	}
// 
// 	std::string Name;
// 	ModuleVersion Version;
// 	bool DefChanged = false;
// 
// 	std::filesystem::path DefinitionPath;
// 	std::filesystem::path IncludePath;
// 	std::filesystem::path SourcePath;
// 
// 	std::vector<std::string> Dependencies;
// };
// 
// inline bool operator==(const Module& module, const std::string& name) { return module.Name == name; }
// 
// class BuildTree
// {
// public:
// 	BuildTree(ArgParser& parser);
// 
// 	std::vector<Module>& GetModules() { return m_ModuleList; }
// 
// 	std::filesystem::path SourcePath;
// 	std::filesystem::path IntermediatePath;
// 	std::filesystem::path BinaryPath;
// 
// private:
// 	void BuildModuleTree();
// 
// 	std::vector<Module> m_ModuleList;
// };
