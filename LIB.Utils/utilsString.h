///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsString.h
// 2023-07-07
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ctime>

#include <string>
#include <optional>

namespace utils
{
	namespace string
	{

template<int Size>
class tStringTruncated
{
	const std::string m_StrTrunc;
	int m_TruncIndex;

public:
	tStringTruncated() = delete;
	explicit tStringTruncated(const std::string& a_str)
		: m_StrTrunc(a_str), m_TruncIndex(0)
	{

	}

	std::string GetNext()
	{
		const std::string StrNum = " (" + std::to_string(m_TruncIndex) + ")";
		std::string StrTrunc = m_StrTrunc;

		if (m_TruncIndex)
			StrTrunc += StrNum;

		if (StrTrunc.size() > Size)
		{
			std::string StrPostfix = m_TruncIndex > 0 ? "..." + StrNum : "...";
			StrTrunc = StrTrunc.substr(0, Size - StrPostfix.size()) + StrPostfix;
		}

		++m_TruncIndex;
		return StrTrunc;
	}
};

std::optional<std::string> Replace(const std::string& str, const std::string& from, const std::string& to);

std::string GetStringEnding(const std::string& pattern, const std::string& str);

	}
}
