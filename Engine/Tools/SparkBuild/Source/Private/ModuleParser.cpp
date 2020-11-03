/*
	SparkBuild.Private.ModuleParser
	Copyright 2020 SparkyPotato

	Module.json parser
*/

#include "ModuleParser.h"

#include "Error.h"
#include <fstream>

#include <Windows.h>

ModuleParser::ModuleParser(ArgParser& parser, BuildTree& tree, std::filesystem::path startPath)
	: m_Tree(tree), m_ExecPath(startPath), m_Parser(parser)
{
	m_Rebuild = parser.GetSwitch(L"rebuild");

	std::wstring intermediate = m_Tree.IntermediatePath;
	std::wstring binaries = m_Tree.BinaryPath;

	m_ModuleCache = intermediate + L"Build/ModuleCache.json";
	m_TupPath = intermediate + L"../.tup/";
	m_BinariesPath = binaries + L"Build/";

	RebuildModules();
}

void ModuleParser::RebuildModules()
{
	Clean();

	InitTup();

	for (auto& module : m_Tree.GetModules())
	{
		std::ifstream stream(module.DefinitionPath);
		json moduleDef;
		
		try
		{
			stream >> moduleDef;
		}
		catch (...)
		{
			throw Error(L"PARSE_FAIL: %s", module.DefinitionPath.c_str());
		}

		ParseModule(module, moduleDef);

		CreateTupfiles(module, module.DefinitionPath.parent_path());

		stream.close();
	}
}

void ModuleParser::ParseModule(Module& module, json& moduleDef)
{
	try
	{
		module.Name = moduleDef.at("Module");
	}
	catch (...)
	{
		throw Error(L"UNNAMED_MODULE");
	}

	try
	{
		std::vector<std::string> version = moduleDef.at("Version");

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

void ModuleParser::Clean()
{
	if (m_Rebuild)
	{
		wprintf(L"Cleaning up Intermediate and Binaries. \n");
		if (std::filesystem::exists(m_ModuleCache)) { std::filesystem::remove_all(m_ModuleCache.parent_path()); }
		if (std::filesystem::exists(m_TupPath)) { std::filesystem::remove_all(m_TupPath); }
		if (std::filesystem::exists(m_BinariesPath)) { std::filesystem::remove_all(m_BinariesPath); }

		wprintf(L"Cleaning up Build files. \n");
		std::filesystem::recursive_directory_iterator it(m_Tree.SourcePath);

		for (auto& entry : it)
		{
			if (entry.path().filename() == L"Tupfile") std::filesystem::remove_all(entry);
		}
	}
}

void ModuleParser::InitTup()
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
}

void ModuleParser::CreateTupfiles(Module& module, std::filesystem::path moduleRoot)
{
	CreateTupfile(module, moduleRoot);

	for (auto& entry : std::filesystem::directory_iterator(moduleRoot))
	{
		if (entry.is_directory()) { CreateTupfiles(module, entry.path()); }
	}
}

void ModuleParser::CreateTupfile(Module& module, std::filesystem::path path)
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

		tupfile << L"CFLAGS += /c \n";
		tupfile << L"CFLAGS += /EHsc \n";
		tupfile << L"CFLAGS += /utf-8 \n";

		tupfile << L"CFLAGS +=  /I\"" << includePath << L"\"\n";
		tupfile << L"CFLAGS += /D\"_UNICODE\" \n";
		tupfile << L"CFLAGS += /D\"UNICODE\" \n";
		tupfile << L"CFLAGS += /diagnostics:caret \n";
		tupfile << L"CFLAGS += /TP \n";
		tupfile << L"CFLAGS += /W3 \n";
		tupfile << L"CFLAGS += /WX \n";
		tupfile << L"CFLAGS += /GS \n";
		tupfile << L"CFLAGS += /std:c++17 \n";

		tupfile << std::endl;
		tupfile << L"RELFLAGS += /D\"IS_RELEASE\" \n";
		tupfile << L"RELFLAGS += /O2 \n";

		tupfile << std::endl;
		tupfile << L"DEVFLAGS += /D\"IS_DEVELOPMENT\" \n";
		tupfile << L"DEVFLAGS += /O2 \n";

		tupfile << std::endl;
		tupfile << L"DEBFLAGS += /D\"IS_DEBUG\" \n";
		tupfile << L"DEBFLAGS += /Od \n";
		tupfile << L"DEBFLAGS += /RTC1 \n";
		tupfile << L"DEBFLAGS += /Z7 \n";

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

		tupfile << L"include_rules" << L"\n";
		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(RELFLAGS) %f /Fo\"%o\" |> " << relFilePath << L"%B.o" << L"\n";
		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(DEVFLAGS) %f /Fo\"%o\" |> " << devFilePath << L"%B.o" << L"\n";
		tupfile << L": foreach *.cpp |> cl $(CFLAGS) $(DEBFLAGS) %f /Fo\"%o\" |> " << debugFilePath << L"%B.o" << L"\n";

		SetFileAttributesW(std::wstring(stringPath + L"/Tupfile").c_str(), FILE_ATTRIBUTE_HIDDEN);
	}
}
