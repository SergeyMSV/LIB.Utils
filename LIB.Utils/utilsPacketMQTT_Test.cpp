#include "utilsPacketMQTT.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{

using namespace packet::mqtt;
using namespace packet::mqtt::hidden;

template<typename T>
void UnitTest_PacketMQTT(const T& pack, const std::string& testName)
{
	std::cout << '\n';
	std::cout << pack.ToString() << '\n';
	auto PackVector = pack.ToVector();
	std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

	auto Pack_parsed = T::Parse(PackVector);
	utils::test::RESULT(testName, Pack_parsed.has_value() && Pack_parsed == pack);
}

void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength);
void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data);
void UnitTest_PacketMQTT_RemainingLengthToVector(const std::string& cap, std::uint32_t packetLength, const std::vector<std::uint8_t>& data);
void UnitTest_PacketMQTT_RemainingLengthToVectorWrong(const std::string& cap, std::uint32_t packetLength);

void UnitTest_PacketMQTT()
{
	UnitTest_PacketMQTT_RemainingLengthParse("1 B", { 0x7F }, 127);
	UnitTest_PacketMQTT_RemainingLengthParse("2 B", { 0xFF, 0x7F }, 16383);
	UnitTest_PacketMQTT_RemainingLengthParse("3 B", { 0xFF, 0xFF, 0x7F }, 2097151);
	UnitTest_PacketMQTT_RemainingLengthParse("4 B", { 0xFF, 0xFF, 0xFF, 0x7F }, 268435455);
	UnitTest_PacketMQTT_RemainingLengthParseWrong("Empty", { });
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF", { 0xFF });
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF", { 0xFF, 0xFF });
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF });
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF, 0xFF });
	UnitTest_PacketMQTT_RemainingLengthParseWrong("0xFF, 0xFF, 0xFF, 0xFF, 0x7F", { 0xFF, 0xFF, 0xFF, 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVector("0", 0, { 0x00 });
	UnitTest_PacketMQTT_RemainingLengthToVector("127", 127, { 127 });
	UnitTest_PacketMQTT_RemainingLengthToVector("16383", 16383, { 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVector("2097151", 2097151, { 0xFF, 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVector("268435455 = 0x0FFFFFFF (MAX)", 268435455, { 0xFF, 0xFF, 0xFF, 0x7F });
	UnitTest_PacketMQTT_RemainingLengthToVectorWrong("0x4FFFFFFF", 0x4FFFFFFF);

	{
		std::vector<std::uint8_t> Data{ 0x10, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 };
		tSpan DataSpan(Data);
		auto PackType = TestPacket(DataSpan);
		utils::test::RESULT("TestPacket 1", PackType.has_value() && *PackType == tControlPacketType::CONNECT);
	}

	{
		std::vector<std::uint8_t> Data{ 0x00, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 };
		tSpan DataSpan(Data);
		auto PackType = TestPacket(DataSpan);
		utils::test::RESULT("TestPacket tError::PacketType 1", !PackType.has_value());
	}

	{

		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 });
		if (Pack.has_value())
			std::cout << Pack->ToString() << '\n';
		utils::test::RESULT("Pack CONNECT Parse MQTT 3.1.1", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQTT");
	}

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x1e, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x05, 0xc2, 0x00, 0x3c, 0x00, 0x04, 0x6d, 0x79, 0x50, 0x79, 0x00, 0x06, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x00, 0x04, 0x70, 0x61, 0x73, 0x73});
		if (Pack.has_value())
			std::cout << Pack->ToString() << '\n';
		utils::test::RESULT("Pack CONNECT Parse MQTT v.5", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQTT");
	}

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x55, 0x00, 0x06, 0x4d, 0x51, 0x49, 0x73, 0x64, 0x70, 0x03, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 });
		if (Pack.has_value())
			std::cout << Pack->ToString() << '\n';
		utils::test::RESULT("Pack CONNECT Parse MQTT v.3.1 MQIsdp (presumably)", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQIsdp");
	}

	UnitTest_PacketMQTT(tPacketCONNECT(true, 10, "my_client_id", tQoS::AtLeastOnceDelivery, true, "my_will_topic", "my_will_message", "my_user_name", "my_password"), "Pack CONNECT serialize-deserialize 1");
	UnitTest_PacketMQTT<tPacketBase<tVariableHeaderCONNECT, tPayloadCONNECT>>(tPacketCONNECT(true, 10, "my_client_id", tQoS::AtLeastOnceDelivery, true, "my_will_topic", "my_will_message", "my_user_name", "my_password"), "Pack CONNECT serialize-deserialize 2");
	UnitTest_PacketMQTT(tPacketCONNECT(true, 10, "my_client_id345678901234567", tQoS::AtLeastOnceDelivery, true, "my_will_topic", "my_will_message", "my_user_name", "my_password"), "Pack CONNECT ClientId size - 23 symbols = my_client_id34567890123");
	UnitTest_PacketMQTT(tPacketCONNACK(true, tConnectReturnCode::ConnectionRefused_NotAuthorized), "Pack CONNACK serialize-deserialize");
	UnitTest_PacketMQTT(tPacketPUBLISH(true, true, "my_topic_name/some_device", tQoS::AtLeastOnceDelivery, 10), "Pack PUBLISH serialize-deserialize 1");
	UnitTest_PacketMQTT(tPacketPUBLISH(true, true, "my_topic_name/some_device"), "Pack PUBLISH serialize-deserialize 2");
	UnitTest_PacketMQTT(tPacketPUBLISH(true, true, "my_topic_name/some_device", tQoS::AtLeastOnceDelivery, 10, { 0x01, 0x02 }), "Pack PUBLISH + Payload serialize-deserialize 1");
	UnitTest_PacketMQTT(tPacketPUBLISH(true, true, "my_topic_name/some_device", { 0x01, 0x02 }), "Pack PUBLISH + Payload serialize-deserialize 2");
	UnitTest_PacketMQTT(tPacketPUBACK(0x1234), "Pack PUBACK serialize-deserialize");
	UnitTest_PacketMQTT(tPacketPUBREC(0x1234), "Pack PUBREC serialize-deserialize");
	UnitTest_PacketMQTT(tPacketPUBREL(0x1234), "Pack PUBREL serialize-deserialize");
	UnitTest_PacketMQTT(tPacketPUBCOMP(0x1234), "Pack PUBCOMP serialize-deserialize");
	{
		tPacketSUBSCRIBE Pack(0x1234, "my_topic_filter_1", tQoS::AtMostOnceDelivery);
		Pack.AddTopicFilter("my_topic_filter_2", tQoS::ExactlyOnceDelivery);
		Pack.AddTopicFilter("my_topic_filter_3", tQoS::AtLeastOnceDelivery);
		UnitTest_PacketMQTT(Pack, "Pack SUBSCRIBE serialize-deserialize");
	}
	UnitTest_PacketMQTT(tPacketSUBACK(0x1234, tSubscribeReturnCode::SuccessMaximumQoS_ExactlyOnceDelivery), "Pack SUBACK serialize-deserialize");
	{
		tPacketUNSUBSCRIBE Pack(0x1234, "my_topic_filter_1");
		Pack.AddTopicFilter("my_topic_filter_2");
		Pack.AddTopicFilter("my_topic_filter_3");
		UnitTest_PacketMQTT(Pack, "Pack UNSUBSCRIBE serialize-deserialize");
	}
	UnitTest_PacketMQTT(tPacketUNSUBACK(0x1234), "Pack UNSUBACK serialize-deserialize");

	UnitTest_PacketMQTT(tPacketPINGREQ(), "Pack PINGREQ serialize-deserialize");
	UnitTest_PacketMQTT(tPacketPINGRESP(), "Pack PINGRESP serialize-deserialize");
	UnitTest_PacketMQTT(tPacketDISCONNECT(), "Pack DISCONNECT serialize-deserialize");

	{ // copy c_tor and move c_tor
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x55, 0x00, 0x06, 0x4d, 0x51, 0x49, 0x73, 0x64, 0x70, 0x03, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 });
		if (Pack.has_value())
			std::cout << Pack->ToString() << '\n';
		tPacketCONNECT Pack2 = *Pack;
		tPacketCONNECT Pack3 = std::move(*Pack);
		//utils::test::RESULT("????", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQIsdp");
	}

	{ // copy c_tor and move c_tor
		auto PackVector = tPacketPUBLISH(true, true, "my_topic_name/some_device", tQoS::AtLeastOnceDelivery, 10, { 0x01, 0x02, 0x03, 0x04 }).ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack = tPacketPUBLISH::Parse(PackVector);
		if (Pack.has_value())
			std::cout << Pack->ToString() << '\n';
		tPacketPUBLISH Pack2 = *Pack;
		tPacketPUBLISH Pack3 = std::move(*Pack);
		Pack2 = *Pack;
		Pack2 = std::move(Pack3);

		//utils::test::RESULT("????", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQIsdp");
	}

	
}

void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength)
{
	tSpan DataSpan(data);
	tRemainingLengthParseExp Length = tRemainingLength::Parse(DataSpan);
	utils::test::RESULT("RemainingLength Parse " + cap, Length.has_value() && Length == packetLength);
}

void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data)
{
	tSpan DataSpan(data);
	tRemainingLengthParseExp Length = tRemainingLength::Parse(DataSpan);
	utils::test::RESULT("RemainingLength Parse WRONG " + cap, !Length.has_value());
}

void UnitTest_PacketMQTT_RemainingLengthToVector(const std::string& cap, std::uint32_t packetLength, const std::vector<std::uint8_t>& data)
{
	tRemainingLengthToVectorExp Length = tRemainingLength::ToVector(packetLength);
	utils::test::RESULT("RemainingLength ToVector " + cap, Length.has_value() && Length == data);
}

void UnitTest_PacketMQTT_RemainingLengthToVectorWrong(const std::string& cap, std::uint32_t packetLength)
{
	tRemainingLengthToVectorExp Length = tRemainingLength::ToVector(packetLength);
	utils::test::RESULT("RemainingLength ToVector WRONG " + cap, !Length.has_value());
}

}