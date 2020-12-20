// Copyright 2020 SparkyPotato

#include "Global/Global.h"

#include "json/json.hpp"

using json = nlohmann::json;

void to_json(json& j, const Module& m)
{
	j = {
		{ "Version", m.Version },
		{ "Dependencies", m.Dependencies },
		{ "Executable", m.Executable },
		{ "Definition", m.Definition.string() },
		{ "WriteTime", m.WriteTime },
		{ "Hash", m.Hash }
	};
}

void from_json(const json& j, Module& m)
{
	j.at("Version").get_to(m.Version);
	j.at("Dependencies").get_to(m.Dependencies);
	j.at("Executable").get_to(m.Executable);
	m.Definition = j.at("Definition").get<std::string>();
	j.at("WriteTime").get_to(m.WriteTime);
	j.at("Hash").get_to(m.Hash);
}

namespace Global
{
	fs::path ProjectPath;
	BuildType Type;
	BuildConfig Config;
	std::unordered_map<std::string, Module> Registry;

	void LoadRegistry()
	{
		if (fs::exists("Registry/Modules"))
		{
			json registry;
			std::ifstream("Registry/Modules") >> registry;
			Registry = registry.get<std::unordered_map<std::string, Module>>();
		}
	}

	void SaveRegistry()
	{
		json registry = Registry;
		if (!fs::exists("Registry")) { fs::create_directory("Registry"); }
		std::ofstream("Registry/Modules") << std::setw(2) << registry;
	}
}
