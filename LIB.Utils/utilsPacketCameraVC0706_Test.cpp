#include "utilsPacketCameraVC0706.h"
#include "utilsPacket_Test.h"

#include <iostream>

template<typename T>
bool UnitTest_Packet_ParseA(const std::vector<std::uint8_t>& data, utils::packet::vc0706::tMsgStatus& responseStatus, T& response)
{
	using namespace utils::packet::vc0706;

	std::vector<std::uint8_t> packetData = data;

	responseStatus = tMsgStatus::None;

	std::optional<tPacketRet> PacketOpt = tPacketRet::Find(packetData);
	if (!PacketOpt.has_value())
		return false;

	responseStatus = PacketOpt->GetMsgStatus();
	if (responseStatus != tMsgStatus::None)
		return false;

	tPacketRet::Parse(*PacketOpt, response);
	return true;
}

namespace utils
{

void UnitTest_PacketCameraVC0706()
{
	std::cout << "\n""utils::packet::vc0706\n";

	using namespace packet::vc0706;

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 1",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x00 },
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x00 });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 2",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 3",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 4",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x11,
					  0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0',
					  0x56, 0x21, 0x11 },
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 5 parts",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7'},
		std::vector<std::uint8_t>{  '0', '3', ' ', '1', '.', '0', '0' },
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: it can be an error, but it may not cause an exception 1 ",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x11,
					  0x56, 0x21, 0x11 },
		std::vector<std::uint8_t>{});

	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 1",
		std::vector<std::uint8_t>{ 0x76, 0x00, 0x11, 0x00, 0x00 },
		std::vector<std::uint8_t>{ 0x76, 0x00, 0x11, 0x00, 0x00 });

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 2",
		std::vector<std::uint8_t>{ 0x76, 0x00, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		std::vector<std::uint8_t>{ 0x76, 0x00, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 3",
		std::vector<std::uint8_t>{ 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		std::vector<std::uint8_t>{ 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeSetSerialNumber",
		std::vector<std::uint8_t>{ 0x56, 0x31, 0x21, 0x01, 0x32 },
		tPacketCmd::MakeSetSerialNumber(0x31, 0x32));

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: MakeSetPortUART",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x24, 0x03, 0x01, 0x0D, 0xA6 },
		tPacketCmd::MakeSetPort(0x21, tUARTBaudrate::BR115200));

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: MakeSetPortUARTHS",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x24, 0x05, 0x02, 0x03, 0x53, 0x00, 0x01 },
		tPacketCmd::MakeSetPort(0x21, tUARTHSBaudrate::BR921600));

	//... ReadDataReg

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x30, 0x04, 0x04, 0x04, 0x02, 0x40 },
		tPacketCmd::MakeReadDataReg(tMemoryDataReg::I2C_EEPROM, 0x00, {0x0240, 4}));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_Port",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x07 },
		tPacketCmd::MakeReadDataReg_Port(tMemoryDataReg::I2C_EEPROM, 0x21));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_PortUART",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x02, 0x00, 0x08 },
		tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_PortUARTHS",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x04, 0x00, 0x0A },
		tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, 0x21));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_Resolution",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x19 },
		tPacketCmd::MakeReadDataReg_Resolution(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_Compression",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x1A },
		tPacketCmd::MakeReadDataReg_Compression(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x31, 0x08, 0x04, 0x04, 0x02, 0x40, 0x11, 0x21, 0x22, 0x23 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x00, { 0x0240, 4 }, { 0x11, 0x21, 0x22, 0x23 }));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_Port",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x31, 0x05, 0x04, 0x01, 0x00, 0x07, 0x02 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x21, tPort::UARTHS));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_PortUART",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x31, 0x06, 0x04, 0x02, 0x00, 0x08, 0x0D, 0xA6 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x21, tUARTBaudrate::BR115200));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_PortUARTHS",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x31, 0x08, 0x04, 0x04, 0x00, 0x0A, 0x02, 0x98, 0x00, 0x0E },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x21, tUARTHSBaudrate::BR115200));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_PortUARTHS",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x31, 0x08, 0x04, 0x04, 0x00, 0x0A, 0x03, 0x53, 0x00, 0x01 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x21, tUARTHSBaudrate::BR921600));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_Resolution",
		std::vector<std::uint8_t>{ 0x56, 0x21, 0x31, 0x05, 0x04, 0x01, 0x00, 0x19, 0x22 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x21, tResolution::VR160x120));
	
	//...

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadFBufCurrent",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00 },
		tPacketCmd::MakeReadFBufCurrent(tPort::UART, 0x00, 0x0100, 0x0200, 0x1000));//0x1000 = 40.96ms

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenCurrent",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x34, 0x01, 0x00 },
		tPacketCmd::MakeGetFBufLenCurrent(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenNext",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x34, 0x01, 0x01 },
		tPacketCmd::MakeGetFBufLenNext(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenNext",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x34, 0x01, 0x01 },
		tPacketCmd::MakeGetFBufLenNext(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeFBufCtrlStopCurrentFrame",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x36, 0x01, 0x00 },
		tPacketCmd::MakeFBufCtrlStopCurrentFrame(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeFBufCtrlResumeFrame",
		std::vector<std::uint8_t>{ 0x56, 0x00, 0x36, 0x01, 0x02 },
		tPacketCmd::MakeFBufCtrlResumeFrame(0x00));
	
	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Make("tPacketCmd Make MakeSetSerialNumber", tPacketCmd::MakeSetSerialNumber(0x31, 0x32));
	UnitTest_Packet_Make("tPacketCmd Make MakeReadFBufCurrent", tPacketCmd::MakeReadFBufCurrent(tPort::UART, 0x00, 0x0100, 0x0200, 0x1000));

	//////////////////////////////////////////////////////////////////////////////////////////////////

	{
		tMsgStatus MsgStatus;
		std::string Value;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' }, MsgStatus, Value);

		//std::vector<std::uint8_t> Data = { 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' };
		//const size_t DataSize = Data.size();

		//tPacketRet Packet;
		//bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		//if (Result)
		//	Result = tPacketRet::Parse(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = (Value <=> "VC0703 1.00") == 0;

		utils::test::RESULT("tPayloadRet::ParseGetVersion", Result);
	}

	{
		tMsgStatus MsgStatus;
		tPort Value = tPort::UART;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x30, 0x00, 0x01, 0x02 }, MsgStatus, Value);
		
		//std::vector<std::uint8_t> Data = { 0x76, 0x21, 0x30, 0x00, 0x01, 0x02 };
		//const size_t DataSize = Data.size();

		//tPacketRet Packet;
		//bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		//if (Result)
		//	Result = tPacketRet::Parse(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tPort::UARTHS;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_Port", Result);
	}

	{
		tMsgStatus MsgStatus;
		tUARTBaudrate Value = tUARTBaudrate::BR9600;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x30, 0x00, 0x02, 0x0D, 0xA6 }, MsgStatus, Value);

		//std::vector<std::uint8_t> Data = { 0x76, 0x21, 0x30, 0x00, 0x02, 0x0D, 0xA6 };
		//const size_t DataSize = Data.size();

		//tPacketRet Packet;
		//bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		//if (Result)
		//	Result = tPacketRet::Parse(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tUARTBaudrate::BR115200;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_PortUART", Result);
	}

	{
		tMsgStatus MsgStatus;
		tUARTHSBaudrate Value = tUARTHSBaudrate::BR921600;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x30, 0x00, 0x04, 0x03, 0x53, 0x00, 0x01 }, MsgStatus, Value);
		
		//std::vector<std::uint8_t> Data = { 0x76, 0x21, 0x30, 0x00, 0x04, 0x03, 0x53, 0x00, 0x01 };
		//const size_t DataSize = Data.size();

		//tPacketRet Packet;
		//bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		//if (Result)
		//	Result = tPacketRet::Parse(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tUARTHSBaudrate::BR921600;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_PortUARTHS", Result);
	}

	{
		tMsgStatus MsgStatus;
		tResolution Value = tResolution::VR160x120;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x30, 0x00, 0x01, 0x11 }, MsgStatus, Value);

		//std::vector<std::uint8_t> Data = { 0x76, 0x21, 0x30, 0x00, 0x01, 0x11 };
		//const size_t DataSize = Data.size();

		//tPacketRet Packet;
		//bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		//if (Result)
		//	Result = tPacketRet::Parse(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tResolution::VR320x240;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_Resolution", Result);
	}

	{
		tMsgStatus MsgStatus;
		tFBufLen Value;
		bool Result = UnitTest_Packet_ParseA({ 0x76, 0x21, 0x34, 0x00, 0x04, 0x12, 0x34, 0x56, 0x78 }, MsgStatus, Value);

		if (Result)
			Result = Value.Value == 0x12345678;

		utils::test::RESULT("tPayloadRet::Parse GetFBufLen", Result);
	}

	//Examples

	{
		std::vector<std::uint8_t> VectPacket = tPacketCmd::MakeGetVersion(2).ToVector();

		//Send(VectPacket)
	}
	
	{
		tPacketRet PacketRet;

		if (PacketRet.GetMsgStatus() != tMsgStatus::None)
		{
			//Handle Error
		}

		switch (PacketRet.GetMsgId())
		{
		case tMsgId::ReadDataReg:
		{
			tPort ValuePort = tPort::UART;

			if (tPacketRet::Parse(PacketRet, ValuePort) == tMsgStatus::None)
			{
				//OK
			}
			break;
		}
		}
	}


	std::cout << std::endl;
}

}
