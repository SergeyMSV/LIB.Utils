#include "utilsPacketMQTT.h"
#include "utilsTest.h"

//#include <cassert>

//#include <algorithm>
//#include <iostream>

namespace utils
{

using namespace packet_MQTT;
void UnitTest_PacketMQTT_RemainingLength(const std::string& cap, const std::vector<std::uint8_t>& data, std::size_t packetLength);
void UnitTest_PacketMQTT_RemainingLengthWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tRemainingLength::tError error);

void UnitTest_PacketMQTT()
{
	UnitTest_PacketMQTT_RemainingLength("1 B", { 0x7F }, 127);
	UnitTest_PacketMQTT_RemainingLength("2 B", { 0xFF, 0x7F }, 16383);
	UnitTest_PacketMQTT_RemainingLength("3 B", { 0xFF, 0xFF, 0x7F }, 2097151);
	UnitTest_PacketMQTT_RemainingLength("4 B", { 0xFF, 0xFF, 0xFF, 0x7F }, 268435455);
	UnitTest_PacketMQTT_RemainingLengthWrong("Empty", { }, tRemainingLength::tError::TooShort);
	UnitTest_PacketMQTT_RemainingLengthWrong("0xFF", { 0xFF }, tRemainingLength::tError::NotAll);
	UnitTest_PacketMQTT_RemainingLengthWrong("0xFF, 0xFF", { 0xFF, 0xFF }, tRemainingLength::tError::NotAll);
	UnitTest_PacketMQTT_RemainingLengthWrong("0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF }, tRemainingLength::tError::NotAll);
	UnitTest_PacketMQTT_RemainingLengthWrong("0xFF, 0xFF, 0xFF, 0xFF", { 0xFF, 0xFF, 0xFF, 0xFF }, tRemainingLength::tError::TooLong);
	UnitTest_PacketMQTT_RemainingLengthWrong("0xFF, 0xFF, 0xFF, 0xFF, 0x7F", { 0xFF, 0xFF, 0xFF, 0xFF, 0x7F }, tRemainingLength::tError::TooLong);

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

void UnitTest_PacketMQTT_RemainingLength(const std::string& cap, const std::vector<std::uint8_t>& data, std::size_t packetLength)
{
	tRemainingLengthExp Length = tRemainingLength::Parse(data);
	utils::test::RESULT("RemainingLength " + cap, Length.has_value() && Length->Value == packetLength);
}

void UnitTest_PacketMQTT_RemainingLengthWrong(const std::string& cap, const std::vector<std::uint8_t>& data, tRemainingLength::tError error)
{
	tRemainingLengthExp Length = tRemainingLength::Parse(data);
	utils::test::RESULT("RemainingLength WRONG " + cap, !Length.has_value() && Length.error() == error);
}


}