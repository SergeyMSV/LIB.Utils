#include "utilsPacket.h"
#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAPayload.h"
#include "utilsPacketNMEAPayloadPMTK.h"
#include "utilsPacketNMEAPayloadPTWS.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

//#define SHOW_RESULTS

namespace utils
{

template<class TPayload, class TArg>
void UnitTest_PacketNMEAPayload(const std::string& name, const TArg& msg)
{
	using tPacketNMEA = utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon>;

	std::vector<std::uint8_t> DataVector(msg.cbegin(), msg.cend());
	const std::string PacketNew(DataVector.cbegin(), DataVector.cend());

	tPacketNMEA PacketFound;
	tPacketNMEA::Find(DataVector, PacketFound);
	const std::vector<std::uint8_t> PacketFoundVec = PacketFound.ToVector();
	const std::string PacketFoundStr(PacketFoundVec.cbegin(), PacketFoundVec.cend());

	utils::packet::nmea::tPayloadCommon::value_type PacketFoundData = PacketFound.GetPayloadValue();
	TPayload PayloadParsed(PacketFoundData);

	//if (PayloadParsed.GNSS.Value == TPayload::gnss_type::tGNSS_State::UNKNOWN) // Parsed!!
	//	return;

	const utils::packet::nmea::tPayloadCommon::value_type PacketData1 = PayloadParsed.GetPayload();

	tPacketNMEA Packet2(PacketData1);

	std::vector<std::uint8_t> PacketRaw = Packet2.ToVector();
	std::string PacketRes(PacketRaw.cbegin(), PacketRaw.cend());

#ifdef SHOW_RESULTS
	std::cout << "Pack         " << PacketNew;
	std::cout << "Pack found:  " << PacketFoundStr;
	std::cout << "Pack result: " << PacketRes;
#endif // SHOW_RESULTS
	utils::test::RESULT(name, PayloadParsed.IsVerified() && PacketNew == PacketRes);
}

template<class TPayload, class TArg>
void UnitTest_PacketNMEAPayloadERR(const std::string& name, const TArg& msg)
{
	using tPacketNMEA = utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon>;

	std::vector<std::uint8_t> DataVector(msg.cbegin(), msg.cend());
	const std::string PacketNew(DataVector.cbegin(), DataVector.cend());

	tPacketNMEA PacketFound;
	tPacketNMEA::Find(DataVector, PacketFound);
	const std::vector<std::uint8_t> PacketFoundVec = PacketFound.ToVector();
	const std::string PacketFoundStr(PacketFoundVec.cbegin(), PacketFoundVec.cend());

	utils::packet::nmea::tPayloadCommon::value_type PacketFoundData = PacketFound.GetPayloadValue();
	TPayload PayloadParsed(PacketFoundData);

#ifdef SHOW_RESULTS
	std::cout << "Pack         " << PacketNew;
	std::cout << "Pack found:  " << PacketFoundStr;
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + name, !PayloadParsed.IsVerified());
}

