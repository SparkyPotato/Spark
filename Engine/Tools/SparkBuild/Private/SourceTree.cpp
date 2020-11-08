// SparkBuild.Private.SourceTree
// Copyright 2020 SparkyPotato

// Tree of all source files and modules in the project

#include "SourceTree.h"

#include "Error.h"

SourceTree::SourceTree()
{
	fs::path projectDir = CommandLine::GetProperty("dir");

	Verbose("Generating SourceTree for: ", projectDir, ".");

	fs::path sourcePath = projectDir.string() + "/Source/";
	if (!fs::exists(sourcePath))
	{
		Error("Project does not contain a Source folder!");
	}
	FindModules(sourcePath);

	fs::path dependencyPath = projectDir.string() + "/Dependencies";
	if (fs::exists(dependencyPath))
	{
		Verbose("Found project dependencies.");
		FindModules(dependencyPath);
	}

	fs::path toolPath = projectDir.string() + "/Tools";
	if (fs::exists(toolPath))
	{
		Verbose("Found project tools.");
		FindModules(toolPath);
	}

	BasePlatform::Output("Found ", m_Modules.size(), " modules.");

	if (CommandLine::GetSwitch("verbose"))
	{
		Verbose(false, "");
		Verbose("Modules located in:");
		for (auto& buildModule : m_Modules)
		{
			Verbose(false, buildModule.Location.Path.string());
		}
		Verbose(false, "");
	}

	// Would use 'module' here but it's a reserved keyword in C++20
	for (auto& buildModule : m_Modules)
	{
		PopulateFolder(buildModule.Location);
	}

	BasePlatform::Output
	(
		"Found ", m_HeaderCount, " headers and ",
		m_SourceCount, " source files, spread over ",
		m_DirectoryCount, " directories."
	);
}

void SourceTree::FindModules(const fs::path& folder)
{
	// Check if the folder contains a module definition
	if (fs::exists(folder.string() + "/Module.json"))
	{
		// Cleanup backslashes in the path (Windows annoying)
		std::string cleanPath = folder.string();
		std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

		m_Modules.emplace_back(cleanPath);
		return;
	}

	// If it doesn't, recursively iterate over sub-folders to find all modules
	for (auto& entry : fs::directory_iterator(folder))
	{
		if (entry.is_directory()) { FindModules(entry.path()); }
	}
}

void SourceTree::PopulateFolder(Folder& folder)
{
	m_DirectoryCount++;

	for (auto& entry : fs::directory_iterator(folder.Path))
	{
		std::string extension = entry.path().extension().string();

		// SparkBuild only supports .cpp, .cc, and .cxx files
		bool isSource = extension == ".cpp" || extension == ".cc" || extension == ".cxx";
		if (entry.is_regular_file() && isSource) 
		{
			std::string cleanPath = entry.path().string();
			std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

			folder.SourceFiles.emplace_back(cleanPath);
			m_SourceCount++;
		}

		bool isHeader = extension == ".h" || extension == ".hpp";
		if (entry.is_regular_file() && isHeader) 
		{
			std::string cleanPath = entry.path().string();
			std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

			folder.HeaderFiles.emplace_back(cleanPath);
			m_HeaderCount++;
		}

		if (entry.is_directory())
		{
			std::string cleanPath = entry.path().string();
			std::replace(cleanPath.begin(), cleanPath.end(), '\\', '/');

			auto& subfolder = folder.Subfolders.emplace_back(cleanPath);
			PopulateFolder(subfolder); // Recursively populate sub-folders
		}
	}
}
