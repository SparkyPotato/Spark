/*
	SparkBuild.Private.BuildTree
	Copyright 2020 SparkyPotato

	Module tree generator for compilation
*/

#include "BuildTree.h"
#include "Error.h"

BuildTree::BuildTree(ArgParser& parser)
{
	std::wstring source = parser.GetProperty(L"source");

	if (source.empty())
	{
		source = L"../../../Engine";
	}

	BuildModuleList(source + L"/Source");
}

void BuildTree::BuildModuleList(std::filesystem::path path)
{
	try
	{
		std::filesystem::recursive_directory_iterator directory(path);

		for (auto& entry : directory)
		{
			if (entry.is_regular_file() && entry.path().filename() == L"Module.json")
			{
				m_ModuleDefs.emplace_back(entry.path());
			}
		}
	}
	catch (...)
	{
		throw Error(L"PATH_NOT_FOUND: %s", path.c_str());
	}
}
