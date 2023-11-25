///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsTest
// 2019-04-05
// Standard ISO/IEC 114882, C++11
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace utils
{
	namespace test
	{

void ASSERT(bool value);
void RESULT(const char* msg, bool result);
void RESULT(const std::string& msg, bool result);
void WARNING(const char* msg, bool show = true);
void WARNING(const std::string& msg, bool show = true);

	}
}
