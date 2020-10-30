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

	BuildModuleTree(source + L"/Source");
}

void BuildTree::BuildModuleTree(std::filesystem::path path)
{
	try
	{
		wprintf(L"Found %zu modules. \n\n", SearchPath(path, m_ModuleTree));
	}
	catch (const Error& e)
	{
		throw e;
	}
	catch (...)
	{
		throw Error(L"PATH_NOT_FOUND: %s", path.c_str());
	}
}

size_t BuildTree::SearchPath(std::filesystem::path path, std::vector<Module>& list)
{
	size_t foundModules = 0;

	std::filesystem::directory_iterator directory(path);

	bool pathIsModule = false;

	for (auto& entry : directory)
	{
		if (entry.is_regular_file() && entry.path().filename() == L"Module.json")
		{
			if (!pathIsModule)
			{
				pathIsModule = true;
				list.emplace_back(entry.path());
				foundModules++;
			}
			else
			{
				throw Error(L"DUP_MODULE");
			}
		}
	}

	std::filesystem::directory_iterator dir(path);

	for (auto& entry : dir)
	{
		if (entry.is_directory() && pathIsModule)
		{
			foundModules += SearchPath(entry.path(), (*(list.end() - 1)).Submodules);
		}
		else if (entry.is_directory())
		{
			foundModules += SearchPath(entry.path(), list);
		}
	}

	return foundModules;
}
