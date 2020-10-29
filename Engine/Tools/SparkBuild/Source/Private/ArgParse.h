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

private:
	std::map<std::wstring, std::wstring> m_PropertyMap;
};
