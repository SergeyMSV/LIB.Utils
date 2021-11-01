//2021-10-29
#pragma once

#include "utilsBase.h"
#include "utilsTest.h"

#include <string>

namespace utils
{

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const tVectorUInt8& raw, const tVectorUInt8& data)
{
	tVectorUInt8 DataVector = raw;

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	if (data.empty())
	{
		Result = !Result;
	}
	else
	{
		const tVectorUInt8 A = Packet.ToVector();

		Result = Result &&
			A == data;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_Packet_Find(const std::string& testName, const std::string& raw, const std::string& data)
{
	UnitTest_Packet_Find<T>(testName, tVectorUInt8(raw.begin(), raw.end()), tVectorUInt8(data.begin(), data.end()));
}

template<typename T>
void UnitTest_Packet_Parse(const std::string& testName, const tVectorUInt8& raw, const typename T::payload_value_type& data)
{
	tVectorUInt8 DataVector = raw;

	T Packet;

	bool Result = T::Find(DataVector, Packet);

	typename T::payload_value_type PacketData = Packet.GetPayload();

	if (PacketData.size() != data.size() || PacketData != data)
	{
		Result = false;
	}

	utils::test::RESULT(testName.c_str(), Result);
}

template<typename T>
void UnitTest_Packet_Parse(const std::string& testName, const std::string& raw, const typename T::payload_value_type& data)
{
	UnitTest_Packet_Parse<T>(testName, tVectorUInt8(raw.begin(), raw.end()), data);
}

template<typename T>
void UnitTest_Packet_Make(const std::string& testName, const typename T::payload_value_type& data)
{
	T Packet(data);

	const tVectorUInt8 DataVector = Packet.ToVector();

	UnitTest_Packet_Parse<T>(testName, DataVector, data);
}

}