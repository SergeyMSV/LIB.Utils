#include "utilsCryptoAES.h"
#include "utilsTest.h"

#include <string.h>

#include <iostream>
#include <iomanip>

namespace utils
{
	namespace crypto
	{
		namespace AES
		{

void Dump(const char *msg, unsigned char *data, int len)
{
	printf("%8.8s: ", msg);

	for (int i = 0; i < len; i++)
	{
		printf(" %2.2x", data[i]);
	}

	printf("\n");
}

void aes_cypher_128_test()
{
#if 1
	unsigned char buf[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
#else
	unsigned char buf[] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	unsigned char key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
#endif

	printf("\ntAES_CYPHER_128 encrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Encrypt_ECB(tAES_CYPHER_128, buf, sizeof(buf), key);

	printf("\ntAES_CYPHER_128 decrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Decrypt_ECB(tAES_CYPHER_128, buf, sizeof(buf), key);

	printf("Output:\n");
	Dump("data", buf, sizeof(buf));
}

void aes_cypher_192_test()
{
	unsigned char buf[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };

	printf("\ntAES_CYPHER_192 encrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Encrypt_ECB(tAES_CYPHER_192, buf, sizeof(buf), key);

	printf("\ntAES_CYPHER_192 decrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Decrypt_ECB(tAES_CYPHER_192, buf, sizeof(buf), key);

	printf("Output:\n");
	Dump("data", buf, sizeof(buf));
}

void aes_cypher_256_test()
{
	unsigned char buf[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
	
	printf("\ntAES_CYPHER_256 encrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Encrypt_ECB(tAES_CYPHER_256, buf, sizeof(buf), key);

	printf("\ntAES_CYPHER_256 decrypt test case:\n");
	printf("Input:\n");
	Dump("data", buf, sizeof(buf));
	Dump("key ", key, sizeof(key));
	Decrypt_ECB(tAES_CYPHER_256, buf, sizeof(buf), key);

	printf("Output:\n");
	Dump("data", buf, sizeof(buf));
}

	}
}

	namespace unit_test
	{

template<class TKey>
void UnitTest_ECB(const std::string& testName, const TKey& key, const tVectorUInt8& dataRaw, const tVectorUInt8& dataEnc)
{
	tVectorUInt8 DataToEncrypt = dataRaw;

	bool Result = crypto::AES_Encrypt(key, DataToEncrypt) == crypto::tAES_CERR_None;

	tVectorUInt8 DataToDecrypt = DataToEncrypt;

	Result = Result && crypto::AES_Decrypt(key, DataToDecrypt) == crypto::tAES_CERR_None;

	Result = Result &&
		dataRaw == DataToDecrypt &&
		DataToEncrypt == dataEnc;

	utils::test::RESULT(testName.c_str(), Result);
}

template<class TKey>
void UnitTest_ECB(const std::string& testName, const TKey& key, const tVectorUInt8& dataRaw)
{
	tVectorUInt8 DataToEncrypt = dataRaw;

	bool Result = crypto::AES_Encrypt(key, DataToEncrypt) == crypto::tAES_CERR_None;

	tVectorUInt8 DataToDecrypt = DataToEncrypt;

	Result = Result && crypto::AES_Decrypt(key, DataToDecrypt) == crypto::tAES_CERR_None;

	Result = Result &&
		dataRaw == DataToDecrypt &&
		DataToEncrypt != DataToDecrypt;

	utils::test::RESULT(testName.c_str(), Result);
}

template<class TKey>
void UnitTest_CBC(const std::string& testName, const TKey& key, const std::vector<tVectorUInt8>& data)
{
	std::vector<tVectorUInt8> DataToEncrypt = data;

	bool Result = crypto::AES_Encrypt(key, DataToEncrypt[0]) == crypto::tAES_CERR_None;

	tVectorUInt8 IV = DataToEncrypt[0];

	for (std::size_t i = 1; i < data.size() && Result; ++i)
	{
		Result = Result && crypto::AES_Encrypt(key, DataToEncrypt[i], IV) == crypto::tAES_CERR_None;

		IV = DataToEncrypt[i];
	}

	std::vector<tVectorUInt8> DataToDecrypt = DataToEncrypt;

	if (Result)
	{
		Result = Result && crypto::AES_Decrypt(key, DataToDecrypt[0]) == crypto::tAES_CERR_None;
	}

	for (std::size_t i = 1; i < DataToDecrypt.size() && i < DataToEncrypt.size() && Result; ++i)
	{
		IV = DataToEncrypt[i - 1];

		Result = Result && crypto::AES_Decrypt(key, DataToDecrypt[i], IV) == crypto::tAES_CERR_None;
	}

	Result = Result &&
		DataToDecrypt == data &&
		DataToDecrypt != DataToEncrypt;

	utils::test::RESULT(testName.c_str(), Result);
}

void UnitTest_CryptoAES()
{
	std::cout << "\n""utils::crypto::AES" << std::endl;

#ifdef LIB_UTILS_CRYPTO_AES_LOG
	aes_cypher_128_test();
	aes_cypher_192_test();
	aes_cypher_256_test();
#endif//LIB_UTILS_CRYPTO_AES_LOG

	{
		tVectorUInt8 Data{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
		tVectorUInt8 DataENCRYPTED{ 0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a };

		tKey128 Key;
		Key.Field.A = 0x03020100;
		Key.Field.B = 0x07060504;
		Key.Field.C = 0x0B0A0908;
		Key.Field.D = 0x0F0E0D0C;

		UnitTest_ECB("ECB: AES_128", Key, Data, DataENCRYPTED);
	}

	{
		tVectorUInt8 Data{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
		tVectorUInt8 DataENCRYPTED{ 0xdd, 0xa9, 0x7c, 0xa4, 0x86, 0x4c, 0xdf, 0xe0, 0x6e, 0xaf, 0x70, 0xa0, 0xec, 0x0d, 0x71, 0x91 };

		tKey192 Key;
		Key.Field.A = 0x03020100;
		Key.Field.B = 0x07060504;
		Key.Field.C = 0x0B0A0908;
		Key.Field.D = 0x0F0E0D0C;
		Key.Field.E = 0x13121110;
		Key.Field.F = 0x17161514;

		UnitTest_ECB("ECB: AES_192", Key, Data, DataENCRYPTED);
	}

	{
		tVectorUInt8 Data{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
		tVectorUInt8 DataENCRYPTED{ 0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf, 0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89 };

		tKey256 Key;
		Key.Field.A = 0x03020100;
		Key.Field.B = 0x07060504;
		Key.Field.C = 0x0B0A0908;
		Key.Field.D = 0x0F0E0D0C;
		Key.Field.E = 0x13121110;
		Key.Field.F = 0x17161514;
		Key.Field.G = 0x1B1A1918;
		Key.Field.H = 0x1F1E1D1C;

		UnitTest_ECB("ECB: AES_256", Key, Data, DataENCRYPTED);
	}

	{
		auto Test_ECB_AES_128 = [](const std::string& testName, const std::string& dataStr)
		{
			tVectorUInt8 Data(dataStr.cbegin(), dataStr.cend());

			tKey128 Key;
			Key.Field.A = 0x33323130;
			Key.Field.B = 0x37363534;
			Key.Field.C = 0x42413938;
			Key.Field.D = 0x46454443;

			UnitTest_ECB(testName, Key, Data);
		};

		Test_ECB_AES_128("ECB: AES_128 - Data 16 bytes", "QWERTYUIOPASDFGH");
		Test_ECB_AES_128("ECB: AES_128 - Data 32 bytes", "QWERTYUIOPASDFGHqwertyuiopasdfgh");
		Test_ECB_AES_128("ECB: AES_128 - Data 64 bytes", "QWERTYUIOPASDFGHqwertyuiopasdfghÉÖÓÊÅÍÃØÙÇÕÚÔÛÂÀÏÐÎËÄÆÝß×ÑÌÈÒÜÁÞ");
	}

	{
		auto Test_ECB_AES_128 = [](const std::string& testName, const std::string& dataStr)
		{
			tVectorUInt8 Data(dataStr.cbegin(), dataStr.cend());

			tKey128 Key;
			Key.Field.A = 0x33323130;
			Key.Field.B = 0x37363534;
			Key.Field.C = 0x42413938;
			Key.Field.D = 0x46454443;

			bool Result = crypto::AES_Encrypt(Key, Data) == crypto::tAES_CERR_DataSize;

			utils::test::RESULT(testName.c_str(), Result);
		};

		Test_ECB_AES_128("ECB: AES_128 - EncryptECB Data 24 bytes (wrong size)", "QWERTYUIOPASDFGHJKLZXCVB");
	}

	{
		auto Test_ECB_AES_128 = [](const std::string& testName, const tVectorUInt8& data)
		{
			tKey128 Key;
			Key.Field.A = 0x33323130;
			Key.Field.B = 0x37363534;
			Key.Field.C = 0x42413938;
			Key.Field.D = 0x46454443;

			bool Result = crypto::AES_Decrypt(Key, const_cast<tVectorUInt8&>(data)) == crypto::tAES_CERR_DataSize;

			utils::test::RESULT(testName.c_str(), Result);
		};

		Test_ECB_AES_128("ECB: AES_128 - DecryptECB Data 15 bytes (wrong size)", { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee });
	}

	{
		std::vector<std::string> VectorString;
		VectorString.push_back("QWERTYUIOPASDFGH");
		VectorString.push_back("qwertyuiopasdfgh");
		VectorString.push_back("jklzxcvbnm[]{}:;");

		std::vector<tVectorUInt8> Data;

		for (auto i : VectorString)
		{
			tVectorUInt8 LocalVector(i.cbegin(), i.cend());

			Data.push_back(LocalVector);
		}

		tKey128 Key128;
		Key128.Field.A = 0x33323130;
		Key128.Field.B = 0x37363534;
		Key128.Field.C = 0x42413938;
		Key128.Field.D = 0x46454443;

		UnitTest_CBC("CBC: AES_128 - Data 16 bytes", Key128, Data);

		tKey192 Key192;
		Key192.Field.A = 0x33323130;
		Key192.Field.B = 0x37363534;
		Key192.Field.C = 0x42413938;
		Key192.Field.D = 0x46454443;
		Key192.Field.E = 0x4A494847;
		Key192.Field.F = 0x4E4D4C4B;

		UnitTest_CBC("CBC: AES_192 - Data 16 bytes", Key192, Data);

		tKey256 Key256;
		Key256.Field.A = 0x33323130;
		Key256.Field.B = 0x37363534;
		Key256.Field.C = 0x42413938;
		Key256.Field.D = 0x46454443;
		Key256.Field.E = 0x4A494847;
		Key256.Field.F = 0x4E4D4C4B;
		Key256.Field.G = 0x5251504F;
		Key256.Field.H = 0x56555453;

		UnitTest_CBC("CBC: AES_256 - Data 16 bytes", Key192, Data);
	}

	{
		tVectorUInt8 Data{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

		tKey128 Key;
		Key.Field.A = 0x03020100;
		Key.Field.B = 0x07060504;
		Key.Field.C = 0x0B0A0908;
		Key.Field.D = 0x0F0E0D0C;

		tVectorUInt8 IV(15, 0);

		bool Result = crypto::AES_Encrypt(Key, Data, IV) == crypto::tAES_CERR_IVSize;

		utils::test::RESULT("CBC: AES_128 - Data 16 bytes, Encrypt IV 15 bytes (wrong size IV)", Result);

		Result = crypto::AES_Decrypt(Key, Data, IV) == crypto::tAES_CERR_IVSize;

		utils::test::RESULT("CBC: AES_128 - Data 16 bytes, Decrypt IV 15 bytes (wrong size IV)", Result);
	}


	std::cout<<std::endl;
}

	}
}
