///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPath.h
// 2022-04-20
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ctime>

#include <filesystem>
#include <string>

namespace utils
{

std::string GetDateTime(tm a_DateTime);
std::string GetDateTime();
tm GetDateTime(const std::string& a_value);

namespace path
{

std::string GetAppName(const std::string& path);
std::string GetAppNameMain(const std::string& path);

std::filesystem::path GetPathConfig(const std::string& filename);
std::filesystem::path GetPathConfigExc(const std::string& filename);

}

}
