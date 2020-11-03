/*
	SparkBuild.Private.TupGenerator
	Copyright 2020 SparkyPotato

	Tupfile and Tuprules generator
*/

#include "TupGenerator.h"
#include "Error.h"

#include <fstream>
#include <Windows.h>

TupGenerator::TupGenerator(ArgParser& parser, BuildTree& tree)
	: m_Tree(tree), m_Parser(parser)
{
	std::wstring intermediate = m_Tree.IntermediatePath;
	m_TupPath = intermediate + L"../.tup/";
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
		intermediate += L"/../";

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

void TupGenerator::CreateTupfiles(Module& module, std::filesystem::path moduleRoot)
{
	CreateTupfile(module, moduleRoot);

	for (auto& entry : std::filesystem::directory_iterator(moduleRoot))
	{
		if (entry.is_directory()) { CreateTupfiles(module, entry.path()); }
	}
}

void TupGenerator::CreateTupfile(Module& module, std::filesystem::path path)
{
	wchar_t modName[1000];
	MultiByteToWideChar(CP_UTF8, 0, module.Name.c_str(), -1, modName, 1000);

	std::filesystem::path privatePath = module.DefinitionPath.parent_path().wstring() + L"/Private/";
	privatePath = std::filesystem::absolute(privatePath);

	std::wstring stringPath = path;

	if (path == module.DefinitionPath.parent_path())
	{
		if (std::filesystem::exists(stringPath + L"/Tuprules.tup")) std::filesystem::remove(stringPath + L"/Tuprules.tup");
		std::wofstream tupfile(stringPath + L"/Tuprules.tup");

		std::wstring includePath = std::filesystem::absolute(module.DefinitionPath.parent_path()).wstring() + L"/Public/";
		std::replace(includePath.begin(), includePath.end(), '\\', '/');

		tupfile << L"CFLAGS +=  /I\"" << includePath << L"\"\n";

		SetFileAttributesW(std::wstring(stringPath + L"/Tuprules.tup").c_str(), FILE_ATTRIBUTE_HIDDEN);
	}
	else if (path == privatePath || std::search(privatePath.begin(), privatePath.end(), path.begin(), path.end()) != privatePath.end())
	{
		if (std::filesystem::exists(stringPath + L"/Tupfile")) std::filesystem::remove(stringPath + L"/Tupfile");
		std::wofstream tupfile(stringPath + L"/Tupfile");

		std::wstring debugFilePath = std::filesystem::absolute(m_Tree.IntermediatePath).wstring() + L"/Build/Debug/" + modName + L"/";
		std::replace(debugFilePath.begin(), debugFilePath.end(), '\\', '/');
		std::wstring devFilePath = std::filesystem::absolute(m_Tree.IntermediatePath).wstring() + L"/Build/Development/" + modName + L"/";
		std::replace(devFilePath.begin(), devFilePath.end(), '\\', '/');
		std::wstring relFilePath = std::filesystem::absolute(m_Tree.IntermediatePath).wstring() + L"/Build/Release/" + modName + L"/";
		std::replace(relFilePath.begin(), relFilePath.end(), '\\', '/');

		tupfile << L"include_rules" << L"\n\n";

		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(RELFLAGS) %f /Fo\"%o\" |> " << relFilePath << L"%B.o" << L"\n";
		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(DEVFLAGS) %f /Fo\"%o\" |> " << devFilePath << L"%B.o" << L"\n";
		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(DEBFLAGS) %f /Fo\"%o\" |> " << debugFilePath << L"%B.o" << L"\n";

		SetFileAttributesW(std::wstring(stringPath + L"/Tupfile").c_str(), FILE_ATTRIBUTE_HIDDEN);
	}
}

void TupGenerator::LoadBuildConfig()
{
	std::wstring buildConfigPath = m_Tree.IntermediatePath.wstring() + L"/Build/BuildConfig.json";

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

		std::ofstream(buildConfigPath) << std::setw(4) << buildConfig;

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
	std::wstring globalRules = m_Tree.SourcePath.wstring() + L"/Tuprules.tup";
	if (std::filesystem::exists(globalRules)) std::filesystem::remove(globalRules);
	std::wofstream tupfile(globalRules);

	tupfile << L"CFLAGS += /c \n";
	tupfile << L"CFLAGS += /EHsc \n";
	tupfile << L"CFLAGS += /utf-8 \n";
	tupfile << L"CFLAGS += /GS \n";
	tupfile << L"CFLAGS += /D\"_UNICODE\" \n";
	tupfile << L"CFLAGS += /D\"UNICODE\" \n";
	tupfile << L"CFLAGS += /diagnostics:caret \n";

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
}

void TupGenerator::LoadBuildCache()
{
	std::filesystem::path buildCachePath = m_Tree.IntermediatePath.wstring() + L"/Build/Cache/BuildCache.json";

	if (!std::filesystem::exists(buildCachePath.parent_path())) { std::filesystem::create_directory(buildCachePath.parent_path()); }
	if (std::filesystem::exists(buildCachePath)) { std::ifstream(buildCachePath) >> m_BuildCache; }
}

void TupGenerator::SaveBuildCache()
{
	std::filesystem::path buildCachePath = m_Tree.IntermediatePath.wstring() + L"/Build/Cache/BuildCache.json";

	if (std::filesystem::exists(buildCachePath)) { std::filesystem::remove(buildCachePath); }
	std::ofstream(buildCachePath) << std::setw(4) << m_BuildCache;
}
