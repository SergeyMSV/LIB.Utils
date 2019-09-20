///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsTest
//
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 04 05  |
// |      1b    |   2019 09 05  | Updated
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LIB_UTILS_TEST_H
#define LIB_UTILS_TEST_H

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

#endif//LIB_UTILS_TEST_H
