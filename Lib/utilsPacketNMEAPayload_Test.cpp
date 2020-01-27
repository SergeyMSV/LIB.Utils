#include "utilsPacketNMEA.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

void UnitTest_PacketNMEAPayloadGGA();
void UnitTest_PacketNMEAPayloadGSV();
void UnitTest_PacketNMEAPayloadRMC();

void UnitTest_PacketNMEAPayload()
{
	std::cout << "\n""utils::packet_NMEA::Payload\n";

	using namespace utils::packet_NMEA;

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	{
		tPacketNMEA Packet;

		tVectorUInt8 RawPacket = Packet.ToVector();

		std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
	}

	UnitTest_PacketNMEAPayloadGGA();
	UnitTest_PacketNMEAPayloadGSV();
	UnitTest_PacketNMEAPayloadRMC();


	std::cout << std::endl;
}

}
