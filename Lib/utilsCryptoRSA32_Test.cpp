#include "utilsBase.h"
#include "utilsCryptoRSA32.h"
#include "utilsTest.h"

#include <string.h>

#include <string>

#include <iostream>
#include <iomanip>

namespace utils
{
	namespace unit_test
	{

std::vector<int> ToVector32(const char* data, std::size_t dataSize)
{
	std::vector<int> Vector32;

	std::size_t Size = dataSize / 4;

	if ((dataSize % 4) > 0)
	{
		Size++;
	}

	std::size_t Index = 0;

	for (std::size_t i = 0; i < Size; ++i)
	{
		int Value = 0;

		Value |= (unsigned char)data[Index++];

		if (Index < dataSize)
		{
			Value |= (unsigned char)data[Index++] << 8;
		}

		if (Index < dataSize)
		{
			Value |= (unsigned char)data[Index++] << 16;
		}

		if (Index < dataSize)
		{
			Value |= (unsigned char)data[Index++] << 24;
		}

		Vector32.push_back(Value);
	}

	return Vector32;
}

std::string ToString(std::vector<int>& data)
{
	std::string Vector32;

	for (int i = 0; i < data.size(); ++i)
	{
		Vector32.push_back(data[i] & 0xFF);
		Vector32.push_back((data[i] >> 8) & 0xFF);
		Vector32.push_back((data[i] >> 16) & 0xFF);
		Vector32.push_back((data[i] >> 24) & 0xFF);
	}

	return Vector32;
}

void UnitTest_CryptoRSA32()
{
	std::cout<<"\n""utils::crypto::RSA32"<<std::endl;

	{
		tKey32 PublicKeyN;
		PublicKeyN.Field.A = 0x015ac7bb;

		tKey32 PublicKeyE;
		PublicKeyE.Field.A = 0x00009467;

		std::string DataStr = "`1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./~!@##$%^^&*()_+QWERTYUIOPASDFGHJKLZXCVBNMöóêåíãøùçõúôûâàïðîëäæýÿ÷ñìèòüáþÉÖÓÊÅÍÃØÙÇÕÚÔÛÂÀÏÐÎËÄÆÝß×ÑÌÈÒÜÁÞ";

		std::vector<int> DataVector = ToVector32(DataStr.data(), DataStr.size());

		std::vector<int> EncryptedMsg = utils::crypto::RSA32_Encrypt(DataVector, PublicKeyE, PublicKeyN);

		std::vector<int> DecryptedMsg = utils::crypto::RSA32_Decrypt(EncryptedMsg, PublicKeyE, PublicKeyN);

		std::string DataStr2 = ToString(DecryptedMsg);

		//std::cout << DataStr << std::endl;
		//std::cout << DataStr2 << std::endl;

		bool Result = 
			DataVector == DecryptedMsg &&
			EncryptedMsg != DecryptedMsg;

		utils::test::RESULT("Parse: Just a packet", Result);
	}


	std::cout<<std::endl;
}

	}
}
