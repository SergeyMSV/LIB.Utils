///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsCryptoRsa32.h
//
// Standard ISO/IEC 114882, C++98
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2011 10 25  |
// |      2     |   2016 02 02  | Sligntly modified
// |      3     |   2017 03 16  | Placed into utils::crypto::...
// |            |               | Introduced libConfig.h
// |      4     |   2018 01 16  | Introduced namesapce RSA32 instead of class tRsa
// |      5     |   2019 10 10  | Update
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include "utilsKey.h"

#include <cstddef>

#include <vector>

namespace utils
{
	namespace crypto
	{

std::vector<int> RSA32_Encrypt(std::vector<int>& msg, tKey32 key, tKey32 mod);
std::vector<int> RSA32_Decrypt(std::vector<int>& msg, tKey32 key, tKey32 mod);

	}
}
