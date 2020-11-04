/*
	SparkBuild.Private.TupGenerator
	Copyright 2020 SparkyPotato

	Tupfile and Tuprules generator
*/

#include "TupGenerator.h"
#include "Error.h"

#include <fstream>
#include <Windows.h>

TupGenerator::TupGenerator(ArgParser& parser, BuildTree& tree, json& moduleRegistry)
	: m_Tree(tree), m_Parser(parser), m_ModuleRegistry(moduleRegistry)
{
	std::wstring intermediate = m_Tree.IntermediatePath;
	m_TupPath = intermediate + L"../../.tup/";
}

TupGenerator::~TupGenerator()
{
	SaveBuildCache();
}

void TupGenerator::InitTup()
{
	if (!std::filesystem::exists(m_TupPath))
	{
		std::wstring intermediate = m_Tree.IntermediatePath;
		intermediate += L"../../";

		auto directory = new wchar_t[10000];
		GetCurrentDirectoryW(1000, directory);

		SetCurrentDirectoryW(intermediate.c_str());
		system("tup init");

		SetFileAttributesW(m_TupPath.c_str(), FILE_ATTRIBUTE_HIDDEN);

		SetCurrentDirectoryW(directory);

		delete[] directory;
	}

	LoadBuildCache();
	LoadBuildConfig();
}

