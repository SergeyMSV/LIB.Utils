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

std::optional<std::string> Replace(const std::string& str, const std::string& from, const std::string& to);

std::string GetStringEnding(const std::string& pattern, const std::string& str);

	}
}
