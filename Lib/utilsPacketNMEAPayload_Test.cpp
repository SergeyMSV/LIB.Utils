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

	UnitTest_PacketNMEAPayload<tPayloadGGA<15, 3, 4, 4, 5, 2, 3, 2>>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadGGA<15, 0, 4, 4, 5, 2, 3, 2>>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,3,09,86,03,164,*52\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GNRMC,221325.000,V,,,,,0.00,0.00,241016,,,N*56\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67\xd\xa");

	UnitTest_PacketNMEAPayload<tPayloadPMTK001, std::string>("$PMTK001,604,3*32\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadPMTK010, std::string>("$PMTK010,001*2E\xd\xa");
	UnitTest_PacketNMEAPayload<tPayloadPMTK011, std::string>("$PMTK011,MTKGPS*08\xd\xa");

	UnitTest_PacketNMEAPayload(tPayloadPMTK101ResetHot());
	UnitTest_PacketNMEAPayload(tPayloadPMTK102ResetWarm());
	UnitTest_PacketNMEAPayload(tPayloadPMTK103ResetCold());
	UnitTest_PacketNMEAPayload(tPayloadPMTK104ResetFull());
	
	UnitTest_PacketNMEAPayload(tPayloadPMTK314());
	UnitTest_PacketNMEAPayload(tPayloadPMTK314(1, 2, 3, 4, 5, 6, 18));

	UnitTest_PacketNMEAPayload(tPayloadPMTK353());
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(false, false));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(false, true));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(true, false));
	UnitTest_PacketNMEAPayload(tPayloadPMTK353(true, true));
	if (tPayloadPMTK353(true, true).GetState() == Type::tGNSS_State::GPS_GLONASS)
		std::cout << "OK\n";

	UnitTest_PacketNMEAPayload(tPayloadPMTK705("AXN_3.84_3333_12345600", "0000","","1.0"));
	UnitTest_PacketNMEAPayload(tPayloadPMTK705("AXN_3.84_3333_12345600", "0000", "la-la", "1.0"));

	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_GET());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL("M33-asdfasdf.fff.df"));

	UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL());
	UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL(12, 1575.000002));
	UnitTest_PacketNMEAPayload<tPayloadPTWS_JAM_SIGNAL_VAL, std::string>("$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa");

	std::cout << std::endl;
}

}
