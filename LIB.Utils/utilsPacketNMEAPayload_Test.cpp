#include "utilsPacket.h"
#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAPayload.h"
#include "utilsPacketNMEAPayloadP.h"
#include "utilsPacketNMEAPayloadPMTK.h"
#include "utilsPacketNMEAPayloadPSRF.h"
#include "utilsPacketNMEAPayloadPTWS.h"
#include "utilsPacketNMEAPayloadPUBX.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

//#define SHOW_RESULTS

namespace utils
{

template<class TPacket, class TContent>
TContent UnitTest_PacketNMEA(const std::string& name, const std::string& msg)
{
	std::vector<std::uint8_t> DataVector(msg.begin(), msg.end());

	std::string PacketFoundStr;
	typename TPacket::payload_value_type PacketFoundData;

	std::optional<TPacket> PacketFoundOpt = TPacket::Find(DataVector);
	if (PacketFoundOpt.has_value())
	{
		const std::vector<std::uint8_t> PacketFoundVec = PacketFoundOpt->ToVector();
		PacketFoundStr = std::string(PacketFoundVec.begin(), PacketFoundVec.end());
		PacketFoundData = PacketFoundOpt->GetPayloadValue();
	}

	TContent PayloadParsed(PacketFoundData);

	//if (PayloadParsed.GNSS.Value == TPayload::gnss_type::tGNSS_State::UNKNOWN) // Parsed!!
	//	return;

	TPacket Packet2(PayloadParsed.GetPayload());

	std::vector<std::uint8_t> PacketRaw = Packet2.ToVector();
	std::string PacketRes(PacketRaw.cbegin(), PacketRaw.cend());

#ifdef SHOW_RESULTS
	std::cout << "Pack         " << msg;
	std::cout << "Pack found:  " << PacketFoundStr;
	std::cout << "Pack result: " << PacketRes;
	if (PacketRes.empty())
		std::cout << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(name, PayloadParsed.IsVerified() && msg == PacketRes);

	return PayloadParsed;
}

template<class TContent>
TContent UnitTest_PacketNMEA_CRC(const std::string& name, const std::string& msg)
{
	return UnitTest_PacketNMEA<packet::nmea::tPacketNMEA_Common_CRC, TContent>(name, msg);
}

template<class TContent>
void UnitTest_PacketNMEA_NoCRC(const std::string& name, const std::string& msg)
{
	UnitTest_PacketNMEA<packet::nmea::tPacketNMEA_Common_NoCRC, TContent>(name, msg);
}

template<class TPacket, class TContent>
void UnitTest_PacketNMEA_ERR(const std::string& name, const std::string& msg)
{
	std::vector<std::uint8_t> DataVector(msg.begin(), msg.end());

	std::string PacketFoundStr;
	std::vector<std::string> PacketFoundData;

	std::optional<TPacket> PacketFoundOpt = TPacket::Find(DataVector);
	if (PacketFoundOpt.has_value())
	{
		std::vector<std::uint8_t> PacketFoundVec = PacketFoundOpt->ToVector();
		PacketFoundStr = std::string(PacketFoundVec.begin(), PacketFoundVec.end());
		PacketFoundData = PacketFoundOpt->GetPayloadValue();
	}

	TContent PayloadParsed(PacketFoundData);

#ifdef SHOW_RESULTS
	std::cout << "Pack         " << msg;
	std::cout << "Pack found:  " << PacketFoundStr;
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + name, !PayloadParsed.IsVerified());
}

template<class TContent>
void UnitTest_PacketNMEA_CRC_ERR(const std::string& name, const std::string& msg)
{
	UnitTest_PacketNMEA_ERR<packet::nmea::tPacketNMEA_Common_CRC, TContent>(name, msg);
}

