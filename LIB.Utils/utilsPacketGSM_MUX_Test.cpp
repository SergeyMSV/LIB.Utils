#include <modGsmCinterionBGS2R1_PacketMUX.h>

#include <utilsConverter.h>
#include <utilsTest.h>

#include <string.h>

#include <iostream>
#include <iomanip>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketMUX
		{

void UnitTests()
{
	std::cout<<"\n\n""mod::CinterionBGS2R1::PacketMUX::UnitTests"<<std::endl;

	//Test ToVector - TryParse
	{
		tPacket Packet(tPacketFieldControlFrameType_SABM, true, 4);

		std::vector<char> PacketVector = Packet.ToVector();

		tPacket Packet_Decoded;

		bool Result = tPacket::TryParse(PacketVector, Packet_Decoded);

		utils::test::RESULT("ToVector - TryParse", Result);
	}

	//Test Find: packet1
	{
		char Data[] = { 0xF9, 0x03, 0x73, 0x01, 0xD7, 0xF9 };

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 6;

		utils::test::RESULT("Find: packet", Result);
	}

	//Test Find: packet1 + rubbish
	{
		char Data[] = { 0xF9, 0xF9, 0xF9, 0x03, 0x73, 0x01, 0xD7, 0xF9, 0xF9, 0x03, 0xF9 };

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 6;

		utils::test::RESULT("Find: packet + rubbish", Result);
	}

	//Test Find: packet2 F9 13 73 01 5D F9
	{
		char Data[] = { 0xF9, 0x13, 0x73, 0x01, 0x5D, 0xF9 };

		std::vector<char> DataVector(Data, Data + sizeof(Data));
		std::vector<char> PacketVector = FindPacket(DataVector);

		bool Result = PacketVector.size() == 6;

		utils::test::RESULT("Find: packet2", Result);
	}

	//Test Parse: F9 03 73 01 D7 F9
	{
		char Data[] = { 0xF9, 0x03, 0x73, 0x01, 0xD7, 0xF9 };

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		tPacket Packet_Decoded;

		bool Result = tPacket::TryParse(DataVector, Packet_Decoded);

		utils::test::RESULT("Parse: F9 03 73 01 D7 F9", Result);
	}

	//Test Make: UIH ToVector-TryParse
	{
		char Data[] = "ABC6";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		tPacket Packet(tPacketFieldControlFrameType_UIH, true, 4, DataVector);

		std::vector<char> PacketVector = Packet.ToVector();

		tPacket Packet_Decoded;

		bool Result = tPacket::TryParse(PacketVector, Packet_Decoded);

		utils::test::RESULT("Make: UIH ToVector-TryParse", Result);
	}


	std::cout<<std::endl;
}

		}
	}
}
