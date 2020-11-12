// SparkBuild.Private.BasePlatform
// Copyright 2020 SparkyPotato

// Platform - specific interface

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
	static std::wstring s_LinkerPath;
	static std::wstring s_LibPath;

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
	}

	void Compile(Module& buildModule, std::vector<File*> files)
	{
		std::wstring command = L" ";

		for (auto file : files)
		{
			command += L"\"" + file->Path.wstring() + L"\" ";
		}

		command += LR"(/D"UNICODE" /EHsc /D"_UNICODE" /c /utf-8 /diagnostics:caret /nologo /std:c++17 /MP )";
		command += L"/sourceDependencies\"" + Globals::IntermediatePath.wstring() + L"/DependencyGraph/" + L"\" ";

		command += L"/Fo\"" + Globals::IntermediatePath.wstring() + L"/" +
			ToUTF16(CommandLine::GetProperty("config")) + L"/Build/" + ToUTF16(buildModule.Name) + L"/\" ";

		for (auto& dependency : buildModule.Dependencies)
		{
			fs::path include = Globals::ModuleRegistry[dependency]["Path"].get<String>() + "/Public";
			command += L"/I\"" + include.wstring() + L"/\" ";
		}

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