template<class TContent>
void UnitTest_PacketNMEA_CRC(const std::string& name, TContent content, const std::string& msg)
{
	auto Packet = packet::nmea::tPacketNMEA_Common_CRC(content.GetPayload());
	std::vector<std::uint8_t> PacketVect = Packet.ToVector();
	utils::test::RESULT(name, PacketVect == utils::ToVector<std::string>(msg));
}

void UnitTest_PacketNMEAPayload()
{
	std::cout << "\n""utils::packet::nmea::Payload\n";

	using namespace std::string_literals;

	using namespace utils::packet::nmea;

	std::cout << '\n';

	// Standard NMEA 0183
	UnitTest_PacketNMEA_CRC<base::tContentTXT>("base TXT 3", "$GPTXT,01,01,25,DR MODE - ANTENNA FAULT^21*38\xd\xa");

	std::cout << '\n';

	// Proprietary
	UnitTest_PacketNMEA_CRC<base::tContentP<1, 3, 3>>("tContentP 1,3,3: P MTK 000", "$PMTK000*32\xd\xa");
	UnitTest_PacketNMEA_CRC<base::tContentP<1, 3, 2, 4>>("tContentP 1,3,2,4: P MTK 00 0", "$PMTK000*32\xd\xa");
	//UnitTest_PacketNMEA_CRC<base::tContentP<1, 3>>("tContentP 3", "$PMTK000*32\xd\xa");
	UnitTest_PacketNMEA_CRC<base::tContentP<4, 3>>("base Proprietary 1", "$PMTK000*32\xd\xa");
	UnitTest_PacketNMEA_CRC<base::tContentP<4, 3>>("base Proprietary 2", "$PMTK011,MTKGPS*08\xd\xa");
	UnitTest_PacketNMEA_CRC<base::tContentP<4, 3>>("base Proprietary 3", "$PMTK001,355,3,1,1,0,0*33\xd\xa");
	UnitTest_PacketNMEA_NoCRC<base::tContentP<4, 3>>("base Proprietary 10", "$PSRFTXT,CHNL: 12\xd\xa");

	std::cout << '\n';

	// MTK
	UnitTest_PacketNMEA_CRC<mtk::tContentPMTK_TEST>("mtk TEST 1", "$PMTK000*32\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk::tContentACK>("mtk ACK  1", "$PMTK001,604,3*32\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk::tContentPMTK_SetSerialPort>("mtk Set SerialPort (251)", "$PMTK251,38400*27\xd\xa");
	UnitTest_PacketNMEA_CRC("PMTK TEST 1", mtk::tContentPMTK_TEST(), "$PMTK000*32\xd\xa");
	UnitTest_PacketNMEA_CRC("PMTK Set SerialPort 1", mtk::tContentPMTK_SetSerialPort(38400), "$PMTK251,38400*27\xd\xa");

	std::cout << '\n';

	// SiRF
	UnitTest_PacketNMEA_NoCRC<sirf::tContentPSRF_TXT>("sirf TEXT 1", "$PSRFTXT,NMEA: ID 101 Ack Input\xd\xa");
	UnitTest_PacketNMEA_NoCRC<sirf::tContentPSRF_TXT>("sirf TEXT 2", "$PSRFTXT,GSU-7x : Position Co.,Ltd.2009\xd\xa");
	//UnitTest_PacketNMEA_NoCRC<sirf::tContentPSRF_TXT>("sirf TEXT 2.1 wrong", "$PSRFTXT,GSU-7x : Position Co.,Ltd.2009*34\xd\xa"); // CRC is right. (not real, just for test)
	//UnitTest_PacketNMEA_NoCRC<sirf::tContentPSRF_TXT>("sirf TEXT 2.2 wrong", "$PSRFTXT,GSU-7x : Position Co.,Ltd.2009*21\xd\xa"); // CRC is wrong. (not real, just for test)
	UnitTest_PacketNMEA_NoCRC<sirf::tContentPSRF_TXT>("sirf TEXT 3", "$PSRFTXT,Firmware Checksum: 1fb5\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf::tContentPSRF_SetSerialPort>("sirf Set SerialPort (100)", "$PSRF100,0,9600,8,1,0*0C\xd\xa");
	UnitTest_PacketNMEA_CRC("PSRF Set SerialPort 1", sirf::tContentPSRF_SetSerialPort(38400), "$PSRF100,1,38400,8,1,0*3D\xd\xa");
	//UnitTest_PacketNMEA_CRC("PSRF Set SerialPort 1", sirf::tContentPSRF_SetSerialPort(38500), "$PSRF100,1,38500,8,1,0*3C\xd\xa");

	std::cout << '\n';

	// SiRF legacy
	UnitTest_PacketNMEA_CRC<sirf_legacy::tContentPSRF_TXT>("sirf legacy LEADTEK TEXT 1 Version", "$PSRFTXTVersion 322.000.000-LD00*23\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_legacy::tContentPSRF_TXT>("sirf legacy LEADTEK TEXT 2", "$PSRFTXTCLK:  96250*09\xd\xa");
	UnitTest_PacketNMEA_NoCRC<sirf_legacy::tContentPSRF_TXT>("sirf legacy GSU TEXT 1", "$PSRFTXT,NMEA: ID 101 Ack Input\xd\xa");
	UnitTest_PacketNMEA_NoCRC<sirf_legacy::tContentPSRF_TXT>("sirf legacy GSU TEXT 2", "$PSRFTXT,GSU-7x : Position Co.,Ltd.2009\xd\xa");
	UnitTest_PacketNMEA_NoCRC<sirf_legacy::tContentPSRF_TXT>("sirf legacy GSU TEXT 3", "$PSRFTXT,Firmware Checksum: 1fb5\xd\xa");

	std::cout << '\n';

	// u-blox
	UnitTest_PacketNMEA_CRC<ublox::tContentPUBX_Poll_LatLonPos>("ublox Poll Lat Lon Pos", "$PUBX,00*33\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox::tContentPUBX_Poll_UTMPos>("ublox Poll UTM Pos", "$PUBX,01*32\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox::tContentPUBX_Poll_Sats>("ublox Poll Sats", "$PUBX,03*30\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox::tContentPUBX_Poll_Clock>("ublox Poll Clock", "$PUBX,04*37\xd\xa");
	utils::test::RESULT("ublox Unknown message", ublox::IsUnknownMessage(base::tContentTXT({ "GPTXT","01","01","01","PUBX 41 unknown msg" })));
	utils::test::RESULT("ublox Version HW", ublox::IsVersionHW(base::tContentTXT({ "GPTXT","01","01","02","ANTARIS ATR012x HW 01122334" })));
	utils::test::RESULT("ublox Version Ext", ublox::IsVersionExt(base::tContentTXT({ "GPTXT","01","01","02","EXT CORE       2.00    Sep 03 2026 15:54:32" })));
	utils::test::RESULT("ublox Version Int", ublox::IsVersionInt(base::tContentTXT({ "GPTXT","01","01","02","INT EXT0 (RCV) M3H8.9  Sep 03 2026 18:23:41" })));
	utils::test::RESULT("ublox License", ublox::IsLicense(base::tContentTXT({ "GPTXT","01","01","02","LIC 1234-5678-90AB-CDEF-7564" })));
	utils::test::RESULT("ublox QDS", ublox::IsQDS(base::tContentTXT({ "GPTXT","01","01","02","QDS 12345678912" })));

	std::cout << '\n';

	// MTK EB-500
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGGA>("mtk_eb500 GGA 1", "$GNGGA,172905.087,0000.0000,N,00000.0000,E,0,0,,,M,,M,,*6A\xd\xa"); // C++14
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGGA>("mtk_eb500 GGA 2", "$GNGGA,172905.087,0000.0000,N,00000.0000,E,0,10,,,M,,M,,*5B\xd\xa"); // C++14
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGGA>("mtk_eb500 GGA 3", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,,280.7,M,14.5,M,,*42\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGGA>("mtk_eb500 GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,2,0.00,280.7,M,14.5,M,,*5C\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 1", "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 2", "$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 3", "$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 4", "$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 5", "$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 6", "$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 7", "$GPGSV,4,4,13,08,01,029,*49\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 8", "$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 9", "$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 10", "$GLGSV,3,3,09,86,03,164,*52\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentGSV>("mtk_eb500 GSV 20", "$GPGSV,1,1,00*79\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentRMC>("mtk_eb500 RMC 1", "$GPRMC,032306.028,V,8960.0000,N,00000.0000,E,0.00,0.00,060180,,,N*70\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb500::tContentRMC>("mtk_eb500 RMC 2", "$GPRMC,191709.000,A,1122.3344,N,01122.3344,E,0.01,53.59,080417,,,D*5D\xd\xa");

	std::cout << '\n';

	// MTK EB-800A
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGGA>("mtk_eb800a GGA 1", "$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa"); // C++14
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGGA>("mtk_eb800a GGA 2", "$GNGGA,172905.087,,,,,0,10,,,M,,M,,*60\xd\xa"); // C++14
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGGA>("mtk_eb800a GGA 3", "$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,,280.701,M,14.501,M,,*42\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGGA>("mtk_eb800a GGA 4", "$GPGGA,000124.168,0000.000000,N,00000.000000,E,6,2,0.00,280.701,M,14.501,M,,*5C\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGSA>("mtk_eb800a GSA 1", "$GNGSA,A,3,12,24,02,06,29,14,32,31,03,25,19,,1.10,0.75,0.81*17\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGSA>("mtk_eb800a GSA 2", "$GNGSA,A,3,83,,,,,,,,,,,,1.10,0.75,0.81*1C\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGLL>("mtk_eb800a GLL 1", "$GNGLL,,,,,000106.799,V,N*64\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGLL>("mtk_eb800a GLL 2", "$GNGLL,1234.556677,N,12233.445566,E,123456.000,A,A*45\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentGSV>("mtk_eb800a GSV 1", "$GLGSV,3,3,09,86,03,,*61\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<mtk_eb800a::tContentGSV>("mtk_eb800a GSV 2", "$GPGSV,3,3,10,11,01,025,,193,,,*75\xd\xa"); // 193 - sv, SatID = 193 is an error
	UnitTest_PacketNMEA_CRC_ERR<mtk_eb800a::tContentGSV>("mtk_eb800a GSV 3", "$GPGSV,2,2,08,11,22,219,,13,40,335,,14,25,057,,193,,,*73\xd\xa"); // 193 - sv, SatID = 193 is an error
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentRMC>("mtk_eb800a RMC 1", "$GPRMC,032306.028,V,8960.000000,N,00000.000000,E,0.00,0.00,060180,,,N*70\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentRMC>("mtk_eb800a RMC 2", "$GPRMC,191709.000,A,1122.334455,N,01122.334455,E,0.01,53.59,080417,,,D*5D\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentRMC>("mtk_eb800a RMC 3", "$GPRMC,191709.000,A,1122.000055,N,01122.334455,E,0.01,53.09,080417,,,D*58\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<mtk_eb800a::tContentRMC>("mtk_eb800a RMC 3", "$GNRMC,210805.445,V,,,,,-0.01,260.46,081126,,,N*7E\xd\xa"); // 21:07:26.353 ER: GNRMC 210805.445 V     -0.01 260.46 081126   N (speed over ground is negative) - [such messages are ignored]
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentVTG>("mtk_eb800a VTG 1", "$GNVTG,62.90,T,,M,0.00,N,0.00,K,N*11\xd\xa");
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentVTG>("mtk_eb800a VTG 2", "$GPVTG,62.90,T,,M,0.07,N,0.12,K,A*04\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<mtk_eb800a::tContentVTG>("mtk_eb800a VTG 3", "$GNVTG,167.84,T,,M,-0.01,N,-0.01,K,N*20\xd\xa"); // Speed is negative - that's an error
	UnitTest_PacketNMEA_CRC_ERR<mtk_eb800a::tContentVTG>("mtk_eb800a VTG 4", "$GNVTG,167.84,T,,M,0.00,N,-0.01,K,N*0C\xd\xa"); // Speed is negative - that's an error
	UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentZDA>("mtk_eb800a ZDA 1", "$GNZDA,000106.799,06,01,1980,,*4F\xd\xa");
	{
		auto content = UnitTest_PacketNMEA_CRC<mtk_eb800a::tContentZDA>("mtk_eb800a ZDA 2", "$GNZDA,200832.000,16,03,2026,,*41\xd\xa"); // 2026-03-16 20:08:32.000 = 1773691712
		std::pair<std::time_t, std::uint32_t> time = content.GetDateTime();
		utils::test::RESULT("mtk_eb800a ZDA 2 - time", time.first == 1773691712 && time.second == 0);
	}

	std::cout << '\n';

	// SiRF GSU-7x
	// The parsers shall not cause an error.
	//UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 1", "$GP,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*33\xd\xa");
	//UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 1", "$GPG,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*74\xd\xa");
	//UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 1", "$GPGG,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,,*33\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,0014.5,M,000.0,0000*42\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 3", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,00280.7,M,-014.5,M,000.0,0000*5F\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 4", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,00.0,-0280.7,M,0014.5,M,000.0,0000*5F\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 10", "$GPGGA,000124.168,3600.0000,N,13600.0000,E,0,00,99.9,00000.0,M,0000.0,M,000.0,0000*43\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 11", "$GPGGA,132652.908,1122.3344,N,12233.4455,E,0,00,99.9,00176.1,M,0014.4,M,000.0,0000*4B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 12", "$GPGGA,112531.000,1122.3344,N,12233.4455,E,1,12,00.7,00639.3,M,0014.3,M,000.0,0000*4B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 13", "$GPGGA,111520.707,1122.3344,N,12233.4455,E,0,00,99.9,-0013.7,M,0014.3,M,000.0,0000*53\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 2", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,,00.0,00280.7,M,0014.5,M,000.0,0000*40\xd\xa"); // ERR
	UnitTest_PacketNMEA_CRC_ERR<sirf_gsu_7x::tContentGGA>("sirf_gsu_7x GGA 1", "$GPGGA,000124.168,0000.0000,N,00000.0000,E,6,02,,00280.7,M,0014.5,M,000.0,0000*5C\xd\xa"); // ERR
	//UnitTest_PacketNME_CRC<sirf_gsu_7x::tContentGGA>("$GNGGA,172905.087,,,,,0,0,,,M,,M,,*51\xd\xa");
	//UnitTest_PacketNME_CRC<sirf_gsu_7x::tContentGGA>("$GPGGA,134734,0000.0000,N,00000.0000,E,6,02,,00280.70,M,014.50,M,,*52\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGLL>("sirf_gsu_7x GLL 1", "$GPGLL,3600.0000,N,13600.0000,E,000124.168,V*2A\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGLL>("sirf_gsu_7x GLL 2", "$GPGLL,1122.3344,N,12233.4455,E,132652.908,V*22\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGLL>("sirf_gsu_7x GLL 3", "$GPGLL,1122.3344,N,12233.4455,E,112531.000,A*30\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSA>("sirf_gsu_7x GSA 1", "$GPGSA,A,1,,,,,,,,,,,,,99.9,99.9,99.9*09\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSA>("sirf_gsu_7x GSA 2", "$GPGSA,A,3,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*06\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSA>("sirf_gsu_7x GSA 3", "$GPGSA,A,3,23,29,07,08,09,18,26,28,,,,,12.3,45.6,78.9*08\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<sirf_gsu_7x::tContentGSA>("sirf_gsu_7x GSA 10", "$GPGSA,A,8,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*0D\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<sirf_gsu_7x::tContentGSA>("sirf_gsu_7x GSA 11", "$GPGSA,R,3,07,30,21,09,20,05,27,16,04,13,08,14,01.3,00.7,01.0*15\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  1", "$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  2", "$GPGSV,3,2,10,10,07,189,,05,05,220,,09,34,274,42,18,25,309,44*72\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  3", "$GPGSV,3,3,10,26,82,187,47,28,43,056,46*77\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  4", "$GPGSV,4,1,13,28,59,070,15,13,52,205,30,15,49,261,21,17,43,151,*71\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  5", "$GPGSV,4,2,13,24,26,290,,19,26,168,16,30,23,113,16,11,19,050,29*70\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  6", "$GPGSV,4,3,13,01,15,065,18,18,13,311,10,10,10,342,14,20,03,255,*79\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  7", "$GPGSV,4,4,13,08,01,029,*49\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  8", "$GLGSV,3,1,09,75,74,051,,84,40,051,,85,39,123,,66,37,279,*68\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV  9", "$GLGSV,3,2,09,74,23,089,,67,22,338,,65,12,223,,83,05,008,*62\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV 10", "$GLGSV,3,3,09,86,03,164,*52\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentGSV>("sirf_gsu_7x GSV 11", "$GPGSV,3,1,12,07,48,075,74,10,36,135,,13,-2,128,,16,-5,240,34*79\xd\xa"); // that's a real one but it's not correct.
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentRMC>("sirf_gsu_7x RMC 1", "$GPRMC,000400.148,V,3600.0000,N,13600.0000,E,9999.99,999.99,180109,,*24\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentRMC>("sirf_gsu_7x RMC 2", "$GPRMC,123456.789,V,1122.3344,N,01122.3344,E,9999.99,999.99,180109,,*2D\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentRMC>("sirf_gsu_7x RMC 3", "$GPRMC,123456.789,V,1122.3344,N,12233.4455,E,9999.99,999.99,180109,,*2C\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentRMC>("sirf_gsu_7x RMC 4", "$GPRMC,123456.789,A,1122.3344,N,01122.3344,E,0052.58,132.68,180109,,*37\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentRMC>("sirf_gsu_7x RMC 5", "$GPRMC,123456.789,A,1122.3344,N,12233.4455,E,0052.58,132.68,180109,,*36\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentVTG>("sirf_gsu_7x VTG 1", "$GPVTG,999.99,T,,M,9999.99,N,9999.99,K*59\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentVTG>("sirf_gsu_7x VTG 2", "$GPVTG,132.68,T,,M,0052.58,N,0097.38,K*51\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentZDA>("sirf_gsu_7x ZDA 1", "$GPZDA,000125.168,25,01,2009,,*52\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentZDA>("sirf_gsu_7x ZDA 2", "$GPZDA,000125.168,25,01,2009,00,00*52\xd\xa"); // last two fields are not supported
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentZDA>("sirf_gsu_7x ZDA 3", "$GPZDA,000125.168,25,01,2009,-3,59*40\xd\xa"); // last two fields are not supported
	UnitTest_PacketNMEA_CRC<sirf_gsu_7x::tContentZDA>("sirf_gsu_7x ZDA 4", "$GPZDA,000125.168,25,01,2009,-13,59*71\xd\xa"); // last two fields are not supported

	std::cout << '\n';

	// SiRF LR9548S
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGGA>("sirf_lr9548s GGA 1", "$GPGGA,001630.035,0000.0000,N,00000.0000,E,0,00,50.0,0.0,M,0.0,M,0.0,0000*74\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGGA>("sirf_lr9548s GGA 2", "$GPGGA,215750.098,1122.3344,N,12233.4455,E,1,03,3.7,-14.3,M,14.3,M,0.0,0000*6C\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGSA>("sirf_lr9548s GSA 1", "$GPGSA,A,1,,,,,,,,,,,,,50.0,50.0,50.0*05\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGSA>("sirf_lr9548s GSA 2", "$GPGSA,A,2,07,08,09,,,,,,,,,,3.8,3.7,1.0*3B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGSV>("sirf_lr9548s GSV 1", "$GPGSV,3,1,12,07,00,000,,10,00,000,,13,00,000,,16,00,000,*7B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGSV>("sirf_lr9548s GSV 2", "$GPGSV,3,3,12,09,07,000,,12,04,000,,07,00,000,,08,-3,000,*62\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentGSV>("sirf_lr9548s GSV 3", "$GPGSV,3,3,12,09,07,000,,12,04,000,,07,-10,000,,08,-11,000,*7D\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentRMC>("sirf_lr9548s RMC 1", "$GPRMC,001629.033,V,0000.0000,N,00000.0000,E,0.0,0.00,160406,,,N*4F\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentRMC>("sirf_lr9548s RMC 2", "$GPRMC,215750.098,A,1122.3344,N,12233.4455,E,0.15,19.29,160706,,,A*5B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentRMC>("sirf_lr9548s RMC 3", "$GPRMC,215750.098,A,1122.3344,N,12233.4455,E,1230.15,19.29,160706,,,A*6B\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentVTG>("sirf_lr9548s VTG 1", "$GPVTG,0.00,T,,M,0.0,N,0.0,K,N*32\xd\xa");
	UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentVTG>("sirf_lr9548s VTG 2", "$GPVTG,19.81,T,,M,0.20,N,0.4,K,N*35\xd\xa");
	UnitTest_PacketNMEA_CRC_ERR<sirf_lr9548s::tContentRMC>("sirf_lr9548s RMC 1", "$GPRMC,215750.098,A,1122.3344,N,12233.4455,E,0.153,19.29,160706,,,A*68\xd\xa");
	{
		const auto content = UnitTest_PacketNMEA_CRC<sirf_lr9548s::tContentRMC>("sirf_lr9548s RMC 2", "$GPRMC,112658.429,A,1234.5678,N,12345.6789,E,0.64,182.50,100407,,,A*6F\xd\xa"); // 2007-04-10 11:26:58.429
		const std::pair<std::time_t, std::uint32_t> time = content.GetDateTime();
		const std::chrono::year_month_day ymd_real(std::chrono::year(2026), std::chrono::month(11), std::chrono::day(24));
		const auto tp_ymd_wnro = wnro::GetCorrectedDateWNRO(time.first, ymd_real);		
		utils::test::RESULT("sirf_lr9548s RMC 5 - date, time & WNRO", time.first == 1176204418 && time.second == 429 && tp_ymd_wnro == ymd_real);
	}

	std::cout << '\n';

	// Telit
	UnitTest_PacketNMEA_CRC<telit::tContentJAM_SIGNAL>("telit JAM 1", "$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1574.990234*65\xd\xa");
	UnitTest_PacketNMEA_CRC<telit::tContentJAM_SIGNAL>("telit JAM 2", "$PTWS,JAM,SIGNAL,VAL,INDEX,18,FREQ,1574.990234*54\xd\xa");
	UnitTest_PacketNMEA_CRC<telit::tContentVERSION>("telit VERSION 1", "$PTWS,VERSION,VAL,v13-2.2.0-STD-3.8.13-N96-B2*3F\xd\xa");

	std::cout << '\n';

	// U-Blox TIM-4H
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGGA>("ublox_tim4h GGA 1", "$GPGGA,,,,,,0,00,99.99,,,,,,*48\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGGA>("ublox_tim4h GGA 2", "$GPGGA,221314.00,,,,,0,00,99.99,,,,,,*61\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGGA>("ublox_tim4h GGA 3", "$GPGGA,223154.99,1234.56789,N,12345.67891,E,1,05,1.26,-582.2,M,14.4,M,,*7F\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGSA>("ublox_tim4h GSA 1", "$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGSA>("ublox_tim4h GSA 2", "$GPGSA,A,2,02,23,10,,,,,,,,,,11.62,11.57,1.00*06\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGLL>("ublox_tim4h GLL 1", "$GPGLL,,,,,,V,N*64\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGLL>("ublox_tim4h GLL 2", "$GPGLL,,,,,112208.00,V,N*42\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGLL>("ublox_tim4h GLL 3", "$GPGLL,1234.56789,N,12345.67891,E,113507.00,A,A*69\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGSV>("ublox_tim4h GSV 1", "$GPGSV,1,1,00*79\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGSV>("ublox_tim4h GSV 2", "$GPGSV,2,1,07,01,11,111,,02,12,112,,03,13,305,35,04,07,044,47*7F\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentGSV>("ublox_tim4h GSV 3", "$GPGSV,2,1,06,02,23,285,,24,09,071,,14,,,32,08,49,286,35*42\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentRMC>("ublox_tim4h RMC 1", "$GPRMC,,V,,,,,,,,,,N*53\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentRMC>("ublox_tim4h RMC 2", "$GPRMC,112207.00,V,,,,,,,240410,,,N*79\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentRMC>("ublox_tim4h RMC 3", "$GPRMC,113507.00,A,1234.56789,N,12345.67891,E,1.637,52.16,240410,,,A*5E\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentRMC>("ublox_tim4h RMC 4", "$GPRMC,091106.00,A,1234.56789,N,12345.67891,E,0.900,251.61,030407,,,A*68\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentVTG>("ublox_tim4h VTG 1", "$GPVTG,,,,,,,,,N*30\xd\xa"); // Statements in fixed fields can disappear in empty messages.
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentVTG>("ublox_tim4h VTG 2", "$GPVTG,52.16,T,,M,1.637,N,3.033,K,A*0D\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentVTG>("ublox_tim4h VTG 3", "$GPVTG,251.61,T,,M,0.900,N,1.669,K,A*3D\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentZDA>("ublox_tim4h ZDA 1", "$GPZDA,,,,,00,00*48\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentZDA>("ublox_tim4h ZDA 2", "$GPZDA,112206.00,24,04,2010,00,00*61\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentZDA>("ublox_tim4h ZDA 3", "$GPZDA,091106.00,03,04,2007,00,00*6B\xd\xa");
	{
		const auto content = UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentZDA>("ublox_tim4h ZDA 4", "$GPZDA,091106.00,03,04,2007,00,00*6B\xd\xa");
		const std::pair<std::time_t, std::uint32_t> time = content.GetDateTime();
		const std::chrono::year_month_day ymd_real(std::chrono::year(2026), std::chrono::month(11), std::chrono::day(17));
		const auto tp_ymd_wnro = wnro::GetCorrectedDateWNRO(time.first, ymd_real);
		utils::test::RESULT("ublox_tim4h ZDA 5 - date, time & WNRO", time.first == 1175591466 && time.second == 0 && tp_ymd_wnro == ymd_real);
	}
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 1", "$GPTXT,01,01,02,u-blox ag - www.u-blox.com*50\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 2", "$GPTXT,01,01,02,ANTARIS ATR062x HW 04070501*2C\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 3", "$GPTXT,01,01,02,EXT CORE       5.00    Jan 09 2006 12:00:00*6F\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 4", "$GPTXT,01,01,02,INT EXT0 (RCV) M4H1.1  Jan 09 2006 15:39:54*79\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 5", "$GPTXT,01,01,02,LIC 1234-5678-90AB-CDEF-4312*29\xd\xa");
	UnitTest_PacketNMEA_CRC<ublox_tim4h::tContentTXT>("ublox_tim4h TXT 6", "$GPTXT,01,01,02,QDS 12345678901*1B\xd\xa");

	std::cout << '\n';
}

}
