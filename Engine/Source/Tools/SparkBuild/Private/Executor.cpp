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
		DirtyFolder(*buildModule, buildModule->Location);
	}

	BasePlatform::Output("Changed ", reparses, " module/s.");
}

void Executor::CheckHeaders()
{
	for (auto header : m_Tree.GetDirtyHeaders())
	{
		for (auto& dependency : header.second->DependedOn)
		{
			for (auto source : m_Tree.GetSources())
			{
				std::error_code ec;
				if (fs::equivalent(source.second->Path, dependency, ec))
				{
					m_Tree.AddDirtySource(source.first, source.second);
				}
			}
		}
	}
}

void Executor::Compile()
{
	int compilations = 0;
	for (auto source : m_Tree.GetDirtySourceFiles())
	{
		m_CompileList[source.first].emplace_back(source.second);
		compilations++;
	}
	BasePlatform::Output("Compiling ", compilations, " file/s.");

	for (auto& list : m_CompileList)
	{
		BasePlatform::Compile(*list.first, list.second);
	}
}

void Executor::AddHeaderDependencies()
{
	fs::path dependencies = Globals::IntermediatePath.string() + "/DependencyGraph/";
	for (auto& entry : fs::directory_iterator(dependencies))
	{
		json j;
		std::ifstream(entry.path()) >> j;

		const auto& sourceDependencies = j["Data"]["Includes"].get<std::vector<String>>();

		for (auto& header : sourceDependencies)
		{
			for (auto headerFile : m_Tree.GetHeaders())
			{
				if (fs::equivalent(headerFile.second->Path, header))
				{
					headerFile.second->DependedOn.emplace_back(j["Data"]["Source"]);
				}
			}
		}
	}

	fs::remove_all(dependencies);
}

void Executor::Link()
{
	for (auto source : m_Tree.GetDirtySourceFiles())
	{
		m_ModulesToRelink[source.first] = 1;
	}

	for (auto buildModule : m_ModulesToRelink)
	{
		m_LinkThreads.emplace_back(&BasePlatform::GenerateExports, std::ref(*buildModule.first));
	}
	for (auto& thread : m_LinkThreads)
	{
		thread.join();
	}
	m_LinkThreads.clear();

	for (auto buildModule : m_ModulesToRelink)
	{
		m_LinkThreads.emplace_back(&BasePlatform::Link, std::ref(*buildModule.first));
	}
	for (auto& thread : m_LinkThreads)
	{
		thread.join();
	}
	m_LinkThreads.clear();
}

void Executor::CopyDependencies()
{
	std::vector<Module*> executables;

	for (auto& buildModule : m_Tree.GetModules())
	{
		if (buildModule.Executable) { executables.emplace_back(&buildModule); }
	}

	std::map<String, std::vector<String>> executableDependencies;
	for (auto executable : executables)
	{
		DiscoverDependencies(executable->Name, executableDependencies[executable->Name]);
	}

	for (auto buildModule : m_ModulesToRelink)
	{
		for (auto executable : executables)
		{
			fs::path executablePath = Globals::BinariesPath.string() + "/" +
				CommandLine::GetProperty("config") + "/" + executable->Name + "/Executable/";

			for (auto& dependency : executableDependencies[executable->Name])
			{
				CopyModule(dependency, executablePath);
			}
		}
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

	// Parse if module should be built as an executable as well
	try { buildModule.Executable = parser.at("Executable"); }
	catch (...) { Warning("Module '", buildModule.Name, "' has no executable specifier, assuming false."); }

	Globals::ModuleRegistry[buildModule.Name] =
	{
		{ "Version", buildModule.Version },
		{ "Path", fs::absolute(buildModule.Location.Path).string() },
		{ "BinaryPath", fs::absolute(Globals::BinariesPath).string() },
		{ "Executable", buildModule.Executable },
		{ "Dependencies", buildModule.Dependencies }
	};
}

void Executor::DirtyFolder(Module& buildModule, Folder& folder)
{
	for (auto& source : folder.SourceFiles)
	{
		m_Tree.AddDirtySource(&buildModule, &source);
	}

	for (auto& sub : folder.Subfolders)
	{
		DirtyFolder(buildModule, sub);
	}
}

void Executor::CopyModule(String& moduleName, fs::path location)
{
	fs::path path = Globals::ModuleRegistry[moduleName]["BinaryPath"].get<String>() + "/" +
		CommandLine::GetProperty("config") + "/" + moduleName + "/" + moduleName + ".dll";

	if (fs::exists(path))
	{
		fs::copy_file(path, location.string() + moduleName + ".dll", fs::copy_options::overwrite_existing);
	}
}

void Executor::DiscoverDependencies(const String& buildModule, std::vector<String>& dependencies)
{
	const std::vector<String>& moduleDependencies = Globals::ModuleRegistry[buildModule]["Dependencies"].get<std::vector<String>>();
	dependencies.insert(dependencies.end(), moduleDependencies.begin(), moduleDependencies.end());

	for (auto& dependency : moduleDependencies)
	{
		DiscoverDependencies(dependency, dependencies);
	}
}