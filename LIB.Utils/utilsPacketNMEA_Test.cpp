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
