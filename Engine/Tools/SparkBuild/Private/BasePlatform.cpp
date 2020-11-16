// SparkBuild.Private.BasePlatform
// Copyright 2020 SparkyPotato

// Platform-specific interface

#include "BasePlatform.h"

#include "Error.h"
#include "SourceTree.h"

// SparkBuild is a small tool with very little platform-specific functionality used.
// This is why we put all platform ifdefs in a single-file.

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

namespace BasePlatform
{
	static std::wstring s_CompilerPath;
	static std::wstring s_BaseCompilerCommand;

	static std::wstring s_LinkerPath;
	static std::wstring s_BaseLinkerCommand;

	static std::wstring s_LibPath;
	static std::wstring s_BaseLibCommand;

	bool SetWorkingDirectory(const String& directory)
	{
		wchar_t dir[5000];
		MultiByteToWideChar(CP_UTF8, 0, directory.c_str(), -1, dir, 5000);

		return SetCurrentDirectoryW(dir);
	}

	void SetupCompiler()
	{
		// That is *always* going to be where vswhere is located
		const char* vswhere = R"("C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe")";

		// Call vswhere and store the installation path in a file 'tmp'
		system((std::string(vswhere) + " -latest -property installationPath > tmp").c_str());
		std::string vsPath;
		getline(std::ifstream("tmp"), vsPath);
		fs::remove("tmp"); // Remove temporary

		// Get the default MSVC toolset version and store it in toolsetVersion
		std::string toolPath = vsPath + "/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt";
		std::string toolsetVersion;
		std::ifstream(toolPath) >> toolsetVersion;

		BasePlatform::Output("Using MSVC toolchain version ", toolsetVersion, ".");

		// Store the path for cl, link, and lib
		s_CompilerPath = ToUTF16(vsPath) + L"\\VC\\Tools\\MSVC\\" + ToUTF16(toolsetVersion) + L"\\bin\\Hostx64\\x64\\cl.exe";
		s_LinkerPath = ToUTF16(vsPath) + L"\\VC\\Tools\\MSVC\\" + ToUTF16(toolsetVersion) + L"\\bin\\Hostx64\\x64\\link.exe";
		s_LibPath = ToUTF16(vsPath) + L"\\VC\\Tools\\MSVC\\" + ToUTF16(toolsetVersion) + L"\\bin\\Hostx64\\x64\\lib.exe";

		s_BaseCompilerCommand = LR"( /D"UNICODE" /EHsc /D"_UNICODE" /W4 /c /utf-8 /diagnostics:caret /nologo /std:c++17 /MP )";
		s_BaseLinkerCommand = LR"( /NOLOGO )";
		s_BaseLibCommand = LR"( /NOLOGO /DEF )";

		std::string config = CommandLine::GetProperty("config");
		if (config == "Debug")
		{
			s_BaseCompilerCommand += LR"( /D"CONFIG_DEBUG" /Od /Z7 )";
			s_BaseLinkerCommand += LR"( /DEBUG /INCREMENTAL )";
		}
		else if (config == "Development")
		{
			s_BaseCompilerCommand += LR"( /D"CONFIG_DEVELOPMENT" /O2 /Z7 )";
			s_BaseLinkerCommand += LR"( /DEBUG /INCREMENTAL )";
		}
		else
		{
			s_BaseCompilerCommand += LR"( /D"CONFIG_RELEASE" /O2 /GL )";
			s_BaseLinkerCommand += LR"( /LTCG )";
			s_BaseLibCommand += LR"( /LTCG )";
		}
	}

	void StartCompiler(std::wstring& command)
	{
		STARTUPINFO startupInfo;
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(processInfo));

		bool startup = CreateProcessW
		(
			s_CompilerPath.c_str(),
			command.data(),
			nullptr, nullptr,
			true,
			NORMAL_PRIORITY_CLASS,
			nullptr,
			nullptr,
			&startupInfo,
			&processInfo
		);

		if (!startup)
		{
			Error("Failed to start compiler! Error code: ", GetLastError());
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		DWORD code;
		GetExitCodeProcess(processInfo.hProcess, &code);

		if (code != 0)
		{
			Error("Compiler error.");
		}

		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}

	void StartLinker(std::wstring& command)
	{
		STARTUPINFO startupInfo;
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(processInfo));

		bool startup = CreateProcessW
		(
			s_LinkerPath.c_str(),
			command.data(),
			nullptr, nullptr,
			true,
			NORMAL_PRIORITY_CLASS,
			nullptr,
			nullptr,
			&startupInfo,
			&processInfo
		);

		if (!startup)
		{
			Error("Failed to start linker! Error code: ", GetLastError());
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		DWORD code;
		GetExitCodeProcess(processInfo.hProcess, &code);

		if (code != 0)
		{
			Error("Linker error.");
		}

		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}

