/*
	SparkBuild.Private.ArgParse
	Copyright 2020 SparkyPotato

	Command-line argument parser
*/

#pragma once

#include <vector>
#include <string>
#include <map>

class ArgParser
{
public:
	ArgParser(const std::vector<std::wstring>& args);

	const std::wstring& GetProperty(const std::wstring& property) { return m_Properties[property]; }

private:
	std::map<std::wstring, std::wstring> m_Properties;
	std::vector<std::wstring> m_Switches;
};
