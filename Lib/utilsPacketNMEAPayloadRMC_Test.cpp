#include "utilsPacketNMEAPayloadRMC.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

void UnitTest_PacketNMEAPayloadRMC()
{
	std::cout << "\n""utils::packet_NMEA::PayloadRMC\n";

	using namespace utils::packet_NMEA;

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	{
		std::string Raw("$GNRMC,221325.000,V,,,,,0.00,0.00,241016,,,N*56\xd\xa");
		tVectorUInt8 DataVector(Raw.begin(), Raw.end());

		tPacketNMEA Packet;

		bool Result = tPacketNMEA::Find(DataVector, Packet);

		utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

		typedef tPayloadRMC<13, 10, 9, 10> tMsgRMC;

		tMsgRMC Val(PacketData);

		if (Val.GNSS.Value != tMsgRMC::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
		{
			utils::packet_NMEA::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

			tPacketNMEA Packet2(PacketData1);

			tVectorUInt8 RawPacket = Packet2.ToVector();

			std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
		}
	}

	{
		std::string Raw("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67\xd\xa");
		tVectorUInt8 DataVector(Raw.begin(), Raw.end());

		tPacketNMEA Packet;

		bool Result = tPacketNMEA::Find(DataVector, Packet);

		utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

		typedef tPayloadRMC<13, 10, 9, 10> tMsgRMC;

		tMsgRMC Val(PacketData);

		if (Val.GNSS.Value != tMsgRMC::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
		{
			utils::packet_NMEA::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

			tPacketNMEA Packet2(PacketData1);

			tVectorUInt8 RawPacket = Packet2.ToVector();

			std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
		}
	}

	{
		std::string Raw("$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa");
		tVectorUInt8 DataVector(Raw.begin(), Raw.end());

		tPacketNMEA Packet;

		bool Result = tPacketNMEA::Find(DataVector, Packet);

		utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

		typedef tPayloadRMC<13, 10, 11, 12> tMsgRMC;

		tMsgRMC Val(PacketData);

		if (Val.GNSS.Value != tMsgRMC::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
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
