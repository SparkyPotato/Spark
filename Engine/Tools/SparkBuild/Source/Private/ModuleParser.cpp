/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#include "ModuleParser.h"

#include "Error.h"
#include <fstream>

ModuleParser::ModuleParser(ArgParser& parser, std::vector<Module>& modules)
{
	for (auto& module : modules)
	{
		std::ifstream stream(module.DefinitionPath);
		json j;
		stream >> j;

		try
		{
			module.Name = j.at("Module");
		}
		catch (...)
		{
			throw Error(L"UNNAMED_MODULE");
		}

		try
		{
			std::vector<std::string> version = j.at("Version");

			if (version.size() < 3 || version.size() > 4) { throw Error(L"ILLEGAL_VERSION_FORMAT"); }

			module.Version.Major = std::stoul(version[0]);
			module.Version.Minor = std::stoul(version[1]);
			module.Version.Patch = std::stoul(version[2]);

			if (version.size() == 4)
			{
				module.Version.Prerelease = version[3];
			}
		}
		catch (...)
		{
			throw Error(L"NO_MODULE_VERSION");
		}
	}
}
