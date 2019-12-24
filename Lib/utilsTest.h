///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsTest.h
//
// Standard ISO/IEC 114882, C++17
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 04 05  |
// |      1b    |   2019 09 05  | Updated
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

namespace utils
{
	namespace test
	{

void ASSERT(bool value);
void RESULT(const char* msg, bool result);
void WARNING(const char* msg, bool show = true);

	}
}
