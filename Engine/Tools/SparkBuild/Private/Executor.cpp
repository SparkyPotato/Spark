// SparkBuild.Private.Executor
// Copyright 2020 SparkyPotato

// Executes all necessary operations on a source tree

#include "Executor.h"

#include <regex>

#include "Error.h"

Executor::Executor(SourceTree& tree)
	: m_Tree(tree)
{}

void Executor::Parse()
{
	int reparses = 0;

	for (auto buildModule : m_Tree.GetDirtyModules())
	{
		reparses++;
		ParseModule(*buildModule);

		// Module has been marked dirty so all source files must be marked dirty
		DirtyFolder(buildModule->Location);
	}

	BasePlatform::Output("Built ", reparses, " modules.");
}

void Executor::Compile()
{
	int recompilations = 0;

	for (auto source : m_Tree.GetDirtySourceFiles())
	{
		recompilations++;
		SpawnCompilationThread(source);
	}

	BasePlatform::Output("Compiling ", recompilations, " files.");

	for (auto& thread : m_CompilationThreads)
	{
		thread.join();
	}
}

void Executor::ParseModule(Module& buildModule)
{
	Verbose("Parsing module");

	// Semver regex - https://regex101.com/r/vkijKf/1/
	static const std::regex versionRegex(R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-])"
		R"([0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)");

	json parser;

	// Get the name of the folder containing the module definition
	std::string folderName = buildModule.Location.Path.string();
	folderName = folderName.substr(folderName.rfind('/') + 1);
	//                                  Skips the last '/'-^

	// Check if the Module.json definition file exists
	std::string definitionPath = buildModule.Location.Path.string() + "/Module.json";
	if (!fs::exists(definitionPath))
	{
		Error("Module '", folderName, "' has no module definition.");
	}

	// Catch any parse exceptions
	try { std::ifstream(definitionPath) >> parser; }
	catch (...) { Error("Failed to parse module '", folderName, "'"); }

	// Parse Name and report any mismatches between folder name and module name
	buildModule.Name = parser["Name"];
	if (buildModule.Name.empty()) { Error("Module '", folderName, "' has no Name specifier."); }
	if (buildModule.Name != folderName)
	{
		Error("Module '", buildModule.Name, "' does not match with its folder '", folderName, "'.");
	}

	// Parse Version
	buildModule.Version = parser["Version"];
	if (buildModule.Version.empty()) { Error("Module '", buildModule.Name, "' does not have a version specifier"); }
	if (!std::regex_match(buildModule.Version, versionRegex))
	{
		Error("Module '", buildModule.Name, "' does not have a valid version specifier. See https://semver.org.");
	}

	// Parse Dependencies
	try { buildModule.Dependencies = parser.at("Dependencies").get<std::vector<String>>(); }
	catch (...) { Warning("Module '", buildModule.Name, "' has no dependencies specifier, assuming none."); }

	Globals::ModuleRegistry[buildModule.Name] =
	{
		{ "Version", buildModule.Version },
		{ "Path", buildModule.Location.Path.string() }
	};
}

void Executor::DirtyFolder(Folder& folder)
{
	for (auto& source : folder.SourceFiles)
	{
		source.Dirty = true;
		m_Tree.AddDirtySource(&source);
	}

	for (auto& sub : folder.Subfolders)
	{
		DirtyFolder(sub);
	}
}

void Executor::SpawnCompilationThread(File* source)
{
	m_CompilationThreads.emplace_back(&Executor::ThreadWorker, source);
}

void Executor::ThreadWorker(File* source)
{
	BasePlatform::Compile(source, "");
}
