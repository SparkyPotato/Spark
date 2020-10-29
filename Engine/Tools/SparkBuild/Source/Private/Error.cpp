/*
	SparkBuild.Private.Error
	Copyright 2020 SparkyPotato

	Error emitter
*/

#include "Error.h"

std::map<std::wstring, std::pair<int, std::wstring>> Error::m_ErrorCodes;

Error::Error(const std::wstring& errorCode)
	: m_ErrorCode(errorCode)
{
	wprintf(L"Error: %s", m_ErrorCode.c_str());
}
