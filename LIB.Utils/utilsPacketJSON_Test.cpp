#include "utilsPacket_Test.h"
#include "utilsPacket.h"
#include "utilsPacketJSON.h"

#include <iostream>
#include <vector>

namespace utils
{

void UnitTest_PacketJSON()
{
	std::cout << "\n""utils::packet::json::tPacketJSON\n";

	using tPacketJSON = packet::tPacket<packet::json::tFormatJSON, packet::json::tPayloadString>;

	UnitTest_Packet_Find<tPacketJSON>("Parse empty packet:", "{}", "{}");
	UnitTest_Packet_Find<tPacketJSON>("Parse packet 1:", "{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}", "{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}");
	//UnitTest_Packet_Find<tPacketJSON>("Parse packet 2:", "werqtre4qt{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}werq werwrfdasdf", "{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}");
	//UnitTest_Packet_Find<tPacketJSON>("Parse packet 3:", "werqtre4qt{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}werq \"prm3_2\":\"321\"}]}", "{\"prm1\":\"123\",\"prm2\":[{\"prm2_1\":\"211\",\"prm2_2\":\"221\"},{\"prm3_1\":\"311\",\"prm3_2\":\"321\"}]}");

	std::cout << std::endl;
}

}