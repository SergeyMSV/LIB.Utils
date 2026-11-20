#include "utilsPacket.h"
#include "utilsPacketNMEA.h"
#include "utilsPacket_Test.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketNMEA()
{
	std::cout << "\n""utils::packet::nmea\n";

	using tPacketNMEA = packet::tPacket<packet::nmea::tFormatNMEA, packet::nmea::tPayloadCommon>;

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: Just a packet",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: parts",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,", "24,03,31,132,33,04,02,083,44*76\xd\xa",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: Rubbish",
		"GNGG$GNGG$GNGGA,221$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa,083,44*76F\xd\xa",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: Wrong Packet + Right Packet",
		"$GPGSV,3,1,10,11,12,103,14,21,22,101,24,03,31,132,33,04,02,083,44*76\xd\xa$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa",
		"$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: GGA PayloadSize=84 (more than stipulated in NMEA)",
		"$GPGGA,192758.000,1234.5678,N,12345.6789,E,1,08,01.0,01309.6,M,0067.4,M,000.0,0000*4A\xd\xa",
		"$GPGGA,192758.000,1234.5678,N,12345.6789,E,1,08,01.0,01309.6,M,0067.4,M,000.0,0000*4A\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: Find GGA 1",
		"$GPGGA,090210.000,1234.567891,N,12345.678912,E,1,13,0.90,1231.400,M,67.535,M,,*58\xd\xa",
		"$GPGGA,090210.000,1234.567891,N,12345.678912,E,1,13,0.90,1231.400,M,67.535,M,,*58\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: Find RMC 1",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa");

	UnitTest_Packet_Parse("Parse CRC: Parse RMC 1",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa",
		tPacketNMEA(std::vector<std::string>{ "GNRMC", "090210.000", "A", "1234.567891", "N", "12345.678912", "E", "0.03", "274.40", "120517", "", "", "A" }));

	UnitTest_Packet_Make("Parse CRC: Make GNRMC",
		tPacketNMEA(std::vector<std::string>{ "GNRMC", "090210.000", "A", "1234.567891", "N", "12345.678912", "E", "0.03", "274.40", "120517", "", "", "A" }));

	UnitTest_Packet_Find<tPacketNMEA>("Parse CRC: empty packet",
		"$*00\xd\xa",
		"");

	UnitTest_Packet_Make("Parse CRC: Make MYRMC Empty",
		tPacketNMEA(std::vector<std::string>{ "MYRMC" }));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 1",
		tPacketNMEA(std::vector<std::string>{ "MYRMC", "TestMSG" }));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 2",
		tPacketNMEA(std::vector<std::string>{ "MYRMC", "PartA", "PartB" }));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 2",
		tPacketNMEA(std::vector<std::string>{ "MYRMC", "PartA", "PartB", "PartC" }));

	std::cout << "\n""utils::packet::nmea::tPayloadString\n";

	using tPacketNMEA2 = packet::tPacket<packet::nmea::tFormatNMEA, packet::nmea::tPayloadString>;

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: Just a packet",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: Rubbish",
		"GNGG$GNGG$GNGGA,221$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa,083,44*76\xd\xa",
		"$GPGSV,3,1,10,11,12,103,14,21,22,203,24,03,31,132,33,04,02,083,44*76\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: Wrong Packet + Right Packet",
		"$GPGSV,3,1,10,11,12,103,14,21,22,101,24,03,31,132,33,04,02,083,44*76\xd\xa$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa",
		"$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: GGA PayloadSize=84 (more than stipulated in NMEA)",
		"$GPGGA,192758.000,1234.5678,N,12345.6789,E,1,08,01.0,01309.6,M,0067.4,M,000.0,0000*4A\xd\xa",
		"$GPGGA,192758.000,1234.5678,N,12345.6789,E,1,08,01.0,01309.6,M,0067.4,M,000.0,0000*4A\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: Find GGA 1",
		"$GPGGA,090210.000,1234.567891,N,12345.678912,E,1,13,0.90,1231.400,M,67.535,M,,*58\xd\xa",
		"$GPGGA,090210.000,1234.567891,N,12345.678912,E,1,13,0.90,1231.400,M,67.535,M,,*58\xd\xa");

	UnitTest_Packet_Find<tPacketNMEA2>("Parse CRC: Find RMC 1",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa");

	UnitTest_Packet_Parse("Parse CRC: Parse RMC 1",
		"$GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A*7E\xd\xa",
		tPacketNMEA2("GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A"));

	UnitTest_Packet_Make("Parse CRC: Make GNRMC",
		tPacketNMEA2("GNRMC,090210.000,A,1234.567891,N,12345.678912,E,0.03,274.40,120517,,,A"));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC Empty",
		tPacketNMEA2("MYRMC"));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 1",
		tPacketNMEA2("MYRMC,Test_1"));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 2",
		tPacketNMEA2("MYRMC,Test_1,Test_2"));

	UnitTest_Packet_Make("Parse CRC: Make MYRMC 2",
		tPacketNMEA2("MYRMC,Test_1,Test_2,Test_3"));

	std::cout << "\n""Packet NMEA AIS\n";

	using tPacketNMEAAIS = packet::tPacket<packet::nmea::tFormatAIS, packet::nmea::tPayloadCommon>;

	UnitTest_Packet_Parse("Parse CRC: Just a encapsulation packet",
		"!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa",
		tPacketNMEAAIS(std::vector<std::string>{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" }));

	//CRC matches by chance
	//UnitTest_Packet_Parse<tPacketNMEABin>("Parse CRC: Rubbish & encapsulation packet",
	//	"!AIVDM!AIVDM!AIVDM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
	//	{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	//Added '5' into the header
	UnitTest_Packet_Parse<tPacketNMEAAIS>("Parse CRC: Rubbish & encapsulation packet",
		"!AIVDM!AIVDM!AIV5DM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
		tPacketNMEAAIS(std::vector<std::string>{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" }));

	UnitTest_Packet_Make<tPacketNMEAAIS>("Parse CRC: Make MYRMC 3 encapsulation packet",
		tPacketNMEAAIS(std::vector<std::string>{ "MYRMC", "PartA", "PartB", "PartC" }));

	std::cout << std::endl;
}

}
