/*
	SparkBuild.Private.ArgParse
	Copyright 2020 SparkyPotato

	Command-line argument parser
*/

#include "ArgParse.h"

#include "Error.h"

ArgParser::ArgParser(const std::vector<std::wstring>& args)
{
	for (const auto& arg : args)
	{
		if (arg[0] == L'-')
		{
			std::wstring rawArg = arg.substr(1, arg.npos);
			for (auto& c : rawArg)
			{
				if (c >= 65 && c <= 90) c += 32;
			}

			m_Switches.emplace_back(std::move(rawArg));
		}
		else
		{
			size_t pos = arg.find(L'=');

			if (pos == arg.npos)
			{
				throw Error(L"INVALID_ARG");
			}

			std::wstring property = arg.substr(0, pos);
			std::wstring value = arg.substr(pos + 1, arg.npos);

			for (auto& c : property)
			{
				if (c >= 65 && c <= 90) c += 32;
			}

			m_Properties.emplace(property, value);
		}
	}
}
