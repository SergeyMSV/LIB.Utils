#include "utilsPacket_Test.h"
#include "utilsPacketTWR.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketTWR()
{
	std::cout << "\n""utils::packet::tPacketTWR\n";

	using tPacketTWRCmd = packet_TWR::tPacketTWRCmd;

	{
		tPacketTWRCmd Pack = tPacketTWRCmd::Make_Restart();
		UnitTest_Packet_Parse<tPacketTWRCmd>("tPacketCmd Parse: Make_Restart", Pack.ToVector(), Pack);
	}

	{
		tPacketTWRCmd Pack = tPacketTWRCmd::Make_SPI_Request(packet_TWR::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
		UnitTest_Packet_Parse<tPacketTWRCmd>("tPacketCmd Parse: Make_SPI_Request", Pack.ToVector(), Pack);
	}

	using tPacketTWRRsp = packet_TWR::tPacketTWRRsp;

	//{
	//	tPacketTWRRsp Pack = tPacketTWRRsp::Make_Restart(packet_TWR::tMsgStatus::NotSupported);
	//	UnitTest_Packet_Parse<tPacketTWRRsp>("tPacketRsp Parse: Make_Restart", Pack.ToVector(), Pack);
	//}

	//{
	//	tPacketTWRRsp Pack = tPacketTWRRsp::Make_SPI_Request(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
	//	UnitTest_Packet_Parse<tPacketTWRRsp>("tPacketRsp Parse: Make_SPI_Request", Pack.ToVector(), Pack);
	//}

	auto dispPack = [](const std::vector<uint8_t>& pack)
	{
		for (auto i : pack)
			std::cout << std::right << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(i) << ' ';
		std::cout << '\n';
	};

	dispPack(tPacketTWRCmd::Make_Restart().ToVector());
	//dispPack(tPacketTWRRsp::Make_Restart(packet_TWR::tMsgStatus::NotSupported).ToVector());
	dispPack(tPacketTWRCmd::Make_GetVersion().ToVector());
	//dispPack(tPacketTWRRsp::Make_GetVersion(packet_TWR::tMsgStatus::NotSupported, "Version-1.02.03").ToVector());
	dispPack(tPacketTWRCmd::Make_DEMO_Request(packet_TWR::tEndpoint::DEMO, { 0x31,0x32,0x33,0x34,0x35 }).ToVector());
	dispPack(tPacketTWRCmd::Make_GPIO_GetState(packet_TWR::tEndpoint::GPIO_03).ToVector());
	//dispPack(tPacketTWRRsp::Make_GPIO_GetState(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::GPIO_03, true).ToVector());
	dispPack(tPacketTWRCmd::Make_GPIO_SetState(packet_TWR::tEndpoint::GPIO_03, true).ToVector());
	//dispPack(tPacketTWRRsp::Make_GPIO_SetState(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::GPIO_03).ToVector());
	dispPack(tPacketTWRCmd::Make_SPI_Request(packet_TWR::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 }).ToVector());
	//dispPack(tPacketTWRRsp::Make_SPI_Request(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0, { 0x41,0x42,0x43,0x44 }).ToVector());
	dispPack(tPacketTWRCmd::Make_SPI_GetSettings(packet_TWR::tEndpoint::SPI0_CS0).ToVector());
	//dispPack(tPacketTWRRsp::Make_SPI_GetSettings(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0, { 1,2,3,4 }).ToVector());
	//dispPack(tPacketTWRRsp::Make_SPI_GetSettings(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::GPIO_00, 0x01020304).ToVector()); // assert
	dispPack(tPacketTWRCmd::Make_SPI_SetSettings(packet_TWR::tEndpoint::SPI0_CS0, 0x01020304).ToVector());
	//dispPack(tPacketTWRRsp::Make_SPI_SetSettings(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0).ToVector());
	packet_TWR::tChipControl ChipControl;
	ChipControl.Field.Enable = 1;
	ChipControl.Field.Reset = 1;
	dispPack(tPacketTWRCmd::Make_SPI_SetChipControl(packet_TWR::tEndpoint::SPI0_CS0, ChipControl).ToVector());
	//dispPack(tPacketTWRRsp::Make_SPI_SetChipControl(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0).ToVector());

	//dispPack(tPacketTWRRsp::Make(tPacketTWRCmd::Make_SPI_GetSettings(packet_TWR::tEndpoint::SPI0_CS0), packet_TWR::tMsgStatus::NotSupported, packet_TWR::tSPIPortSettings{ 1,2,3,4 }).ToVector());
	//dispPack(tPacketTWRRsp::Make(tPacketTWRCmd::Make_SPI_GetSettings(packet_TWR::tEndpoint::SPI0_CS0), packet_TWR::tMsgStatus::NotSupported, tVectorUInt8{ 0x31,0x32,0x33,0x34 }).ToVector());
	dispPack(tPacketTWRRsp::Make(tPacketTWRCmd::Make_SPI_GetSettings(packet_TWR::tEndpoint::SPI0_CS0), tVectorUInt8{ 0x31,0x32,0x33,0x34 }).ToVector());
	dispPack(tPacketTWRRsp::Make_ERR(tPacketTWRCmd::Make_SPI_GetSettings(packet_TWR::tEndpoint::SPI0_CS0), packet_TWR::tMsgStatus::Message, std::string("Error message")).ToVector());


	auto ShowPack = [](const packet_TWR::tPacketTWR& pack)
	{
		std::cout << "MsgId = " << (int)pack.GetMsgId() << "; MsgStatus = " << (int)pack.GetMsgStatus() << "; Ep = " << (int)pack.GetEndpoint() << ";\n";
		for (auto i : pack.GetPayload())
			std::cout << std::right << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(i) << ' ';
		std::cout << '\n';
	};

	{
		tPacketTWRCmd Pack = tPacketTWRCmd::Make_SPI_Request(packet_TWR::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
		std::cout << "Cmd ";
		ShowPack(Pack);
	}

	//{
	//	tPacketTWRRsp Pack = tPacketTWRRsp::Make_SPI_Request(packet_TWR::tMsgStatus::NotSupported, packet_TWR::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
	//	std::cout << "Rsp ";
	//	ShowPack(Pack);
	//}

	std::cout << std::endl;
}

}