#include "utilsPacketMQTT.h"
#include "utilsTest.h"

//#include <cassert>

//#include <algorithm>
#include <iostream>
#include <ranges>

namespace utils
{

using namespace packet_MQTT;
void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength);
void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tError error);
void UnitTest_PacketMQTT_RemainingLengthToVector(const std::string& cap, std::uint32_t packetLength, const std::vector<std::uint8_t>& data);
void UnitTest_PacketMQTT_RemainingLengthToVectorWrong(const std::string& cap, std::uint32_t packetLength, tError error);

//void TestFun(std::ranges::views:: <std::vector<std::uint8_t>> e)
//void TestFun(std::ranges::view<std::vector<std::uint8_t>> e)
//static void TestFun(std::ranges::view<std::vector<int>> e)
//{
//}
//void printElements(std::ranges::input_range auto&& range) {
//	std::ranges::for_each(range, [](const auto& e) { std::cout << e << '\n'; });
//}

void UnitTest_PacketMQTT()
{
	UnitTest_PacketMQTT_RemainingLengthParse("1 B", { 0x7F }, 127);
	UnitTest_PacketMQTT_RemainingLengthParse("2 B", { 0xFF, 0x7F }, 16383);
	UnitTest_PacketMQTT_RemainingLengthParse("3 B", { 0xFF, 0xFF, 0x7F }, 2097151);
	UnitTest_PacketMQTT_RemainingLengthParse("4 B", { 0xFF, 0xFF, 0xFF, 0x7F }, 268435455);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("Empty", { }, tError::LengthTooShort);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF", { 0xFF }, tError::LengthNotAll);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF", { 0xFF, 0xFF }, tError::LengthNotAll);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF }, tError::LengthNotAll);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF, 0xFF }, tError::LengthTooLong);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF, 0xFF, 0x7F", { 0xFF, 0xFF, 0xFF, 0xFF, 0x7F }, tError::LengthTooLong);
	UnitTest_PacketMQTT_RemainingLengthToVector("0", 0, { 0x00 });
	UnitTest_PacketMQTT_RemainingLengthToVector("127", 127, { 127 });
	UnitTest_PacketMQTT_RemainingLengthToVector("16383", 16383, { 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVector("2097151", 2097151, { 0xFF, 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVector("268435455 = 0x0FFFFFFF (MAX)", 268435455, { 0xFF, 0xFF, 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVectorWrong("0x4FFFFFFF", 0x4FFFFFFF, tError::LengthOverflow);

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0a });
		utils::test::RESULT("Pack CONNECT Parse MQTT", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQTT");
	}

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x00, 0x06, 0x4d, 0x51, 0x49, 0x73, 0x64, 0x70, 0x03, 0xce, 0x00, 0x0a });
		utils::test::RESULT("Pack CONNECT Parse MQIsdp", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQIsdp");
	}

	//tString sds;
	//sds.

	//packet_MQTT::
	//tVariableHeaderConnect sdf(123);

	tPacketCONNECT Pack;
	tPacketCONNECT Pack1("my_client_id", "my_will_topic", "my_will_message", "my_user_name", "my_password");
	auto Pack1Vector = Pack1.ToVector();
	std::cout << utils::test::ToStringHEX(Pack1Vector, true) << '\n';

	auto Pack1_parsed = tPacketCONNECT::Parse(Pack1Vector);


	//{
	//	std::vector<int> r = { 1, 2, 3, 4, 5 };

	//	std::ranges::reverse_view rv(r);

	//	for (auto i : rv)
	//		std::cout << i << " ";
	//}


	//Test: The same
	//{
	//	tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

	//	char Data[] = "\xd\xa""CLOSED\xd\xa";

	//	std::vector<char> DataVector(Data, Data + sizeof(Data));

	//	bool Result = Trap.Try(DataVector);

	//	utils::test::RESULT("The same", Result);
	//}	

	tFixedHeader Fh = MakePUBLISH(false, 0xFF, false);
}

void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength)
{
	std::size_t Offset = 0;
	tRemainingLengthParseExp Length = tRemainingLength::Parse(data, Offset);
	utils::test::RESULT("RemainingLength Parse " + cap, Length.has_value() && Length == packetLength);
}

void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tError error)
{
	std::size_t Offset = 0;
	tRemainingLengthParseExp Length = tRemainingLength::Parse(data, Offset);
	utils::test::RESULT("RemainingLength Parse WRONG " + cap, !Length.has_value() && Length.error() == error);
}

void UnitTest_PacketMQTT_RemainingLengthToVector(const std::string& cap, std::uint32_t packetLength, const std::vector<std::uint8_t>& data)
{
	tRemainingLengthToVectorExp Length = tRemainingLength::ToVector(packetLength);
	utils::test::RESULT("RemainingLength ToVector " + cap, Length.has_value() && Length == data);
}

void UnitTest_PacketMQTT_RemainingLengthToVectorWrong(const std::string& cap, std::uint32_t packetLength, tError error)
{
	tRemainingLengthToVectorExp Length = tRemainingLength::ToVector(packetLength);
	utils::test::RESULT("RemainingLength ToVector WRONG " + cap, !Length.has_value() && Length.error() == error);
}

}