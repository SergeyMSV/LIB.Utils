//2021-10-29
#pragma once

#include "utilsTest.h"

#include <string>
#include <vector>

namespace utils
{

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::vector<std::uint8_t>& raw, const std::vector<std::uint8_t>& data)
{
	std::vector<std::uint8_t> DataVector = raw;

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	if (data.empty())
	{
		Result = !Result;
	}
	else
	{
		const std::vector<std::uint8_t> A = Packet.ToVector();

		Result = Result &&
			A == data;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::string& raw, const std::string& data)
{
	UnitTest_Packet_Find<T>(testName, std::vector<std::uint8_t>(raw.begin(), raw.end()), std::vector<std::uint8_t>(data.begin(), data.end()));
}

template<typename T>
void UnitTest_Packet_Parse(const std::string& testName, const std::vector<std::uint8_t>& raw, const T& packet)
{
	typename T::payload_value_type checkPacketData = packet.GetPayloadValue();

	std::vector<std::uint8_t> DataVector = raw;

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	typename T::payload_value_type PacketData = Packet.GetPayloadValue();

	if (PacketData.size() != checkPacketData.size() || PacketData != checkPacketData)
	{
		Result = false;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_Packet_Parse(const std::string& testName, const std::string& raw, const T& packet)
{
	UnitTest_Packet_Parse<T>(testName, std::vector<std::uint8_t>(raw.begin(), raw.end()), packet);
}

template<typename T>
void UnitTest_Packet_Make(const std::string& testName, const T& packet)
{
	const std::vector<std::uint8_t> DataVector = packet.ToVector();

	UnitTest_Packet_Parse<T>(testName, DataVector, packet);
}

}