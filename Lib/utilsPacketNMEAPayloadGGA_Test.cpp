#include "utilsPacketNMEAPayloadGGA.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

void UnitTest_PacketNMEAPayloadGGA()
{
	std::cout << "\n""utils::packet_NMEA::PayloadGGA\n";

	using namespace utils::packet_NMEA;

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	{
		std::string Raw("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa");
		tVectorUInt8 DataVector(Raw.begin(), Raw.end());

		tPacketNMEA Packet;

		bool Result = tPacketNMEA::Find(DataVector, Packet);

		utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

		typedef tPayloadGGA<15, 10, 9, 10> tMsgGGA;

		tMsgGGA Val(PacketData);

		if (Val.GNSS.Value != tMsgGGA::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
		{
			utils::packet_NMEA::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

			tPacketNMEA Packet2(PacketData1);

			tVectorUInt8 RawPacket = Packet2.ToVector();

			std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
		}
	}

	{
		std::string Raw("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa");
		tVectorUInt8 DataVector(Raw.begin(), Raw.end());

		tPacketNMEA Packet;

		bool Result = tPacketNMEA::Find(DataVector, Packet);

		utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

		typedef tPayloadGGA<15, 6, 9, 10> tMsgGGA;

		tMsgGGA Val(PacketData);

		if (Val.GNSS.Value != tMsgGGA::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
		{
			utils::packet_NMEA::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

			tPacketNMEA Packet2(PacketData1);

			tVectorUInt8 RawPacket = Packet2.ToVector();

			std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
		}
	}


	std::cout << std::endl;
}

}
