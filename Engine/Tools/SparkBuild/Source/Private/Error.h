/*
	SparkBuild.Private.Error
	Copyright 2020 SparkyPotato

	Error emitter
*/

#pragma once

#include <string>
#include <map>

class Error
{
public:
	template <typename ...Args>
	Error(const std::wstring& errorCode, const Args&... args)
		: m_ErrorCode(errorCode)
	{
		std::wstring error = L"Error: " + errorCode;

		wprintf(error.c_str(), args...);
	}

	template <typename ...Args>
	Error(const std::string& errorCode, const Args&... args)
		: m_ErrorCodeS(errorCode)
	{
		std::string error = "Error: " + errorCode;

		printf(error.c_str(), args...);
	}

	int GetReturnValue() { return m_ErrorCodes[m_ErrorCode].first; }
	void PrintDiagnostic() { wprintf(L"%s \n", m_ErrorCodes[m_ErrorCode].second.c_str()); }

private:
	std::wstring m_ErrorCode;
	std::string m_ErrorCodeS;

	static std::map<std::wstring, std::pair<int, std::wstring>> m_ErrorCodes;
};
