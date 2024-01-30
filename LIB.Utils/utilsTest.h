///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsTest
// 2019-04-05
// Standard ISO/IEC 114882, C++11
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace utils
{
	namespace test
	{

void ASSERT(bool value);
void RESULT(const char* msg, bool result);
void RESULT(const std::string& msg, bool result);
void WARNING(const char* msg, bool show = true);
void WARNING(const std::string& msg, bool show = true);

template <class T>
std::string ToStringHEX(const T& val, bool space)
{
	std::stringstream SStr;
	std::for_each(val.begin(), val.end(), [&SStr, &space](std::uint8_t val) { SStr << std::setfill('0') << std::setw(2) << std::hex << (int)val << (space ? " " : ""); });
	return SStr.str();
}
	}
}
