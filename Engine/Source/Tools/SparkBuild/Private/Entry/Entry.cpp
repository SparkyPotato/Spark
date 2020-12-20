// Copyright 2020 SparkyPotato

#include "Global/Output.h"
#include "Core/SourceTree.h"

int Entry(std::vector<std::string>& args)
{
	try
	{
		const char* env = getenv("SPARK_BUILD");
		if (!env)
		{
			throw std::exception("SparkBuild must be invoked from the Build script!");
		}

		if (args.size() != 4)
		{
			throw std::exception("SparkBuild needs 3 arguments, BuildType, ProjectDirectory, and Configuration!");
		}

		// Setup global variables
		Global::ProjectPath = fs::absolute(args[2]);

		if (args[1] == "Build") { Global::Type = BuildType::Build; }
		else if (args[1] == "Rebuild") { Global::Type = BuildType::Rebuild; }
		else if (args[1] == "Clean") { Global::Type = BuildType::Clean; }
		else { throw std::exception("Unknown build type!"); }

		if (args[3] == "Debug") { Global::Config = BuildConfig::Debug; }
		else if (args[3] == "Development") { Global::Config = BuildConfig::Development; }
		else if (args[3] == "Release") { Global::Config = BuildConfig::Release; }
		else { throw std::exception("Unknown build configuration!"); }

		// Set the path as the root engine directory
		fs::current_path(args[0] + "/../../../../../");
		Global::LoadRegistry();

		SourceTree tree;

		Global::SaveRegistry();

		return EXIT_SUCCESS;
	}
	catch (std::exception& e)
	{
		Console("Error: ", e.what());
		return EXIT_FAILURE;
	}
}
