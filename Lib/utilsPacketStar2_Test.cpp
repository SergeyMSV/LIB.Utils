#include "utilsPacketStar2.h"
//#include "utilsTest.h"
#include <iostream>//Replace with utilsTest.h

namespace utils
{
	namespace unit_test
	{

void UnitTest_PacketStar2()
{
	std::cout << "\n""utils::packet::tPacketStar2\n";

	typedef utils::packet::tPacket<utils::packet_Star::tFormatStar2, utils::packet::tPayloadCommon> tPacketStar2;

	{
		tPacketStar2 Packet;

		//Packet.

		tVectorUInt8 PacketVector = Packet.ToVector();
	}

	{
		tVectorUInt8 Data{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar2 Packet;

		if (tPacketStar2::TryParse(Data, Packet))
		{
			std::cout << "tPacketStar2::TryParse() OK\n";
		}
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D };

		tPacketStar2 Packet;

		if (tPacketStar2::Find(Data, Packet))
		{
			std::cout << "tPacketStar2::Find() OK\n";
		}
	}

	{
		tVectorUInt8 Data{
			0x2A, 0x09, 0x00,
			0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
			0x2A, 0x09, 0x00 };

		tPacketStar2 Packet;

		if (tPacketStar2::Find(Data, Packet))
		{
			std::cout << "tPacketStar2::Find() OK\n";
		}
	}
}

	}
}