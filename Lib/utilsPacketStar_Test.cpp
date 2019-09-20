#include "utilsPacketStar.h"
//#include "utilsTest.h"
#include <iostream>//Replace with utilsTest.h


namespace utils
{
	namespace unit_test
	{

void UnitTest_PacketStar()
{
	typedef utils::packet::tPacket<utils::packet_Star::tFormatStar, utils::packet::tPayloadCommon> tPacketStar;

	{
		tPacketStar Packet;

		//Packet.

		tVectorUInt8 PacketVector = Packet.ToVector();
	}

	{
		tVectorUInt8 Data{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar Packet;

		if (tPacketStar::TryParse(Data, Packet))
		{
			std::cout << "tPacketStar::TryParse() OK\n";
		}
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar Packet;

		if (tPacketStar::Find(Data, Packet))
		{
			std::cout << "tPacketStar::Find() OK\n";
		}
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
			0x2A, 0x09, 0x00 };

		tPacketStar Packet;

		if (tPacketStar::Find(Data, Packet))
		{
			std::cout << "tPacketStar::Find() OK\n";
		}
	}
}

	}
}