void UnitTest_PacketNMEAPayload()
{
	std::cout << "\n""utils::packet::nmea::Payload\n";

	using namespace std::string_literals;

	using namespace utils::packet::nmea;

	//utils::packet::nmea::

	typedef utils::packet::tPacket<utils::packet::nmea::tFormatNMEA, utils::packet::nmea::tPayloadCommon> tPacketNMEA;

	{
		tPacketNMEA Packet;
		std::vector<std::uint8_t> RawPacket = Packet.ToVector();
		std::cout << std::string(RawPacket.cbegin(), RawPacket.cend()) << '\n';
	}
	
	std::cout << '\n';

	UnitTest_PacketNMEAPayload<generic::tPayloadGSV>("generic GSV 1", "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);

	UnitTest_PacketNMEAPayloadERR<generic::tPayloadGSV>("generic GSV wrong 1", "$GPGSQ,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);

	std::cout << '\n';

	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGGA>("mtk_eb500 GGA 1",	"$GNGGA,172905.087,0000.0000,N,00000.0000,E,0,0,,,M,,M,,*6A\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGGA>("mtk_eb500 GGA 2",	"$GNGGA,172905.087,0000.0000,N,00000.0000,E,0,10,,,M,,M,,*5B\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGGA>("mtk_eb500 GGA 3",	"$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,,280.7,M,14.5,M,,*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGGA>("mtk_eb500 GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,0.00,280.7,M,14.5,M,,*5C\xd\xa"s);

	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 1", "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 2", "$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 3", "$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 4", "$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 5", "$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 6", "$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 7", "$GPGSV,4,4,13,08,01,029,*49\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 8", "$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 9", "$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 10", "$GLGSV,3,3,09,86,03,164,*52\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadGSV>("mtk_eb500 GSV 20", "$GPGSV,1,1,00*79\xd\xa"s);

	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadRMC>("mtk_eb500 RMC 1", "$GPRMC,032306.037,V,8960.0000,N,00000.0000,E,0.00,0.00,060180,,,N*7E\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb500::tPayloadRMC>("mtk_eb500 RMC 2", "$GPRMC,191709.000,A,1122.3344,N,01122.3344,E,0.01,53.59,080417,,,D*5D\xd\xa"s);

	std::cout << '\n';

	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadGGA>("mtk_eb800a GGA 1",	"$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadGGA>("mtk_eb800a GGA 2",	"$GNGGA,172905.087,,,,,0,10,,,M,,M,,*60\xd\xa"s); // C++14
	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadGGA>("mtk_eb800a GGA 3",	"$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,,280.701,M,14.501,M,,*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadGGA>("mtk_eb800a GGA 3", "$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,0.00,280.701,M,14.501,M,,*5C\xd\xa"s);

	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadGLL>("mtk_eb800a GLL 1", "$GNGLL,1234.556677,N,12233.445566,E,123456.000,A,A*45\xd\xa"s);

	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadVTG>("mtk_eb800a VTG 1", "$GNVTG,62.90,T,,M,0.00,N,0.00,K,N*11\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk_eb800a::tPayloadVTG>("mtk_eb800a VTG 2", "$GPVTG,62.90,T,,M,0.07,N,0.12,K,A*04\xd\xa"s);

	std::cout << '\n';

	// The parsers shall not cause an error.
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 1", "$GP,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*33\xd\xa"s);
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 1", "$GPG,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*74\xd\xa"s);
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 1", "$GPGG,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*33\xd\xa"s);

	
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,0014.5,M,000.0,0000*42\xd\xa"s);
	
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,-0014.5,M,000.0,0000*6F\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 3", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,0014.5,M,000.0,0000*42\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,-0014.5,M,000.0,0000*6F\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 10", "$GPGGA,000124.168,3600.0000,N,13600.0000,E,0,00,99.9,00000.0,M,0000.0,M,000.0,0000*43\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 11", "$GPGGA,132652.908,1122.3344,N,12233.4455,E,0,00,99.9,00176.1,M,0014.4,M,000.0,0000*4B\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 12", "$GPGGA,112531.000,1122.3344,N,12233.4455,E,1,12,00.7,00639.3,M,0014.3,M,000.0,0000*4B\xd\xa"s);
	
	// ERRs
	UnitTest_PacketNMEAPayloadERR<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,,00.0,00280.7,M,0014.5,M,000.0,0000*40\xd\xa"s); // ERR
	UnitTest_PacketNMEAPayloadERR<sirf_gsu_7x::tPayloadGGA>("sirf_gsu_7x GGA 1", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,000.0,0000*5C\xd\xa"s); // ERR
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"s);
	//UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGGA>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGLL>("sirf_gsu_7x GLL 1", "$GPGLL,3600.0000,N,13600.0000,E,000124.168,V*2A\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGLL>("sirf_gsu_7x GLL 2", "$GPGLL,1122.3344,N,12233.4455,E,132652.908,V*22\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGLL>("sirf_gsu_7x GLL 3", "$GPGLL,1122.3344,N,12233.4455,E,112531.000,A*30\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSA>("sirf_gsu_7x GSA 1", "$GPGSA,A,1,,,,,,,,,,,,,99.9,99.9,99.9*09\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSA>("sirf_gsu_7x GSA 2", "$GPGSA,A,3,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*06\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSA>("sirf_gsu_7x GSA 3", "$GPGSA,A,3,23,29,07,08,09,18,26,28,,,,,12.3,45.6,78.9*08\xd\xa"s);
	UnitTest_PacketNMEAPayloadERR<sirf_gsu_7x::tPayloadGSA>("sirf_gsu_7x GSA 10", "$GPGSA,A,8,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*0D\xd\xa"s);
	UnitTest_PacketNMEAPayloadERR<sirf_gsu_7x::tPayloadGSA>("sirf_gsu_7x GSA 11", "$GPGSA,R,3,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*15\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 1", "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 2", "$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 3", "$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 4", "$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 5", "$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 6", "$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 7", "$GPGSV,4,4,13,08,01,029,*49\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 8", "$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 9", "$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadGSV>("sirf_gsu_7x GSV 10", "$GLGSV,3,3,09,86,03,164,*52\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadRMC>("sirf_gsu_7x RMC 1", "$GPRMC,000400.148,V,3600.0000,N,13600.0000,E,9999.99,999.99,180109,,*24\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadRMC>("sirf_gsu_7x RMC 2", "$GPRMC,123456.789,V,1122.3344,N,01122.3344,E,9999.99,999.99,180109,,*2D\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadRMC>("sirf_gsu_7x RMC 3", "$GPRMC,123456.789,V,1122.3344,N,12233.4455,E,9999.99,999.99,180109,,*2C\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadRMC>("sirf_gsu_7x RMC 4", "$GPRMC,123456.789,A,1122.3344,N,01122.3344,E,0052.58,132.68,180109,,*37\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadRMC>("sirf_gsu_7x RMC 5", "$GPRMC,123456.789,A,1122.3344,N,12233.4455,E,0052.58,132.68,180109,,*36\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadVTG>("sirf_gsu_7x VTG 1", "$GPVTG,999.99,T,,M,9999.99,N,9999.99,K*59\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadVTG>("sirf_gsu_7x VTG 2", "$GPVTG,132.68,T,,M,0052.58,N,0097.38,K*51\xd\xa"s);

	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadZDA>("sirf_gsu_7x ZDA 1", "$GPZDA,000125.168,25,01,2009,,*52\xd\xa"s);
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadZDA>("sirf_gsu_7x ZDA 2", "$GPZDA,000125.168,25,01,2009,00,00*52\xd\xa"s); // last two fields are not supported
	UnitTest_PacketNMEAPayload<sirf_gsu_7x::tPayloadZDA>("sirf_gsu_7x ZDA 3", "$GPZDA,000125.168,25,01,2009,-13,59*71\xd\xa"s); // last two fields are not supported

	

	std::cout << '\n';

	UnitTest_PacketNMEAPayload<mtk::tPayloadTEST>("mtk TEST 1", "$PMTK000*32\xd\xa"s);
	UnitTest_PacketNMEAPayload<mtk::tPayloadACK>("mtk ACK  1", "$PMTK001,604,3*32\xd\xa"s);

	UnitTest_PacketNMEAPayload<telit::tPayloadJAM_SIGNAL_VAL>("telit JAM 1", "$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa"s);
	UnitTest_PacketNMEAPayload<telit::tPayloadJAM_SIGNAL_VAL>("telit JAM 2", "$PTWS,JAM,SIGNAL,VAL,INDEX,18,FREQ,1574.990234*54\xd\xa"s);
	UnitTest_PacketNMEAPayload<telit::tPayloadVERSION_VAL>("telit VERSION 1", "$PTWS,VERSION,VAL,v13-2.2.0-STD-3.8.13-N96-B2*3F\xd\xa"s);

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

	/*
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa"s);

	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GPGSV,4,4,13,08,01,029,*49\xd\xa"s);

	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadGSV>("$GLGSV,3,3,09,86,03,164,*52\xd\xa"s);
	
	//UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GNRMC,221325.000,V,,,,,0.00,0.00,241016,,,N*56\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 4, 4>>("$GPRMC,082653.100,A,2446.4768,N,12100.0344,E,0.00,128.42,270705,,,A*67\xd\xa"s);
	//UnitTest_PacketNMEAPayload<tPayloadRMC<13, 3, 6, 6>>("$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa"s);

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

	//UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_GET());
	//UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL());
	//UnitTest_PacketNMEAPayload(tPayloadPTWS_VERSION_VAL("M33-asdfasdf.fff.df"));

	//UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL());
	//UnitTest_PacketNMEAPayload(tPayloadPTWS_JAM_SIGNAL_VAL(12, 1575.000002));
	//UnitTest_PacketNMEAPayload<tPayloadPTWS_JAM_SIGNAL_VAL>("$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa"s);
	*/

	std::cout << std::endl;
}

}
