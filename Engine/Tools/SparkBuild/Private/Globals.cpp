// SparkBuild.Private.Globals
// Copyright 2020 SparkyPotato

// Global definitions

#include "Globals.h"

#include "Error.h"

namespace Globals
{
	fs::path SourcePath;
	fs::path IntermediatePath;
	fs::path BinariesPath;

	json ModuleRegistry;
	json BuildCache;
	bool BuildCacheExists = false;

	void ParseCommandLine(int argc, wchar_t** argv);

	void Setup(int argc, wchar_t** argv)
	{
		/*
			Set working directory to engine root (where Spark.sln/xcworkspace is located) 
			- this may not work if we have a symlink for the executable and it is called through that
		*/
		String executablePath = fs::path(argv[0]).parent_path().string();
		executablePath += "/../../../../";
		BasePlatform::SetWorkingDirectory(executablePath);

		ParseCommandLine(argc, argv);

		SourcePath = CommandLine::GetProperty("dir") + "/Source";
		IntermediatePath = CommandLine::GetProperty("dir") + "/Intermediate";
		BinariesPath = CommandLine::GetProperty("dir") + "/Binaries";

		if (CommandLine::GetSwitch("verbose"))
		{
			Verbose("Switches are: ");
			for (auto& switchArg : CommandLine::Switches)
			{
				Verbose(false, String("    "), switchArg);
			}
			Verbose(false, String(""));

			Verbose("Properties are: ");
			for (auto& property : CommandLine::Properties)
			{
				Verbose(false, String("    "), property.first, " = ", property.second);
			}
			Verbose(false, String(""));
		}

		if (CommandLine::GetProperty("dir").empty())
		{
			Error("No project directory property found"); // SparkBuild needs a project directory location
		}

		if (!fs::exists("Registry")) { fs::create_directory("Registry"); }

		if (fs::exists("Registry/ModuleRegistry.json"))
		{
			std::ifstream("Registry/ModuleRegistry.json") >> ModuleRegistry;
		}

		if (!fs::exists(CommandLine::GetProperty("dir") + "/Intermediate")) { fs::create_directory(CommandLine::GetProperty("dir") + "/Intermediate"); }

		if (fs::exists(CommandLine::GetProperty("dir") + "/Intermediate/BuildCache.json"))
		{
			BuildCacheExists = true;
			std::ifstream(CommandLine::GetProperty("dir") + "/Intermediate/BuildCache.json") >> BuildCache;
		}
		else
		{
			BuildCacheExists = false;
		}

		BasePlatform::SetupCompiler();
	}

	void Save()
	{
		std::ofstream("Registry/ModuleRegistry.json") << std::setw(4) << ModuleRegistry;
		std::ofstream(CommandLine::GetProperty("dir") + "/Intermediate/BuildCache.json") << std::setw(4) << BuildCache;
	}

	bool VerifySwitch(const String& switchArg);
	bool VerifyProperty(const String& propertyName);

	void ParseCommandLine(int argc, wchar_t** argv)
	{
		for (int i = 1; i < argc; i++)
		{
			String arg = BasePlatform::ToUTF8(argv[i]); // Convert from UTF-16 to UTF-8

			if (arg[0] == '-') // A '-' indicates a switch
			{
				String switchArg = arg.substr(1, arg.npos); // Remove the preceding '-'

				for (auto& c : switchArg) // All switches are single-byte, so they can be lowercase-ified with simple addition
				{
					if (c >= 65 && c <= 90) c += 32;
				}

				if (VerifySwitch(switchArg)) { CommandLine::Switches.emplace_back(std::move(switchArg)); }
				else {  Warning("Ignoring unknown switch '", switchArg, "'."); }
			}
			else
			{
				size_t pos = arg.find('=', 0); // Find the '=' separator between the property and its value

				if (pos == arg.npos)
				{
					Error("Invalid property specifier: ", arg); // Invalid property if we don't find the '='
				}

				// If the argument is "dir=Engine/"
				String name = arg.substr(0, pos); // name = "dir"
				String value = arg.substr(pos + 1, arg.npos); // value = "Engine/"
				//              Skips the '='  -^

				for (auto& c : name) // Property names are also single-byte
				{
					if (c >= 65 && c <= 90) c += 32;
				}
				// Values are not edited in any way, as they can be multi-byte UTF-8 characters (pathnames!)

				if (VerifyProperty(name)) { CommandLine::Properties.emplace(std::move(name), std::move(value)); }
				else { Warning("Ignoring unknown property '", name, "'."); }
			}
		}
	}

	static std::vector<String> s_ValidSwitches =
	{
		"rebuild",
		"fatalwarnings",
		"verbose"
	};
	static std::vector<String> s_ValidProperties =
	{
		"dir",
		"config"
	};

	bool VerifySwitch(const String& switchArg)
	{
		auto it = std::find(s_ValidSwitches.begin(), s_ValidSwitches.end(), switchArg);

		return it != s_ValidSwitches.end();
	}

	bool VerifyProperty(const String& propertyName)
	{
		auto it = std::find(s_ValidProperties.begin(), s_ValidProperties.end(), propertyName);

		return it != s_ValidProperties.end();
	}
}

namespace CommandLine
{
	std::map<String, String> Properties;
	std::vector<String> Switches;
}