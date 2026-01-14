#include "utilsPacket.h"
#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAPayload.h"
#include "utilsPacketNMEAPayloadPMTK.h"
#include "utilsPacketNMEAPayloadPTWS.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

#define SHOW_RESULTS

namespace utils
{

template<class TPayload, class TArg>
void UnitTest_PacketNMEAPayload(const std::string& name, const TArg& msg)
{
	typedef utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon> tPacketNMEA;

	std::vector<std::uint8_t> DataVector(msg.cbegin(), msg.cend());
	const std::string PacketNew(DataVector.cbegin(), DataVector.cend());

	tPacketNMEA PacketFound;
	tPacketNMEA::Find(DataVector, PacketFound);
	const std::vector<std::uint8_t> PacketFoundVec = PacketFound.ToVector();
	const std::string PacketFoundStr(PacketFoundVec.cbegin(), PacketFoundVec.cend());

	utils::packet::nmea::tPayloadCommon::value_type PacketFoundData = PacketFound.GetPayloadValue();
	TPayload Val(PacketFoundData);

	//if (Val.GNSS.Value == TPayload::gnss_type::tGNSS_State::UNKNOWN) // Parsed!!
	//	return;

	const utils::packet::nmea::tPayloadCommon::value_type PacketData1 = Val.GetPayload();

	tPacketNMEA Packet2(PacketData1);

	std::vector<std::uint8_t> PacketRaw = Packet2.ToVector();
	std::string PacketRes(PacketRaw.cbegin(), PacketRaw.cend());

#ifdef SHOW_RESULTS
	std::cout << "Pack         " << PacketNew;
	std::cout << "Pack found:  " << PacketFoundStr;
	std::cout << "Pack result: " << PacketRes;
#endif // SHOW_RESULTS
	utils::test::RESULT(name, PacketNew == PacketRes);
}

template<class TPayload>
void UnitTest_PacketNMEAPayload(const TPayload& val)
{
	typedef utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon> tPacketNMEA;

	utils::packet::nmea::tPayloadCommon::value_type PacketData = val.GetPayload();

	tPacketNMEA Packet(PacketData);

	std::vector<std::uint8_t> RawPacket = Packet.ToVector();

	UnitTest_PacketNMEAPayload<TPayload>(RawPacket);
}

void UnitTest_PacketNMEAPayload()
{
	std::cout << "\n""utils::packet::nmea::Payload\n";

	using namespace std::string_literals;

	using namespace utils::packet::nmea;

	//utils::packet::nmea::Type::

	typedef utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon> tPacketNMEA;

	{
		tPacketNMEA Packet;
		std::vector<std::uint8_t> RawPacket = Packet.ToVector();
		std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';
	}
	/*
	UnitTest_PacketNMEAPayload<mtk_axn_1_30_eb500::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 1",	"$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_axn_1_30_eb500::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 2",	"$GNGGA,172905.087,,,,,0,10,,,M,,M,,*60\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_axn_1_30_eb500::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 3",	"$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,,280.7,M,14.5,M,,*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_axn_1_30_eb500::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,0.00,280.7,M,14.5,M,,*5C\xd\xa"s);

	UnitTest_PacketNMEAPayload<mtk_axn_3_8_eb800a::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 1",	"$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_axn_3_8_eb800a::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 2",	"$GNGGA,172905.087,,,,,0,10,,,M,,M,,*60\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_axn_3_8_eb800a::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 3",	"$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,,280.701,M,14.501,M,,*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_axn_3_8_eb800a::tPayloadGGA>("mtk_axn_3_8_eb800a GGA 3", "$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,0.00,280.701,M,14.501,M,,*5C\xd\xa"s);
	*/
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 1", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*72\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,0014.5,M,,*6C\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 3", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,0014.5,M,000.0,0000*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,-0014.5,M,000.0,0000*6F\xd\xa"s);
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s); // C++14
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa"s);

	/*
		// [TBD] what is EMPTY !!!!
	// [TBD] what is EMPTY !!!!
	// $GNGGA,172904.087,,,,,0,0,,,M,,M,,*50
	// $GPGGA,000124.168,3600.0000,N,13600.0000,E,0,00,99.9,00000.0,M,0000.0,M,000.0,0000*43
	// 
	// ",M" vs. "0000.0,M"
	//static std::string ToStringEmpty() { return ","; }
	*/


	//UnitTest_PacketNMEAPayload<base::tPayloadGGA<15, 3, 4, 4, 5, 2, 3, 2>>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s);//C++14
	//UnitTest_PacketNMEAPayload<base::tPayloadGGA<15, 0, 4, 4, 5, 2, 3, 2>>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa"s);

	/*UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa"s);

	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,4,13,08,01,029,*49\xd\xa"s);

	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,3,09,86,03,164,*52\xd\xa"s);
	
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GNRMC,221325.000,V,,,,,0.00,0.00,241016,,,N*56\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 6, 6>>("$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa"s);

	UnitTest_PacketNMEAPayload<tPayloadPMTK001<604>>("$PMTK001,604,3*32\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadPMTK001<355>>("$PMTK001,355,3,1,1,0,0*33\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadPMTK010>("$PMTK010,001*2E\xd\xa"s);
	UnitTest_PacketNMEAPayload<tPayloadPMTK011>("$PMTK011,MTKGPS*08\xd\xa"s);

	UnitTest_PacketNMEAPayload(tPayloadPMTK101ResetHot());
	UnitTest_PacketNMEAPayload(tPayloadPMTK102ResetWarm());
	UnitTest_PacketNMEAPayload(tPayloadPMTK103ResetCold());
	UnitTest_PacketNMEAPayload(tPayloadPMTK104ResetFull());
	
	UnitTest_PacketNMEAPayload(tPayloadPMTK314R2());
	UnitTest_PacketNMEAPayload(tPayloadPMTK314R2(1, 2, 3, 4, 5, 6, 18));

	UnitTest_PacketNMEAPayload(tPayloadPMTK314R3());
	UnitTest_PacketNMEAPayload(tPayloadPMTK314R3(1, 2, 3, 4, 5, 6, 18));
	UnitTest_PacketNMEAPayload(tPayloadPMTK314R3(1, 2, 3, 4, 5, 6, 7, 8, 18, 19, 20, 21));

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
	UnitTest_PacketNMEAPayload<tPayloadPTWS_JAM_SIGNAL_VAL>("$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa"s);
	*/

	std::cout << std::endl;
}

}
