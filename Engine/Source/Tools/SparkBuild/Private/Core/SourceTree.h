// Copyright 2020 SparkyPotato

#pragma once

#include "Global/Output.h"

class SourceTree
{
public:
	SourceTree();

private:
	int FindModules(const fs::path& folder);
	void ParseModule(const fs::path& path, const std::string text, Module& m, const std::string& hash);
};
