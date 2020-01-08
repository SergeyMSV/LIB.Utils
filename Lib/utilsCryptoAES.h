///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsCryptoAES.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2018 01 16  |
// |      2     |   2019 09 15  | Added const
// |      3     |   2019 10 08  | 
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include "utilsKey.h"

#include <cstddef>
#include <cassert>
#include <vector>

namespace utils
{
	namespace crypto
	{

enum tAES_CERR
{
	tAES_CERR_None,
	tAES_CERR_DataSize,
	tAES_CERR_IVSize,

	tAES_CERR_Unknown = 0xFF,
};

		namespace AES
		{

enum tAES_CYPHER//used as an index
{
	tAES_CYPHER_128,
	tAES_CYPHER_192,
	tAES_CYPHER_256,
};

template <class TKey>
constexpr bool IsKeyValid()
{
	return TKey::type_key && (TKey::tKeyValue::Size == 4 || TKey::tKeyValue::Size == 6 || TKey::tKeyValue::Size == 8);
}

template <class TKey>
tAES_CYPHER GetMode()
{
	AES::tAES_CYPHER Mode = static_cast<AES::tAES_CYPHER>((TKey::tKeyValue::Size - 4) / 2);

	assert(Mode >= 0 && Mode <= 2);

	return Mode;
}

tAES_CERR VerifyInputECB(tAES_CYPHER mode, const tVectorUInt8& data);
void Encrypt_ECB(tAES_CYPHER mode, unsigned char* data, std::size_t len, const unsigned char* key);//Electronic Codebook (ECB)
void Decrypt_ECB(tAES_CYPHER mode, unsigned char* data, std::size_t len, const unsigned char* key);//Electronic Codebook (ECB)

tAES_CERR VerifyInputCBC(tAES_CYPHER mode, const tVectorUInt8& data, const tVectorUInt8& iv);
void Encrypt_CBC(tAES_CYPHER mode, unsigned char* data, std::size_t len, const unsigned char* key, const unsigned char* iv);
void Decrypt_CBC(tAES_CYPHER mode, unsigned char* data, std::size_t len, const unsigned char* key, const unsigned char* iv);
		}

//Electronic Codebook (ECB)
template <class TKey>
typename std::enable_if<AES::IsKeyValid<TKey>(), tAES_CERR>::type AES_Encrypt(const TKey& key, tVectorUInt8& data)
{
	AES::tAES_CYPHER Mode = AES::GetMode<TKey>();

	tAES_CERR CERR = AES::VerifyInputECB(Mode, data);

	if (CERR == tAES_CERR_None)
	{
		tVectorUInt8 KeyVector = ToVector(key);

		AES::Encrypt_ECB(Mode, data.data(), data.size(), KeyVector.data());
	}

	return CERR;
}

//Electronic Codebook (ECB)
template <class TKey>
typename std::enable_if<AES::IsKeyValid<TKey>(), tAES_CERR>::type AES_Decrypt(const TKey& key, tVectorUInt8& data)
{
	AES::tAES_CYPHER Mode = AES::GetMode<TKey>();

	tAES_CERR CERR = AES::VerifyInputECB(Mode, data);

	if (CERR == tAES_CERR_None)
	{
		tVectorUInt8 KeyVector = ToVector(key);

		AES::Decrypt_ECB(Mode, data.data(), data.size(), KeyVector.data());
	}

	return CERR;
}

//Cipher Block Chaining (CBC)
template <class TKey>
typename std::enable_if<AES::IsKeyValid<TKey>(), tAES_CERR>::type AES_Encrypt(const TKey& key, tVectorUInt8& data, const tVectorUInt8& iv)
{
	AES::tAES_CYPHER Mode = AES::GetMode<TKey>();

	tAES_CERR CERR = AES::VerifyInputCBC(Mode, data, iv);

	if (CERR == tAES_CERR_None)
	{
		tVectorUInt8 KeyVector = ToVector(key);

		AES::Encrypt_CBC(Mode, data.data(), data.size(), KeyVector.data(), iv.data());
	}

	return CERR;
}

//Cipher Block Chaining (CBC)
template <class TKey>
typename std::enable_if<AES::IsKeyValid<TKey>(), tAES_CERR>::type AES_Decrypt(const TKey& key, tVectorUInt8& data, const tVectorUInt8& iv)
{
	AES::tAES_CYPHER Mode = AES::GetMode<TKey>();

	tAES_CERR CERR = AES::VerifyInputCBC(Mode, data, iv);

	if (CERR == tAES_CERR_None)
	{
		tVectorUInt8 KeyVector = ToVector(key);

		AES::Decrypt_CBC(Mode, data.data(), data.size(), KeyVector.data(), iv.data());
	}

	return CERR;
}

	}
}
