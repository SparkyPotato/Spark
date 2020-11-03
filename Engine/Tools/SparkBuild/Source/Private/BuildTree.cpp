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
	SourcePath = source;

	if (parser.GetSwitch(L"engine"))
	{
		IntermediatePath = source + L"/../../Intermediate/";
		BinaryPath = source + L"/../../Binaries/";
	}
	else
	{
		IntermediatePath = source + L"/../Intermediate/";
		BinaryPath = source + L"/../Binaries/";
	}

	if (parser.GetSwitch(L"clean")) return;

	BuildModuleTree(SourcePath);
}

void BuildTree::BuildModuleTree(std::filesystem::path path)
{
	try
	{
		size_t modules = SearchPath(path, m_ModuleList);

		wprintf(L"Found %zu modules. \n\n", modules);
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
				list.emplace_back(std::filesystem::absolute(std::filesystem::absolute(entry.path())));
				foundModules++;
			}
			else
			{
				throw Error(L"DUP_MODULE");
			}
		}
		else if (entry.is_directory())
		{
			foundModules += SearchPath(entry.path(), m_ModuleList);
		}
	}

	return foundModules;
}
