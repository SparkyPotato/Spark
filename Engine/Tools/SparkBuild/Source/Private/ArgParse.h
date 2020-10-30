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
	bool GetSwitch(const std::wstring name) { return std::find(m_Switches.begin(), m_Switches.end(), name) != m_Switches.end(); }

private:
	std::map<std::wstring, std::wstring> m_Properties;
	std::vector<std::wstring> m_Switches;
};
