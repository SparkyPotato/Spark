/*
	SparkBuild.Private.Globals
	Copyright 2020 SparkyPotato

	Global definitions
*/

#include "Globals.h"
#include "Error.h"

namespace Globals
{
	void ParseCommandLine(int argc, wchar_t** argv);

	void Setup(int argc, wchar_t** argv)
	{
		/*
			Set working directory to engine root (where Spark.sln/xcworkspace is located) 
			- this may not work if we have a symlink for the executable and it is called through that
		*/
		String executablePath = std::filesystem::path(argv[0]).parent_path().string();
		executablePath += "/../../../";
		BasePlatform::SetWorkingDirectory(executablePath);

		ParseCommandLine(argc, argv);

		std::string verbose = std::filesystem::absolute(executablePath).string();
		std::replace(verbose.begin(), verbose.end(), '\\', '/'); // Format the path a bit better before displaying it

		Verbose("Set working directory to '", verbose, "'.");

		Verbose("Switches are: ");
		for (auto& switchArg : CommandLine::Switches)
		{
			Verbose(false, String("    "), switchArg);
		}
		Verbose(false, String(""));

		Verbose("Properties are: ");
		for (auto& property : CommandLine::Properties)
		{
			Verbose(false, String("    "), property.first, ": ", property.second);
		}
		Verbose(false, String(""));

		if (CommandLine::GetProperty("dir").empty())
		{
			auto e = Error("No project directory property found"); // SparkBuild needs a project directory location
		}
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
				String switchArg = arg.substr(1, arg.size() - 2); // Remove the preceding '-'
				//                                            ^- Doesn't copy the terminating null so that string comparison works

				for (auto& c : switchArg) // All switches are single-byte, so they can be lowercase-ified with simple addition
				{
					if (c >= 65 && c <= 90) c += 32;
				}

				if (VerifySwitch(switchArg)) { CommandLine::Switches.emplace_back(std::move(switchArg)); }
				else { auto w = Warning("Ignoring unknown switch '", switchArg, "'."); }
			}
			else
			{
				size_t pos = arg.find('=', 0); // Find the '=' separator between the property and its value

				if (pos == arg.npos)
				{
					auto e = Error("Invalid property specifier: ", arg); // Invalid property if we don't find the '='
				}

				// If the argument is "dir=Engine/"
				String name = arg.substr(0, pos); // name = "dir"
				String value = arg.substr(pos + 1, arg.npos); // value = "Engine/"
				//                              ^- Skips the '='

				for (auto& c : name) // Property names are also single-byte
				{
					if (c >= 65 && c <= 90) c += 32;
				}
				// Values are not edited in any way, as they can be multi-byte UTF-8 characters (pathnames!)

				if (VerifyProperty(name)) { CommandLine::Properties.emplace(std::move(name), std::move(value)); }
				else { auto w = Warning("Ignoring unknown property '", name, "'."); }
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
		"dir"
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
