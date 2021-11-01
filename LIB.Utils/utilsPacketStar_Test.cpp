#include "utilsPacket_Test.h"
#include "utilsPacketStar.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketStar()
{
	std::cout << "\n""utils::packet::tPacketStar\n";

	using tPacketStar = packet::tPacket<packet_Star::tFormatStar, tVectorUInt8>;

	UnitTest_Packet_Find<tPacketStar>("tVectorUInt8 Test 1",
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketStar>("tVectorUInt8 Test 2",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });


	UnitTest_Packet_Find<tPacketStar>("tVectorUInt8 Test 3",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketStar>("tVectorUInt8 Test 4",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{});

	//UnitTest_Packet_Parse<tPacketStar>("tVectorUInt8 Parse 1",
	//	tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
	//	tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	using tPacketStar2 = packet::tPacket<packet_Star::tFormatStar, packet_Star::tPayloadCommon>;

	UnitTest_Packet_Find<tPacketStar2>("tPayloadCommon Find 1",
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketStar2>("tPayloadCommon Find 2",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });


	UnitTest_Packet_Find<tPacketStar2>("tPayloadCommon Find 3",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketStar2>("tPayloadCommon Find 4",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{});

	UnitTest_Packet_Parse<tPacketStar2>("tPayloadCommon Parse 1",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
					  0x2A, 0x09, 0x00 },
		tPacketStar2::payload_value_type(0x01,
			tVectorUInt8{ 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }));

	UnitTest_Packet_Make<tPacketStar2>("tPayloadCommon Make 1",
		tPacketStar2::payload_value_type(0x01,
			tVectorUInt8{ 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }));

	UnitTest_Packet_Make<tPacketStar2>("tPayloadCommon Make 2",
		tPacketStar2::payload_value_type(0x01,
			tVectorUInt8{}));

	std::cout << std::endl;
}

}