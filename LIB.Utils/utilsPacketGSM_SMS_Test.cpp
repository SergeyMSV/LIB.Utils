#include <modGsmCinterionBGS2R1_PacketSMS.h>

#include <utilsConverter.h>
#include <utilsTest.h>

#include <string.h>

#include <iostream>
#include <iomanip>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketSMS
		{

std::vector<char> EncodePduPhoneNumber(const std::vector<char>& phoneNumber);

unsigned char GetPduTPValidityPeriod(unsigned int value);

void UnitTests()
{
	std::cout<<"\n\n""mod::CinterionBGS2R1::PacketSMS::UnitTests"<<std::endl;

	//Test GetPduPayload
	{
		char Data[] = "07919762020033F1040B919752187129F400F4619072715202210E5F640000002001030000000063AE";

		std::vector<char> DataVector(Data, Data + sizeof(Data) - 1);
		std::vector<char> PacketVector = GetPduPayload(DataVector);

		bool Result = PacketVector.size() == 14;

		utils::test::RESULT("GetPduPayload", Result);
	}

	//Test PhoneNumber
	{
		char Data[] = "71234567890";

		std::vector<char> DataVector(Data, Data + sizeof(Data) - 1);
		std::vector<char> PacketVector = EncodePduPhoneNumber(DataVector);

		char Data2[] = "1732547698F0";
		std::vector<char> Payload(Data2, Data2 + sizeof(Data2) - 1);

		bool Result = PacketVector == Payload;

		utils::test::RESULT("PhoneNumber 7...", Result);
	}

	//Test SMS Body
	{
		char Data[] = "+79150455346";
		char Data2[] = "ASWQ";

		std::vector<char> DataVector(Data, Data + sizeof(Data) - 1);
		std::vector<char> Payload(Data2, Data2 + sizeof(Data2) - 1);
		std::vector<char> PacketVector = GetSmsBody(DataVector, Payload, 10);

		bool Result = PacketVector.size() == 39;

		utils::test::RESULT("SMS Body", Result);
	}

	//Test SMS Body Wrong Phone Number
	{
		char Data[] = "79150455346";
		char Data2[] = "ASWQ";

		std::vector<char> DataVector(Data, Data + sizeof(Data) - 1);
		std::vector<char> Payload(Data2, Data2 + sizeof(Data2) - 1);
		std::vector<char> PacketVector = GetSmsBody(DataVector, Payload, 10);

		bool Result = PacketVector.size() == 1;

		utils::test::RESULT("SMS Body Wrong Phone Number", Result);
	}


	std::cout<<std::endl;
}

		}
	}
}
