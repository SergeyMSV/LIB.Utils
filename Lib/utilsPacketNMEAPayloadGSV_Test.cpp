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

	//char* Data = "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa";
	//char* Data = "$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa";
	//char* Data = "$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa";

	//char* Data = "$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa";
	//char* Data = "$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa";
	//char* Data = "$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa";
	//char* Data = "$GPGSV,4,4,13,08,01,029,*49\xd\xa";

	//char* Data = "$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa";
	//char* Data = "$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa";
	//char* Data = "$GLGSV,3,3,09,86,03,164,*52\xd\xa";

/*	{
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
		std::string Raw("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67*56\xd\xa");
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
	}*/


	std::cout << std::endl;
}

}
