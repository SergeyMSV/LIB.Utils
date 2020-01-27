#include "utilsPacketNMEAPayloadGSV.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

void UnitTest_PacketNMEAPayloadGSV()
{
	std::cout << "\n""utils::packet_NMEA::PayloadGSV\n";

	using namespace utils::packet_NMEA;

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	std::cout << std::endl;
}

}
