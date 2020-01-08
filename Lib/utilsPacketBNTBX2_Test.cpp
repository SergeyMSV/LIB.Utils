#include "utilsPacketBNTBX2.h"
//#include "utilsTest.h"
#include <iostream>//Replace with utilsTest.h

namespace utils
{
	namespace unit_test
	{

void UnitTest_PacketBNTBX2()
{
	std::cout << "\n""utils::packet::tPacketBNTBX2\n";

	typedef utils::packet::tPacket<utils::packet_BNTBX::tFormatBNTBX2, utils::packet::tPayloadCommon> tPacketBNTBX2;

	{
		tVectorUInt8 Data{ 
			0x00, 0x08, 0x5F, 0x5F, //Rubbish
			0x5F, 0x66, 0x00, 0x00, 0x00, 0x08, 0x02, 0x01, 0x00, 0x00, 0xAA, 0x62 };

		tPacketBNTBX2 Packet;

		if (tPacketBNTBX2::Find(Data, Packet))
		{
			std::cout << "tPacketBNTBX2::Find() OK\n";
		}
	}

	{
		tVectorUInt8 Data{
			0x00, 0x08, 0x5F, 0x5F, //Rubbish
			0x5F, 0x66, 0x00, 0x00, 0x00, 0x08, 0x02, 0x01, 0x00, 0x00, 0xAA, 0x62,
			0x00, 0x08, 0x5F, 0x5F };

		tPacketBNTBX2 Packet;

		if (tPacketBNTBX2::Find(Data, Packet))
		{
			std::cout << "tPacketBNTBX2::Find() OK\n";
		}
	}
}

	}
}