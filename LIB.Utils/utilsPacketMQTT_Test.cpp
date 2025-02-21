#include "utilsPacketMQTT.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{

using namespace packet_MQTT;
void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength);
void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tError error);
void UnitTest_PacketMQTT_RemainingLengthToVector(const std::string& cap, std::uint32_t packetLength, const std::vector<std::uint8_t>& data);
void UnitTest_PacketMQTT_RemainingLengthToVectorWrong(const std::string& cap, std::uint32_t packetLength, tError error);

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
		std::vector<std::uint8_t> Data{ 0x10, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 };
		tSpan DataSpan(Data);
		auto PackType = TestPacket(DataSpan);
		utils::test::RESULT("TestPacket 1", PackType.has_value() && *PackType == tControlPacketType::CONNECT);
	}

	{
		std::vector<std::uint8_t> Data{ 0x00, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 };
		tSpan DataSpan(Data);
		auto PackType = TestPacket(DataSpan);
		utils::test::RESULT("TestPacket tError::PacketType 1", !PackType.has_value() && PackType.error() == tError::PacketType);
	}

	{

		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x53, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x04, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 });
		utils::test::RESULT("Pack CONNECT Parse MQTT 3.1.1", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQTT");
	}

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x1e, 0x00, 0x04, 0x4d, 0x51, 0x54, 0x54, 0x05, 0xc2, 0x00, 0x3c, 0x00, 0x04, 0x6d, 0x79, 0x50, 0x79, 0x00, 0x06, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x00, 0x04, 0x70, 0x61, 0x73, 0x73});
		utils::test::RESULT("Pack CONNECT Parse MQTT v.5", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQTT");
	}

	{
		auto Pack = tPacketCONNECT::Parse(std::vector<std::uint8_t>{ 0x10, 0x55, 0x00, 0x06, 0x4d, 0x51, 0x49, 0x73, 0x64, 0x70, 0x03, 0xce, 0x00, 0x0b, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x5f, 0x69, 0x64, 0x00, 0x0d, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x74, 0x6f, 0x70, 0x69, 0x63, 0x00, 0x0f, 0x6d, 0x79, 0x5f, 0x77, 0x69, 0x6c, 0x6c, 0x5f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x00, 0x0c, 0x6d, 0x79, 0x5f, 0x75, 0x73, 0x65, 0x72, 0x5f, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x0b, 0x6d, 0x79, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64 });
		utils::test::RESULT("Pack CONNECT Parse MQTT v.3.1 MQIsdp (presumably)", Pack.has_value() && Pack->GetVariableHeader()->ProtocolName == "MQIsdp");
	}

	{
		tPacketCONNECT Pack("my_client_id", "my_will_topic", "my_will_message", "my_user_name", "my_password");
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';
		auto Pack_parsed = tPacketCONNECT::Parse(PackVector);
		utils::test::RESULT("Pack CONNECT serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketCONNECT Pack("my_client_id", "my_will_topic", "my_will_message", "my_user_name", "my_password");
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderCONNECT, tPayloadCONNECT>::Parse(PackVector);
		utils::test::RESULT("Pack CONNECT serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketCONNACK Pack(true, tConnectReturnCode::ConnectionRefused_NotAuthorized);
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderCONNACK, tPayloadCONNACK>::Parse(PackVector);
		utils::test::RESULT("Pack CONNACK serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketPUBLISH Pack(true, true, "my_topic_name/some_device", tQoS::AtLeastOnceDelivery, 10);
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderPUBLISH, tPayloadPUBLISH>::Parse(PackVector);
		utils::test::RESULT("Pack PUBLISH serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketPUBLISH Pack(true, true, "my_topic_name/some_device");
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderPUBLISH, tPayloadPUBLISH>::Parse(PackVector);
		utils::test::RESULT("Pack PUBLISH serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketPUBLISH Pack(true, true, "my_topic_name/some_device", tQoS::AtLeastOnceDelivery, 10, {0x01, 0x02});
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderPUBLISH, tPayloadPUBLISH>::Parse(PackVector);
		utils::test::RESULT("Pack PUBLISH + Payload serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketPUBLISH Pack(true, true, "my_topic_name/some_device", {0x01, 0x02});
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderPUBLISH, tPayloadPUBLISH>::Parse(PackVector);
		utils::test::RESULT("Pack PUBLISH + Payload serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	{
		tPacketPUBACK Pack(0x1234);
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderPUBACK, tPayloadPUBACK>::Parse(PackVector);
		utils::test::RESULT("Pack PUBACK serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}

	//...

	{
		tPacketDISCONNECT Pack;
		auto PackVector = Pack.ToVector();
		std::cout << utils::test::ToStringHEX(PackVector, true) << '\n';

		auto Pack_parsed = tPacket<tVariableHeaderDISCONNECT, tPayloadDISCONNECT>::Parse(PackVector);
		utils::test::RESULT("Pack DISCONNECT serialize-deserialize", Pack_parsed.has_value() && Pack_parsed == Pack);
	}
}

void UnitTest_PacketMQTT_RemainingLengthParse(const std::string& cap, const std::vector<std::uint8_t>& data, std::uint32_t packetLength)
{
	tSpan DataSpan(data);
	tRemainingLengthParseExp Length = tRemainingLength::Parse(DataSpan);
	utils::test::RESULT("RemainingLength Parse " + cap, Length.has_value() && Length == packetLength);
}

void UnitTest_PacketMQTT_RemainingLengthParseWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tError error)
{
	tSpan DataSpan(data);
	tRemainingLengthParseExp Length = tRemainingLength::Parse(DataSpan);
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