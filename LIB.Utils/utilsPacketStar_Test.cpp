#include "utilsPacketStar.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketStar()
{
	std::cout << "\n""utils::packet::tPacketStar\n";

	typedef utils::packet::tPacket<utils::packet_Star::tFormatStar, tVectorUInt8> tPacketStar;

	{
		tPacketStar Packet;

		tVectorUInt8 PacketVector = Packet.ToVector();
	}

	{
		tVectorUInt8 Data{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar Packet;

		bool Result = false;

		if (tPacketStar::Find(Data, Packet))
		{
			Data = Packet.ToVector();

			Result = tPacketStar::Find(Data, Packet);
		}

		utils::test::RESULT("Test 1", Result);
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar Packet;

		bool Result = false;

		if (tPacketStar::Find(Data, Packet))
		{
			Data = Packet.ToVector();

			Result = tPacketStar::Find(Data, Packet);
		}

		utils::test::RESULT("Test 2", Result);
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
			0x2A, 0x09, 0x00 };

		tPacketStar Packet;

		bool Result = false;

		if (tPacketStar::Find(Data, Packet))
		{
			Data = Packet.ToVector();

			Result = tPacketStar::Find(Data, Packet);
		}

		utils::test::RESULT("Test 3", Result);
	}
}

}