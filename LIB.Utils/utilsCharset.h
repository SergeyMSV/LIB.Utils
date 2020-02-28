///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsCharset.h
//
// Standard ISO/IEC 114882, C++17
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 10 28  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstddef>
//#include <stdlib.h>
//#include <string.h>
//
//#include <vector>

namespace utils
{
	namespace charset
	{

int Convert_UTF8_to_Windows1251(const char* utf8, char* windows1251, std::size_t n);

//enum tISO8859
//{
//	tISO8859_1,
//	tISO8859_5,
//};
//
//std::string ToCP866(tISO8859 codePage, const std::string& data);

	}
}
