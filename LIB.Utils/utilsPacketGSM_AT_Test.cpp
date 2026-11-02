#include <modGsmCinterionBGS2R1_PacketAT.h>

#include <utilsConverter.h>
#include <utilsTest.h>

#include <string.h>

#include <iostream>
#include <iomanip>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketAT
		{

void UnitTests()
{
	std::cout<<"\n\n""mod::CinterionBGS2R1::PacketAT::UnitTests"<<std::endl;

	//Test Find: ^SYSSTART
	{
		char Data[] = "^SYSSTART\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 9;

		utils::test::RESULT("Find: ^SYSSTART", Result);
	}

	//Test Find: +CREG: 0,1
	{
		char Data[] = "+CREG: 0,1\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 10;

		utils::test::RESULT("Find: +CREG: 0,1", Result);
	}

	//Test Find: +CREG: 0,1 with Rubbish
	{
		char Data[] = "\xd\xa+CREG: 0,1\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 10;

		utils::test::RESULT("Find: +CREG: 0,1 with Rubbish", Result);
	}

	//Test Find: AT+CREG?
	{
		char Data[] = "AT+CREG?\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 8;

		utils::test::RESULT("Find: AT+CREG?", Result);
	}

	//Test Find: OK
	{
		char Data[] = "OK\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 2;

		utils::test::RESULT("Find: OK", Result);
	}

	//Test Find: +CME ERROR: 234
	{
		char Data[] = "+CME ERROR: 234\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 15;

		utils::test::RESULT("Find: +CME ERROR: 234", Result);
	}

	//Test Find: +CMS ERROR: 234
	{
		char Data[] = "+CMS ERROR: 234\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 15;

		utils::test::RESULT("Find: +CMS ERROR: 234", Result);
	}

	//Test Find: SMS
	{
		char Data[] = "\xd\xa""01020101401041051\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 17;

		utils::test::RESULT("Find: SMS", Result);
	}

	//Test Find: SMS prompt >
	{
		char Data[] = "\xd\xa"">";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 1;

		utils::test::RESULT("Find: SMS prompt >", Result);
	}

	//Test Find: "NO CARRIER"
	{
		char Data[] = "\xd\xa""NO CARRIER\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 10;

		utils::test::RESULT("Find: NO CARRIER", Result);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//Messages

	//Test: COPS
	{
		char Data[] = "+COPS: 0";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCOPS MsgCOPS;

		bool Result = tMsgCOPS::TryParse(PacketVector, MsgCOPS);

		utils::test::RESULT("+COPS: 0", Result && MsgCOPS.GetMode() == 0);
	}

	{
		char Data[] = "+COPS: 0,0,\"MegaFon RUS\"";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCOPS MsgCOPS;

		bool Result = tMsgCOPS::TryParse(PacketVector, MsgCOPS);

		utils::test::RESULT("+COPS: 0,0,\"MegaFon RUS\"", Result && MsgCOPS.GetMode() == 0 && MsgCOPS.GetFormat() == 0 && MsgCOPS.GetOperatorName() == "\"MegaFon RUS\"");
	}

	//Test: CREG
	{
		char Data[] = "+CREG: 0";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCREG MsgCREG;

		bool Result = tMsgCREG::TryParse(PacketVector, MsgCREG);

		utils::test::RESULT("+CREG: 0", !Result && MsgCREG.GetRegStatus() == -1);
	}

	{
		char Data[] = "+CREG: 0,1";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCREG MsgCREG;

		bool Result = tMsgCREG::TryParse(PacketVector, MsgCREG);

		utils::test::RESULT("+CREG: 0,1", Result && MsgCREG.GetRegStatus() == 1);
	}

	//Test: CSQ
	{
		char Data[] = "+CSQ: 1,2";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCSQ MsgCSQ;

		bool Result = tMsgCSQ::TryParse(PacketVector, MsgCSQ);

		utils::test::RESULT("+CSQ: 1,2", Result && MsgCSQ.GetRSSI() == 1 && MsgCSQ.GetBER() == 2);
	}

	{
		char Data[] = "+CSQ: 14,99";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgCSQ MsgCSQ;

		bool Result = tMsgCSQ::TryParse(PacketVector, MsgCSQ);

		utils::test::RESULT("+CSQ: 14,99", Result && MsgCSQ.GetRSSI() == 14 && MsgCSQ.GetBER() == 99);
	}

	//Test: SMONC
	{
		char Data[] = "^SMONC: 250,02,13A6,F96A,61,658,34,30,50,250,02,13A6,A751,57,1022,38,34,34,250,02,13A6,40A3,10,643,28,24,44,250,02,13A6,A756,16,630,24,20,40,250,02,13A6,F964,44,113,25,21,21,250,02,13A6,A750,57,1017,23,19,19,250,02,13A6,A752,57,1020,19,15,15";
		std::vector<char> PacketVector(Data, Data + strlen(Data));

		tMsgSMONC MsgSMONC;

		bool Result = tMsgSMONC::TryParse(PacketVector, MsgSMONC);

		utils::test::RESULT("^SMONC: ...", Result);
	}


	std::cout<<std::endl;
}

		}
	}
}