void TupGenerator::CreateTupfile(Module& module)
{
	wchar_t modName[100];
	MultiByteToWideChar(CP_UTF8, 0, module.Name.c_str(), -1, modName, 100);

	std::wstring debPath = m_Tree.IntermediatePath.wstring() + L"Debug/" + modName;
	std::wstring devPath = m_Tree.IntermediatePath.wstring() + L"Development/" + modName;
	std::wstring relPath = m_Tree.IntermediatePath.wstring() + L"Release/" + modName;

	if (!std::filesystem::exists(debPath)) { std::filesystem::create_directory(debPath); }
	if (!std::filesystem::exists(devPath)) { std::filesystem::create_directory(devPath); }
	if (!std::filesystem::exists(relPath)) { std::filesystem::create_directory(relPath); }

	if (std::filesystem::exists(debPath + L"/Tupfile")) { std::filesystem::remove(debPath + L"/Tupfile"); }
	if (std::filesystem::exists(devPath + L"/Tupfile")) { std::filesystem::remove(devPath + L"/Tupfile"); }
	if (std::filesystem::exists(relPath + L"/Tupfile")) { std::filesystem::remove(relPath + L"/Tupfile"); }

	std::wofstream debTupfile(debPath + L"/Tupfile");
	std::wofstream devTupfile(devPath + L"/Tupfile");
	std::wofstream relTupfile(relPath + L"/Tupfile");

	debTupfile << L"include_rules" << L"\n\n";
	devTupfile << L"include_rules" << L"\n\n";
	relTupfile << L"include_rules" << L"\n\n";

	std::wstring includePath = std::filesystem::absolute(module.DefinitionPath.parent_path()).wstring() + L"/Public/";
	std::replace(includePath.begin(), includePath.end(), '\\', '/');
	debTupfile << L"CFLAGS +=  /I\"" << includePath << L"\"\n\n";
	devTupfile << L"CFLAGS +=  /I\"" << includePath << L"\"\n\n";
	relTupfile << L"CFLAGS +=  /I\"" << includePath << L"\"\n\n";

	for (auto& dependency : module.Dependencies)
	{
		std::string incPath;

		try { incPath = m_ModuleRegistry.at(dependency)["IncludePath"]; }
		catch (...)
		{
			throw Error("Invalid dependency '%s' in module '%s'", dependency.c_str(), module.Name.c_str());
		}

		std::filesystem::path path = incPath;

		debTupfile << L"CFLAGS +=  /I\"" << path.wstring() << L"\"\n\n";
		devTupfile << L"CFLAGS +=  /I\"" << path.wstring() << L"\"\n\n";
		relTupfile << L"CFLAGS +=  /I\"" << path.wstring() << L"\"\n\n";
	}

	std::wstring path = module.SourcePath;
	std::replace(path.begin(), path.end(), L'\\', L'/');

	relTupfile << L": foreach " << path << L"*.cpp |> cl $(CFLAGS) $(RELFLAGS) %f /Fo\"%o\" |> %B.o \n";
	devTupfile << L": foreach " << path << L"*.cpp |> cl $(CFLAGS) $(DEVFLAGS) %f /Fo\"%o\" |> %B.o \n";
	debTupfile << L": foreach " << path << L"*.cpp |> cl $(CFLAGS) $(DEBFLAGS) %f /Fo\"%o\" |> %B.o \n";

	if (!std::filesystem::exists(module.SourcePath)) { return; }

	for (auto& entry : std::filesystem::recursive_directory_iterator(module.SourcePath))
	{
		if (entry.is_directory())
		{
			std::wstring subPath = std::filesystem::absolute(entry.path());
			std::replace(subPath.begin(), subPath.end(), L'\\', L'/');

			relTupfile << L": foreach " << subPath << L"/*.cpp |> cl $(CFLAGS) $(RELFLAGS) %f /Fo\"%o\" |> %B.o \n";
			devTupfile << L": foreach " << subPath << L"/*.cpp |> cl $(CFLAGS) $(DEVFLAGS) %f /Fo\"%o\" |> %B.o \n";
			debTupfile << L": foreach " << subPath << L"/*.cpp |> cl $(CFLAGS) $(DEBFLAGS) %f /Fo\"%o\" |> %B.o \n";
		}
	}

	debTupfile << std::endl;
	devTupfile << std::endl;
	relTupfile << std::endl;

	SetFileAttributesW(std::wstring(debPath + L"/Tupfile").c_str(), FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributesW(std::wstring(devPath + L"/Tupfile").c_str(), FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributesW(std::wstring(relPath + L"/Tupfile").c_str(), FILE_ATTRIBUTE_HIDDEN);
}

void TupGenerator::LoadBuildConfig()
{
	std::wstring buildConfigPath = m_Tree.IntermediatePath.wstring() + L"/../BuildConfig.json";

	if (!std::filesystem::exists(buildConfigPath))
	{
		json buildConfig;

		buildConfig["Global"]["C++Only"] = true;
		buildConfig["Global"]["WarningLevel"] = 4;
		buildConfig["Global"]["WarningsAreErrors"] = true;
		buildConfig["Global"]["C++Version"] = 17;

		buildConfig["Release"]["Optimization"] = "Speed";
		buildConfig["Release"]["RuntimeErrorChecks"] = false;
		buildConfig["Release"]["DebugInformation"] = false;

		buildConfig["Development"]["Optimization"] = "Speed";
		buildConfig["Development"]["RuntimeErrorChecks"] = false;
		buildConfig["Development"]["DebugInformation"] = false;

		buildConfig["Debug"]["Optimization"] = "Off";
		buildConfig["Debug"]["RuntimeErrorChecks"] = true;
		buildConfig["Debug"]["DebugInformation"] = true;

		std::ofstream(buildConfigPath) << std::setw(4) << buildConfig << std::endl;

		auto writeTime = std::filesystem::last_write_time(buildConfigPath).time_since_epoch().count();
		m_BuildCache["BuildConfig"]["WriteTime"] = writeTime;

		SetRules(buildConfig);
	}
	else
	{
		auto writeTime = std::filesystem::last_write_time(buildConfigPath).time_since_epoch().count();
		if (m_BuildCache["BuildConfig"]["WriteTime"] < writeTime)
		{
			wprintf(L"Build config changed. \n");
			m_BuildCache["BuildConfig"]["WriteTime"] = writeTime;

			json buildConfig;
			std::ifstream(buildConfigPath) >> buildConfig;
			SetRules(buildConfig);
		}
	}
}

void TupGenerator::SetRules(json buildConfig)
{
	std::wstring globalRules = L"Tuprules.tup";
	if (std::filesystem::exists(globalRules)) std::filesystem::remove(globalRules);
	std::wofstream tupfile(globalRules);

	tupfile << L"CFLAGS += /c \n";
	tupfile << L"CFLAGS += /EHsc \n";
	tupfile << L"CFLAGS += /utf-8 \n";
	tupfile << L"CFLAGS += /GS \n";
	tupfile << L"CFLAGS += /D\"_UNICODE\" \n";
	tupfile << L"CFLAGS += /D\"UNICODE\" \n";
	tupfile << L"CFLAGS += /diagnostics:caret \n";
	tupfile << L"CFLAGS += /nologo \n";

	if (buildConfig["Global"]["C++Only"]) { tupfile << L"CFLAGS += /TP \n"; }
	if (buildConfig["Global"]["WarningsAreErrors"]) { tupfile << L"CFLAGS += /WX \n"; }

	int warningLevel = buildConfig["Global"]["WarningLevel"];
	if (warningLevel < 5 && warningLevel >= 0) { tupfile << L"CFLAGS += /W" << warningLevel << L" \n"; }
	else if (warningLevel == 5) { tupfile << L"CFLAGS += /Wall \n"; }
	else { throw Error(L"INVALID_WARNING_LEVEL"); }

	int cpp = buildConfig["Global"]["C++Version"];
	if (cpp == 11 || cpp == 14 || cpp == 17) { tupfile << L"CFLAGS += /std:c++" << cpp << L" \n"; }
	else { throw Error(L"INVALID_CPP_VERSION"); }

	tupfile << std::endl;
	tupfile << L"RELFLAGS += /D\"IS_RELEASE\" \n";

	std::string config = buildConfig["Release"]["Optimization"];
	if (config == "Speed") { tupfile << L"RELFLAGS += /O2 \n"; }
	else if (config == "Size") { tupfile << L"RELFLAGS += /O1 \n"; }
	else if (config == "Off") { tupfile << L"RELFLAGS += /Od \n"; }
	else { throw Error(L"INVALID_OPTIMIZATION"); }

	if (buildConfig["Release"]["RuntimeErrorChecks"]) { tupfile << L"RELFLAGS += /RTC1 \n"; }
	if (buildConfig["Release"]["DebugInformation"]) { tupfile << L"RELFLAGS += /Z7 \n"; }

	tupfile << std::endl;
	tupfile << L"DEVFLAGS += /D\"IS_DEVELOPMENT\" \n";
	
	config = buildConfig["Development"]["Optimization"];
	if (config == "Speed") { tupfile << L"DEVFLAGS += /O2 \n"; }
	else if (config == "Size") { tupfile << L"DEVFLAGS += /O1 \n"; }
	else if (config == "Off") { tupfile << L"DEVFLAGS += /Od \n"; }
	else { throw Error(L"INVALID_OPTIMIZATION"); }

	if (buildConfig["Development"]["RuntimeErrorChecks"]) { tupfile << L"DEVFLAGS += /RTC1 \n"; }
	if (buildConfig["Development"]["DebugInformation"]) { tupfile << L"DEVFLAGS += /Z7 \n"; }

	tupfile << std::endl;
	tupfile << L"DEBFLAGS += /D\"IS_DEBUG\" \n";

	config = buildConfig["Debug"]["Optimization"];
	if (config == "Speed") { tupfile << L"DEBFLAGS += /O2 \n"; }
	else if (config == "Size") { tupfile << L"DEBFLAGS += /O1 \n"; }
	else if (config == "Off") { tupfile << L"DEBFLAGS += /Od \n"; }
	else { throw Error(L"INVALID_OPTIMIZATION"); }

	if (buildConfig["Debug"]["RuntimeErrorChecks"]) { tupfile << L"DEBFLAGS += /RTC1 \n"; }
	if (buildConfig["Debug"]["DebugInformation"]) { tupfile << L"DEBFLAGS += /Z7 \n"; }

	SetFileAttributesW(L"Tuprules.tup", FILE_ATTRIBUTE_HIDDEN);
}

void TupGenerator::LoadBuildCache()
{
	std::filesystem::path buildCachePath = m_Tree.IntermediatePath.wstring() + L"../Cache/BuildCache.json";

	if (!std::filesystem::exists(buildCachePath.parent_path())) { std::filesystem::create_directory(buildCachePath.parent_path()); }
	if (std::filesystem::exists(buildCachePath)) { std::ifstream(buildCachePath) >> m_BuildCache; }
}

void TupGenerator::SaveBuildCache()
{
	std::filesystem::path buildCachePath = m_Tree.IntermediatePath.wstring() + L"../Cache/BuildCache.json";

	std::ofstream(buildCachePath, std::ofstream::trunc) << std::setw(4) << m_BuildCache;
}
