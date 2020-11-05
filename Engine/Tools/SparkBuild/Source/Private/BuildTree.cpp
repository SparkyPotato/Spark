/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#include "BuildTree.h"
#include "Error.h"

// BuildTree::BuildTree(ArgParser& parser)
// {
// 	std::wstring source = parser.GetProperty(L"source");
// 	SourcePath = source;
// 
// 	if (parser.GetSwitch(L"engine"))
// 	{
// 		IntermediatePath = source + L"/../../Intermediate/Build/";
// 		BinaryPath = source + L"/../../Binaries/Build/";
// 	}
// 	else
// 	{
// 		IntermediatePath = source + L"/../Intermediate/Build/";
// 		BinaryPath = source + L"/../Binaries/Build/";
// 	}
// 
// 	if (parser.GetSwitch(L"clean")) return;
// 
// 	if (!std::filesystem::exists(BinaryPath)) { std::filesystem::create_directory(BinaryPath); }
// 	if (!std::filesystem::exists(BinaryPath.wstring() + L"Debug")) { std::filesystem::create_directory(BinaryPath.wstring() + L"Debug"); }
// 	if (!std::filesystem::exists(BinaryPath.wstring() + L"Development")) { std::filesystem::create_directory(BinaryPath.wstring() + L"Development"); }
// 	if (!std::filesystem::exists(BinaryPath.wstring() + L"Release")) { std::filesystem::create_directory(BinaryPath.wstring() + L"Release"); }
// 
// 	if (!std::filesystem::exists(IntermediatePath)) { std::filesystem::create_directory(IntermediatePath); }
// 	if (!std::filesystem::exists(IntermediatePath.wstring() + L"Debug")) { std::filesystem::create_directory(IntermediatePath.wstring() + L"Debug"); }
// 	if (!std::filesystem::exists(IntermediatePath.wstring() + L"Development")) { std::filesystem::create_directory(IntermediatePath.wstring() + L"Development"); }
// 	if (!std::filesystem::exists(IntermediatePath.wstring() + L"Release")) { std::filesystem::create_directory(IntermediatePath.wstring() + L"Release"); }
// 
// 	BuildModuleTree();
// }
// 
// void BuildTree::BuildModuleTree()
// {
// 	try
// 	{
// 		int foundModules = 0;
// 
// 		for (auto& moduleDir : std::filesystem::directory_iterator(SourcePath))
// 		{
// 			std::wstring moduleDef = moduleDir.path();
// 
// 			if (std::filesystem::exists(moduleDef + L"/Module.json"))
// 			{
// 				m_ModuleList.emplace_back(moduleDef + L"/Module.json", moduleDef + L"/Public/", moduleDef + L"/Private/");
// 				foundModules++; 
// 			}
// 		}
// 
// 		wprintf(L"Found %d modules. \n", foundModules);
// 	}
// 	catch (const Error& e)
// 	{
// 		throw e;
// 	}
// 	catch (...)
// 	{
// 		throw Error(L"PATH_NOT_FOUND: %s", SourcePath.c_str());
// 	}
// }
