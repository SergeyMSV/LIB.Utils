#include "utilsPacket_Test.h"
#include "utilsPacketTWR.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketTWR()
{
	std::cout << "\n""utils::packet::twr::tPacketTWR\n";


	using tPacketCmd = packet::twr::tPacketCmd;

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse 1",
		std::vector<std::uint8_t>{ 0x2a, 0x08, 0x00, 0x03, 0x00, 0x01, 0x31, 0x32, 0x33, 0x34, 0x35, 0xcb, 0x72 },
		tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x32,0x33,0x34,0x35 }));

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse 2 rubbish",
		std::vector<std::uint8_t>{ 0x2a, 0x10, 0x00, 0x43,
		0x2a, 0x08, 0x00, 0x03, 0x00, 0x01, 0x31, 0x32, 0x33, 0x34, 0x35, 0xcb, 0x72,
		0x2a, 0x10, 0x00, 0x43, 0x1a },
		tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x32,0x33,0x34,0x35 }));

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd Find 1 parts with rubbish",
		std::vector<std::uint8_t>{ 0x2a, 0x10, 0x00, 0x43,
		0x2a, 0x08, 0x00, 0x03, 0x00, 0x01 }, std::vector<std::uint8_t>{ 0x31, 0x32, 0x33, 0x34, 0x35, 0xcb, 0x72,
		0x2a, 0x10, 0x00, 0x43, 0x1a },
		tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x32,0x33,0x34,0x35 }));

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd Find 2 parts with enclosed packet", // 2a 0d 00 03 00 01 31 2a 03 00 01 00 00 ee c8 35 70 0e
		std::vector<std::uint8_t>{ 0x2a, 0x0d, 0x00, 0x03, 0x00, 0x01, 0x31, 0x2a, 0x03, 0x00, 0x01, 0x00, 0x00, 0xee, 0xc8, 0x35},
		std::vector<std::uint8_t>{ 0x70, 0x0e},
		//tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x2a, 0x03, 0x00, 0x01, 0x00, 0x00, 0xee, 0xc8, 0x35 }));
		{ 0x2a, 0x03, 0x00, 0x01, 0x00, 0x00, 0xee, 0xc8 }); // Enclosed packet shall be received in this situation.

	{
		tPacketCmd Pack = tPacketCmd::Make_Restart();
		UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: Make_Restart", Pack.ToVector(), Pack);
	}

	{
		tPacketCmd Pack = tPacketCmd::Make_SPI_Request(packet::twr::tEndpoint::SPI0_CS0, { 0x31,0x32,0x33,0x34 });
		UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: Make_SPI_Request", Pack.ToVector(), Pack);
	}

	using tPacketRsp = packet::twr::tPacketRsp;

	UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse 1",
		std::vector<std::uint8_t>{ 0x2a, 0x10, 0x00, 0x43, 0xfe, 0x70, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x1b, 0x1a },
		tPacketRsp::Make_ERR(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::Message, std::string("Error message")));

	UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse 2 rubbish",
		std::vector<std::uint8_t>{ 0x2a, 0x10, 0x00, 0x43, 
		0x2a, 0x10, 0x00, 0x43, 0xfe, 0x70, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x1b, 0x1a,
		0x2a, 0x10, 0x00, 0x43, 0x1a },
		tPacketRsp::Make_ERR(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::Message, std::string("Error message")));

	UnitTest_Packet_Find<tPacketRsp>("tPacketRsp Find 1 parts",
		std::vector<std::uint8_t>{ 0x2a, 0x10, 0x00, 0x43,
		0x2a, 0x10, 0x00, 0x43, 0xfe, 0x70, 0x45, 0x72, 0x72}, std::vector<std::uint8_t>{ 0x6f, 0x72, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x1b, 0x1a,
		0x2a, 0x10, 0x00, 0x43, 0x1a },
		tPacketRsp::Make_ERR(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::Message, std::string("Error message")));

	{
		tPacketRsp Pack = tPacketRsp::Make_ERR(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), packet::twr::tMsgStatus::NotSupported);
		UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse: Make_ERR", Pack.ToVector(), Pack);
	}

	{
		tPacketRsp Pack = tPacketRsp::Make(tPacketCmd::Make_SPI_GetSettings(packet::twr::tEndpoint::SPI0_CS0), std::vector<std::uint8_t>{ 0x31, 0x32, 0x33, 0x34 });
		UnitTest_Packet_Parse<tPacketRsp>("tPacketRsp Parse: Make_SPI_GetSettings", Pack.ToVector(), Pack);
	}

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
	dispPack(tPacketCmd::Make_DEMO_Request(packet::twr::tEndpoint::DEMO, { 0x31,0x2a, 0x03, 0x00, 0x01, 0x00, 0x00, 0xee, 0xc8, 0x35 }).ToVector()); // Enclosed packet.
	dispPack(tPacketCmd::Make_GPIO_GetState(packet::twr::tEndpoint::GPIO_03).ToVector());
	//dispPack(tPacketRsp::Make_GPIO_GetState(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::GPIO_03, true).ToVector());
	dispPack(tPacketCmd::Make_GPIO_SetState(packet::twr::tEndpoint::GPIO_03, true).ToVector());
	//dispPack(tPacketRsp::Make_GPIO_SetState(packet::twr::tMsgStatus::NotSupported, packet::twr::tEndpoint::GPIO_03).ToVector());
	dispPack(tPacketCmd::Make_SPI_Open(packet::twr::tEndpoint::SPI0_CS0).ToVector());
	dispPack(tPacketCmd::Make_SPI_Close(packet::twr::tEndpoint::SPI0_CS0).ToVector());
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