#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAPayload.h"
#include "utilsPacketNMEAPayloadPMTK.h"
#include "utilsPacketNMEAPayloadPTWS.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

template<class TPayload, class TArg>
void UnitTest_PacketNMEAPayload(const TArg& msg)
{
	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	tVectorUInt8 DataVector(msg.cbegin(), msg.cend());//C++14

	tPacketNMEA Packet;

	bool Result = tPacketNMEA::Find(DataVector, Packet);

	utils::packet_NMEA::tPayloadCommon::value_type PacketData = Packet.GetPayload();

	TPayload Val(PacketData);

	//if (Val.GNSS.Value != TPayload::gnss_type::tGNSS_State::UNKNOWN)//Parsed!!
	{
		utils::packet_NMEA::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

		tPacketNMEA Packet2(PacketData1);

		tVectorUInt8 RawPacket = Packet2.ToVector();

		std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';//C++14
	}
}

template<class TPayload>
void UnitTest_PacketNMEAPayload(const TPayload& val)
{
	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	utils::packet_NMEA::tPayloadCommon::value_type PacketData = val.GetPayload();

	tPacketNMEA Packet(PacketData);

	tVectorUInt8 RawPacket = Packet.ToVector();

	UnitTest_PacketNMEAPayload<TPayload>(RawPacket);
}

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

	UnitTest_PacketNMEAPayload<tPayloadGGA<15, 3, 4, 4, 5, 2, 3, 2>, std::string>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGGA<15, 0, 4, 4, 5, 2, 3, 2>, std::string>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GPGSV,4,4,13,08,01,029,*49\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGSV, std::string>("$GLGSV,3,3,09,86,03,164,*52\xd\xa");
	
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>, std::string>("$GNRMC,221325.000,V,,,,,0.00,0.00,241016,,,N*56\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>, std::string>("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 6, 6>, std::string>("$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadPMTK001, std::string>("$PMTK001,604,3*32\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadPMTK010, std::string>("$PMTK010,001*2E\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadPMTK011, std::string>("$PMTK011,MTKGPS*08\xd\xa");

	UnitTest_PacketNMEAPayload(tPayloadPMTK314());
	UnitTest_PacketNMEAPayload(tPayloadPMTK314(1, 2, 3, 4, 5, 6, 18));

	UnitTest_PacketNMEAPayload(tPayloadPMTK353());
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(false, false));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(false, true));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(true, false));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(true, true));
	if (tPayloadPMTK353(true, true).GetState() == Type::tGNSS_State::GPS_GLONASS)
		std::cout << "OK\n";

	UnitTest_PacketNMEAPayload(tPayloadPMTK705("AXN_3.84_3333_15071800", "0000","","1.0"));
	UnitTest_PacketNMEAPayload(tPayloadPMTK705("AXN_3.84_3333_15071800", "0000", "la-la", "1.0"));

	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_GET());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL("M33-asdfasdf.fff.df"));

	UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL(12, 1575.000002));
	UnitTest_PacketNMEAPayload<tPayloadPTWS_JAM_SIGNAL_VAL, std::string>("$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa");


	std::cout << std::endl;
}

}