	void StartLib(std::wstring& command)
	{
		STARTUPINFO startupInfo;
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);

		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(processInfo));

		bool startup = CreateProcessW
		(
			s_LibPath.c_str(),
			command.data(),
			nullptr, nullptr,
			true,
			NORMAL_PRIORITY_CLASS,
			nullptr,
			nullptr,
			&startupInfo,
			&processInfo
		);

		if (!startup)
		{
			Error("Failed to start lib! Error code: ", GetLastError());
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);

		DWORD code;
		GetExitCodeProcess(processInfo.hProcess, &code);

		if (code != 0)
		{
			Error("Lib error.");
		}

		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}

	void Compile(Module& buildModule, std::vector<File*> files)
	{
		std::wstring command = s_BaseCompilerCommand;

		// Add Dependency/Public folders to include path
		for (auto& dependency : buildModule.Dependencies)
		{
			fs::path include = Globals::ModuleRegistry[dependency]["Path"].get<String>() + "/Public";
			command += L"/I\"" + include.wstring() + L"/\" ";
		}

		// Add module's folder into its include path
		fs::path include = Globals::ModuleRegistry[buildModule.Name]["Path"].get<String>();
		command += L"/I\"" + include.wstring() + L"/\" ";

		// Define BUILD_ModuleName while building a module
		command += L"/D\"BUILD_" + ToUTF16(buildModule.Name) + L"\" ";

		for (auto file : files)
		{
			command += L"\"" + file->Path.wstring() + L"\" ";
		}

		// Output directory for compiled object files
		command += L"/Fo\"" + Globals::IntermediatePath.wstring() + L"/" +
			ToUTF16(CommandLine::GetProperty("config")) + L"/" + ToUTF16(buildModule.Name) + L"/\" ";

		// Where to output header file dependencies
		command += L"/sourceDependencies\"" + Globals::IntermediatePath.wstring() + L"/DependencyGraph/" + L"\" ";

		StartCompiler(command);
	}

	void GenerateExports(Module& buildModule)
	{
		std::wstring command = s_BaseLibCommand;

		// Add all files to be exported
		fs::directory_iterator objPath(Globals::IntermediatePath.wstring() + L"/" +
			ToUTF16(CommandLine::GetProperty("config")) + L"/" + ToUTF16(buildModule.Name));
		for (auto& entry : objPath)
		{
			command += L"\"" + entry.path().wstring() + L"\" ";
		}

		command += L"/OUT:" + Globals::BinariesPath.wstring() + L"/" + ToUTF16(CommandLine::GetProperty("config")) +
			+L"/" + ToUTF16(buildModule.Name) + L"/" + ToUTF16(buildModule.Name) + L".lib";

		StartLib(command);
	}

	void Link(Module& buildModule)
	{
		std::wstring command = s_BaseLinkerCommand;

		// Change later when executables are produced
		if (!buildModule.Executable) { command += L"/DLL "; }

		// Add import libraries for all dependencies
		for (auto& dependency : buildModule.Dependencies)
		{
			fs::path path = Globals::ModuleRegistry[dependency]["BinaryPath"].get<String>() + "/" + CommandLine::GetProperty("config") 
				+ "/" + dependency + "/" + dependency + ".lib";
			if (fs::exists(path))
			{
				command += L"\"" + path.wstring() + L"\" ";
			}
		}

		// Add all files to be linked
		fs::directory_iterator objPath(Globals::IntermediatePath.wstring() + L"/" +
			ToUTF16(CommandLine::GetProperty("config")) + L"/" + ToUTF16(buildModule.Name));
		for (auto& entry : objPath)
		{
			command += L"\"" + entry.path().wstring() + L"\" ";
		}

		// Add exports file
		if (!buildModule.Executable)
		{
			command += L"\"" + Globals::BinariesPath.wstring() + L"/" + ToUTF16(CommandLine::GetProperty("config")) +
				+L"/" + ToUTF16(buildModule.Name) + L"/" + ToUTF16(buildModule.Name) + L".exp" + L"\" ";
		}

		command += L"/OUT:" + Globals::BinariesPath.wstring() + L"/" + ToUTF16(CommandLine::GetProperty("config"))
			+ L"/" + ToUTF16(buildModule.Name) + L"/";

		if (!buildModule.Executable)
		{
			command += ToUTF16(buildModule.Name) + L".dll";
		}
		else
		{
			command += L"Executable/" + ToUTF16(buildModule.Name) + L".exe";
		}

		StartLinker(command);

		if (buildModule.Executable)
		{
			buildModule.Executable = false;
			Link(buildModule);
			buildModule.Executable = true;
		}
	}

	String ToUTF8(const wchar_t* string)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, string, -1, nullptr, 0, nullptr, nullptr);
		String temp(size, 0);

		WideCharToMultiByte(CP_UTF8, 0, string, -1, &temp[0], size, nullptr, nullptr);
		temp.pop_back(); // Fixes a weird issue where we have double nulls

		// The double nulls are a problem because:
		// Consider string L"AB". This would be represented in memory as
		// [ 'A', 'B', '\0' ] - doesn't matter if it was in a std::wstring or a wchar_t array.
		//   ---  ---         : the underlined characters are counted in wstring.size(). 
		// What WideCharToMultiByte is doing is that it is storing L"AB" as
		// [ 'A', 'B', '\0', '\0' ] - inside of the std::string.
		//   ---  ---   ---         : where the underlined characters are also considered in the string.
		// If we now appended 'C' onto the string, it would become
		// [ 'A', 'B', '\0', 'C', '\0' ] - and you can clearly see the problem.
		//   ---  ---  ----  ---         : where (again) the underlined characters are counted.

		return temp;
	}

	std::wstring ToUTF16(const String& string)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(string.c_str()), (int)string.size(), nullptr, 0);
		std::wstring temp(size, 0);

		MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &temp[0], (int)temp.capacity());

		return temp;
	}
}

#endif
