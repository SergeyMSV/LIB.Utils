#include "utilsPacket_Test.h"
#include "utilsPacketTWR.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketTWR()
{
	std::cout << "\n""utils::packet::twr::tPacketTWR\n";

	using tPacketCmd = packet::twr::tPacketCmd;

	{
		tPacketCmd Pack = tPacketCmd::Make_Restart();
		UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: Make_Restart", Pack.ToVector(), Pack);
	}

	{
		tPacketCmd Pack = tPacketCmd::Make_SPI_Request(packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
		UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: Make_SPI_Request", Pack.ToVector(), Pack);
	}

	using tPacketRsp = packet::twr::tPacketRsp;

	//{
	//	tPacketRsp Pack = tPacketRsp::Make_Restart(packet::twr::tMsgStatus::NotSupported);
	//	UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse: Make_Restart", Pack.ToVector(), Pack);
	//}

	//{
	//	tPacketRsp Pack = tPacketRsp::Make_SPI_Request(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
	//	UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse: Make_SPI_Request", Pack.ToVector(), Pack);
	//}

	auto dispPack = [](const std::vector<uint8_t>& pack)
	{
		for (auto i : pack)
			std::cout << std::right << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(i) << ' ';
		std::cout << '\n';
	};

	dispPack(tPacketCmd::Make_Restart().ToVector());
	//dispPack(tPacketRsp::Make_Restart(packet::twr::tMsgStatus::NotSupported).ToVector());
	dispPack(tPacketCmd::Make_GetVersion().ToVector());
	//dispPack(tPacketRsp::Make_GetVersion(packet::twr::tMsgStatus::NotSupported, "Version-1.02.03").ToVector());
	dispPack(tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x32,0x33,0x34,0x35 }).ToVector());
	dispPack(tPacketCmd::Make_GPIO_GetState(packet::twr::tEndpoint::GPIO_03).ToVector());
	//dispPack(tPacketRsp::Make_GPIO_GetState(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::GPIO_03, true).ToVector());
	dispPack(tPacketCmd::Make_GPIO_SetState(packet::twr::tEndpoint::GPIO_03, true).ToVector());
	//dispPack(tPacketRsp::Make_GPIO_SetState(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::GPIO_03).ToVector());
	dispPack(tPacketCmd::Make_SPI_Request(packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 }).ToVector());
	//dispPack(tPacketRsp::Make_SPI_Request(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0, { 0x41,0x42,0x43,0x44 }).ToVector());
	dispPack(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0).ToVector());
	//dispPack(tPacketRsp::Make_SPI_GetSettings(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0, { 1,2,3,4 }).ToVector());
	//dispPack(tPacketRsp::Make_SPI_GetSettings(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::GPIO_00, 0x01020304).ToVector()); // assert
	dispPack(tPacketCmd::Make_SPI_SetSettings(packet::twr::tEndpoint::SPI0_CS0, 0x01020304).ToVector());
	//dispPack(tPacketRsp::Make_SPI_SetSettings(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0).ToVector());
	packet::twr::tChipControl ChipControl;
	ChipControl.Field.Enable = 1;
	ChipControl.Field.Reset = 1;
	dispPack(tPacketCmd::Make_SPI_SetChipControl(packet::twr::tEndpoint::SPI0_CS0, ChipControl).ToVector());
	//dispPack(tPacketRsp::Make_SPI_SetChipControl(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0).ToVector());

	//dispPack(tPacketRsp::Make(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::NotSupported, packet::twr::tSPIPortSettings{ 1,2,3,4 }).ToVector());
	//dispPack(tPacketRsp::Make(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::NotSupported, std::vector<std::uint8_t>{ 0x31,0x32,0x33,0x34 }).ToVector());
	dispPack(tPacketRsp::Make(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), std::vector<std::uint8_t>{ 0x31,0x32,0x33,0x34 }).ToVector());
	dispPack(tPacketRsp::Make_ERR(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::Message, std::string("Error message")).ToVector());


	auto ShowPack = [](const packet::twr::tPacketBase& pack)
	{
		std::cout << "MsgId = " << (int)pack.GetMsgId() << "; MsgStatus = " << (int)pack.GetMsgStatus() << "; Ep = " << (int)pack.GetEndpoint() << ";\n";
		for (auto i : pack.GetPayload())
			std::cout << std::right << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(i) << ' ';
		std::cout << '\n';
	};

	{
		tPacketCmd Pack = tPacketCmd::Make_SPI_Request(packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
		std::cout << "Cmd ";
		ShowPack(Pack);
	}

	//{
	//	tPacketRsp Pack = tPacketRsp::Make_SPI_Request(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
	//	std::cout << "Rsp ";
	//	ShowPack(Pack);
	//}

	std::cout << std::endl;
}

}