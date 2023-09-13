#include "utilsPacket_Test.h"
#include "utilsPacketStar.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketStar()
{
	std::cout << "\n""utils::packet::tPacketStar\n";

	using tPacketSimple = packet_Star::example::tPacketSimple;

	UnitTest_Packet_Find<tPacketSimple>("tVectorUInt8 Test 1",
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketSimple>("tVectorUInt8 Test 2",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });


	UnitTest_Packet_Find<tPacketSimple>("tVectorUInt8 Test 3",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketSimple>("tVectorUInt8 Test 4",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{});

	//UnitTest_Packet_Parse<tPacketSimple>("tVectorUInt8 Parse 1",
	//	tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
	//	tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	using tPacketMsg = packet_Star::example::tPacketMsg;

	UnitTest_Packet_Find<tPacketMsg>("tPayloadCommon Find 1",
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketMsg>("tPayloadCommon Find 2",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });


	UnitTest_Packet_Find<tPacketMsg>("tPayloadCommon Find 3",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D });

	UnitTest_Packet_Find<tPacketMsg>("tPayloadCommon Find 4",
		tVectorUInt8{ 0x2A, 0x09, 0x00,
					  0x2A, 0x09, 0x00 },
		tVectorUInt8{});

	UnitTest_Packet_Parse<tPacketMsg>("tPacketCmd Parse: MakeSomeMessage_01",
		tVectorUInt8{ 0x2A, 0x09, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xB0, 0x8D },
		tPacketMsg::MakeSomeMessage_01({ 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }));

	UnitTest_Packet_Make("tPayloadCommon Make: MakeSomeMessage_01",
		tPacketMsg::MakeSomeMessage_01({ 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }));

	std::cout << std::endl;
}

}