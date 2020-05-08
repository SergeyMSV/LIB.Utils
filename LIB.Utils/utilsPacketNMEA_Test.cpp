#include "utilsPacketNMEA.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

template<typename T>
void UnitTest_PacketNMEA_Find(const std::string& testName, const std::string& raw, const std::string& data)
{
	tVectorUInt8 DataVector(raw.begin(), raw.end());

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	{
		tVectorUInt8 A = Packet.ToVector();

		tVectorUInt8 B(data.begin(), data.end());

		Result = Result &&
			A == B;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_PacketNMEA_Parse(const std::string& testName, const std::string& raw, const typename T::payload_value_type& data)
{
	tVectorUInt8 DataVector(raw.begin(), raw.end());

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	typename T::payload_value_type PacketData = Packet.GetPayload();

	if (PacketData.size() == data.size())
	{
		for (std::size_t i = 0; i < PacketData.size(); ++i)
		{
			if (PacketData != data)
			{
				Result = false;

				break;
			}
		}
	}
	else
	{
		Result = false;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_PacketNMEA_Make(const std::string& testName, const typename T::payload_value_type& data)
{
	T Packet;

	Packet.SetPayload(data);

	tVectorUInt8 DataVector = Packet.ToVector();

	std::string DataMsg(DataVector.cbegin(), DataVector.cend());

	UnitTest_PacketNMEA_Parse<T>(testName, DataMsg, data);
}

void UnitTest_PacketNMEA()
{
	std::cout << "\n""utils::packet::packet_NMEA\n";

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadCommon> tPacketNMEA;

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: Just a packet",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: Rubbish",
		"GNGG$GNGG$GNGGA,221$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa,081,36*7F\xd\xa",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: Wrong Packet + Right Packet",
		"$GPGSV,3,1,10,23,38,230,44,29,21,156,47,07,29,116,41,08,09,081,36*7F\xd\xa$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa",
		"$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: GGA PayloadSize=84 (more than stipulated in NMEA)",
		"$GPGGA,192758.000,5555.4159,N,03745.0626,E,1,08,01.0,00309.6,M,0014.4,M,000.0,0000*48\xd\xa",
		"$GPGGA,192758.000,5555.4159,N,03745.0626,E,1,08,01.0,00309.6,M,0014.4,M,000.0,0000*48\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: Find GGA 1",
		"$GPGGA,090210.000,5539.564975,N,03732.411956,E,1,13,0.90,231.400,M,14.535,M,,*62\xd\xa",
		"$GPGGA,090210.000,5539.564975,N,03732.411956,E,1,13,0.90,231.400,M,14.535,M,,*62\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA>("Parse CRC: Find RMC 1",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa");

	UnitTest_PacketNMEA_Parse<tPacketNMEA>("Parse CRC: Parse RMC 1",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa",
		{ "GNRMC", "090210.000", "A", "5539.564975", "N", "03732.411956", "E", "0.03", "274.40", "120517", "", "", "A" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make GNRMC",
		{ "GNRMC", "090210.000", "A", "5539.564975", "N", "03732.411956", "E", "0.03", "274.40", "120517", "", "", "A" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC Empty",
		{ "MYRMC" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 1",
		{ "MYRMC", "TestMSG" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 2",
		{ "MYRMC", "PartA", "PartB" });

	UnitTest_PacketNMEA_Make<tPacketNMEA>("Parse CRC: Make MYRMC 2",
		{ "MYRMC", "PartA", "PartB", "PartC" });


	std::cout << "\n""utils::packet_NMEA::tPayloadString\n";

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEA, utils::packet_NMEA::tPayloadString> tPacketNMEA2;

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: Just a packet",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: Rubbish",
		"GNGG$GNGG$GNGGA,221$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa,081,36*7F\xd\xa",
		"$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: Wrong Packet + Right Packet",
		"$GPGSV,3,1,10,23,38,230,44,29,21,156,47,07,29,116,41,08,09,081,36*7F\xd\xa$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa",
		"$GNGGA,221325.000,,,,,0,0,,,M,,M,,*53\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: GGA PayloadSize=84 (more than stipulated in NMEA)",
		"$GPGGA,192758.000,5555.4159,N,03745.0626,E,1,08,01.0,00309.6,M,0014.4,M,000.0,0000*48\xd\xa",
		"$GPGGA,192758.000,5555.4159,N,03745.0626,E,1,08,01.0,00309.6,M,0014.4,M,000.0,0000*48\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: Find GGA 1",
		"$GPGGA,090210.000,5539.564975,N,03732.411956,E,1,13,0.90,231.400,M,14.535,M,,*62\xd\xa",
		"$GPGGA,090210.000,5539.564975,N,03732.411956,E,1,13,0.90,231.400,M,14.535,M,,*62\xd\xa");

	UnitTest_PacketNMEA_Find<tPacketNMEA2>("Parse CRC: Find RMC 1",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa");

	UnitTest_PacketNMEA_Parse<tPacketNMEA2>("Parse CRC: Parse RMC 1",
		"$GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A*71\xd\xa",
		"GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A");

	UnitTest_PacketNMEA_Make<tPacketNMEA2>("Parse CRC: Make GNRMC",
		"GNRMC,090210.000,A,5539.564975,N,03732.411956,E,0.03,274.40,120517,,,A");

	UnitTest_PacketNMEA_Make<tPacketNMEA2>("Parse CRC: Make MYRMC Empty", "MYRMC");

	UnitTest_PacketNMEA_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 1", "MYRMC,Test_1");

	UnitTest_PacketNMEA_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 2", "MYRMC,Test_1,Test_2");

	UnitTest_PacketNMEA_Make<tPacketNMEA2>("Parse CRC: Make MYRMC 2", "MYRMC,Test_1,Test_2,Test_3");


	std::cout << "\n""tPacketNMEABin\n";

	typedef utils::packet::tPacket<utils::packet_NMEA::tFormatNMEABin, utils::packet_NMEA::tPayloadCommon> tPacketNMEABin;

	UnitTest_PacketNMEA_Parse<tPacketNMEABin>("Parse CRC: Just a encapsulation packet",
		"!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa",
		{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	//CRC matches by chance
	//UnitTest_PacketNMEA_Parse<tPacketNMEABin>("Parse CRC: Rubbish & encapsulation packet",
	//	"!AIVDM!AIVDM!AIVDM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
	//	{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	//Added '5' into the header
	UnitTest_PacketNMEA_Parse<tPacketNMEABin>("Parse CRC: Rubbish & encapsulation packet",
		"!AIVDM!AIVDM!AIV5DM!AIVDM!AIVDM,1,1,,1,1P000Oh1IT1svTP2r:43grwb05q4,0*01\xd\xa,081,36*7F\xd\xa",
		{ "AIVDM", "1", "1", "", "1", "1P000Oh1IT1svTP2r:43grwb05q4", "0" });

	UnitTest_PacketNMEA_Make<tPacketNMEABin>("Parse CRC: Make MYRMC 3 encapsulation packet",
		{ "MYRMC", "PartA", "PartB", "PartC" });

	std::cout << std::endl;
}

}
