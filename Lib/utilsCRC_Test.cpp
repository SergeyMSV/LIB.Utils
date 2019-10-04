#include "utilsCRC.h"

#include "utilsTest.h"

#include <iostream>

#include <vector>

namespace utils
{
	namespace unit_test
	{

void UnitTest_CRC()
{
	std::cout << "\n""utils::CRC" << std::endl;

	bool Result = false;

	//CRC08_BNTBX1
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_BNTBX1(Data.begin(), Data.end());

		Result = CRC == 0xF7;

		utils::test::RESULT("CRC08_BNTBX1", Result);
	}

	//CRC08_BNTBX1
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_BNTBX1(&Data[0], Data.size());

		Result = CRC == 0xF7;

		utils::test::RESULT("CRC08_BNTBX1", Result);
	}

	//CRC08_DALLAS
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_DALLAS(Data.begin(), Data.end());

		Result = CRC == 0xA1;

		utils::test::RESULT("CRC08_DALLAS", Result);
	}

	//CRC08_DALLAS
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_DALLAS(&Data[0], Data.size());

		Result = CRC == 0xA1;

		utils::test::RESULT("CRC08_DALLAS", Result);
	}

	//CRC08_GSM0710FCS
	{
		std::vector<char> Data{ 0x03,0x3F,0x01 };

		auto CRC = crc::CRC08_GSM0710FCS(Data.begin(), Data.end());

		Result = CRC == 0x1C;

		utils::test::RESULT("CRC08_GSM0710FCS", Result);
	}

	//CRC08_GSM0710FCS
	{
		std::vector<char> Data{ 0x03,0x3F,0x01 };

		auto CRC = crc::CRC08_GSM0710FCS(&Data[0], Data.size());

		Result = CRC == 0x1C;

		utils::test::RESULT("CRC08_GSM0710FCS", Result);
	}

	//CRC08_NMEA
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_NMEA(Data.begin(), Data.end());

		Result = CRC == 0x31;

		utils::test::RESULT("CRC08_NMEA", Result);
	}

	//CRC08_NMEA
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC08_NMEA(&Data[0], Data.size());

		Result = CRC == 0x31;

		utils::test::RESULT("CRC08_NMEA", Result);
	}

	//CRC16_BNTBX2
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_BNTBX2(Data.begin(), Data.end());

		Result = CRC == 0x29B1;

		utils::test::RESULT("CRC16_BNTBX2", Result);
	}

	//CRC16_BNTBX2
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_BNTBX2(&Data[0], Data.size());

		Result = CRC == 0x29B1;

		utils::test::RESULT("CRC16_BNTBX2", Result);
	}

	//CRC16_CCITT
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_CCITT(Data.begin(), Data.end());

		Result = CRC == 0x29B1;

		utils::test::RESULT("CRC16_CCITT", Result);
	}

	//CRC16_CCITT
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_CCITT(&Data[0], Data.size());

		Result = CRC == 0x29B1;

		utils::test::RESULT("CRC16_CCITT", Result);
	}

	//CRC16_ANSI
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_ANSI(Data.begin(), Data.end());

		Result = CRC == 0x4B37;

		utils::test::RESULT("CRC16_ANSI", Result);
	}

	//CRC16_ANSI
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC16_ANSI(&Data[0], Data.size());

		Result = CRC == 0x4B37;

		utils::test::RESULT("CRC16_ANSI", Result);
	}

	//CRC32_BNTBX2
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC32_BNTBX2(Data.begin(), Data.end());

		Result = CRC == 0xCBF43926;

		utils::test::RESULT("CRC32_BNTBX2", Result);
	}

	//CRC32_BNTBX2
	{
		std::vector<char> Data{ 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };

		auto CRC = crc::CRC32_BNTBX2(&Data[0], Data.size());

		Result = CRC == 0xCBF43926;

		utils::test::RESULT("CRC32_BNTBX2", Result);
	}


	std::cout << std::endl;
}

	}
}
