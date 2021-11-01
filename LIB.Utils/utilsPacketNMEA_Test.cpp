#include "utilsPacketNMEA.h"
#include "utilsPacket_Test.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketNMEA()
{
	std::cout << "\n""utils::packet_NMEA\n";

	using tPacketNMEA = packet::tPacket<packet_NMEA::tFormatNMEA, packet_NMEA::tPayloadCommon>;

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC Empty",
		{ "MYRMC" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 1",
		{ "MYRMC", "TestMSG" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 2",
		{ "MYRMC", "PartA", "PartB" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 2",
		{ "MYRMC", "PartA", "PartB", "PartC" });


	std::cout << "\n""utils::packet_NMEA::tPayloadString\n";

	using tPacketNMEA2 = packet::tPacket<packet_NMEA::tFormatNMEA, packet_NMEA::tPayloadString>;

	UnitTest_Packet_Make<tPacketNMEA2>("Parse CRC: Make MYRMC Empty",
		"MYRMC");

	UnitTest_Packet_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 1",
		"MYRMC,Test_1");

	UnitTest_Packet_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 2",
		"MYRMC,Test_1,Test_2");

	UnitTest_Packet_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 2",
		"MYRMC,Test_1,Test_2,Test_3");


	std::cout << "\n""tPacketNMEABin\n";

	using tPacketNMEABin = packet::tPacket<packet_NMEA::tFormatNMEABin, packet_NMEA::tPayloadCommon>;

	UnitTest_Packet_Parse<tPacketNMEABin>("Parse CRC: Just a encapsulation packet",
		"!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa",
		{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	//CRC matches by chance
	//UnitTest_Packet_Parse<tPacketNMEABin>("Parse CRC: Rubbish & encapsulation packet",
	//	"!AIVDM!AIVDM!AIVDM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
	//	{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	//Added '5' into the header
	UnitTest_Packet_Parse<tPacketNMEABin>("Parse CRC: Rubbish & encapsulation packet",
		"!AIVDM!AIVDM!AIV5DM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
		{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	UnitTest_PacketNMEA_Make<tPacketNMEABin>("Parse CRC: Make MYRMC 3 encapsulation packet",
		{ "MYRMC", "PartA", "PartB", "PartC" });

	std::cout << std::endl;
}

}
