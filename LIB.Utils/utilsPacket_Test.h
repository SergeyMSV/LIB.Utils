// 2021-10-29
#pragma once

#include "utilsTest.h"

#include <optional>
#include <string>
#include <vector>

namespace utils
{

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::vector<std::uint8_t>& raw, const std::vector<std::uint8_t>& data)
{
	std::vector<std::uint8_t> DataVector = raw;

	std::optional<T> PacketOpt = T::Find(DataVector);
	bool Result = PacketOpt.has_value();

	if (data.empty())
	{
		Result = !Result;
	}
	else if (Result)
	{
		const std::vector<std::uint8_t> A = PacketOpt->ToVector();

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
void UnitTest_Packet_Find(const std::string& testName, const std::vector<std::uint8_t>& raw1, const std::vector<std::uint8_t>& raw2, const std::vector<std::uint8_t>& data)
{
	std::vector<std::uint8_t> dataRaw = raw1;

	std::optional<T> PacketOpt = T::Find(dataRaw);
	if (!PacketOpt.has_value())
	{
		dataRaw.insert(dataRaw.end(), raw2.begin(), raw2.end());
		PacketOpt = T::Find(dataRaw);
	}
	bool Result = PacketOpt.has_value();

	if (data.empty())
	{
		Result = !Result;
	}
	else if (Result)
	{
		const std::vector<std::uint8_t> A = PacketOpt->ToVector();

		Result = Result &&
			A == data;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::vector<std::uint8_t>& raw1, const std::vector<std::uint8_t>& raw2, const T& packet)
{
	UnitTest_Packet_Find<T>(testName, raw1, raw2, packet.ToVector());
}

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::string& raw1, const std::string& raw2, const std::string& data)
{
	UnitTest_Packet_Find<T>(testName, std::vector<std::uint8_t>(raw1.begin(), raw1.end()), std::vector<std::uint8_t>(raw2.begin(), raw2.end()), std::vector<std::uint8_t>(data.begin(), data.end()));
}

template<typename T>
void UnitTest_Packet_Parse(const std::string& testName, const std::vector<std::uint8_t>& raw, const T& packet)
{
	typename T::payload_value_type checkPacketData = packet.GetPayloadValue();

	std::vector<std::uint8_t> DataVector = raw;

	std::optional<T> PacketOpt = T::Find(DataVector);
	bool Result = PacketOpt.has_value();

	if (Result)
	{
		typename T::payload_value_type PacketData = PacketOpt->GetPayloadValue();

		if (PacketData.size() != checkPacketData.size() || PacketData != checkPacketData)
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