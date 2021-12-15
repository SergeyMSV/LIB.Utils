#include "utilsPacketCameraVC0706.h"
#include "utilsPacket_Test.h"

#include <iostream>

namespace utils
{

void UnitTest_PacketCameraVC0706()
{
	std::cout << "\n""utils::packet_CameraVC0706\n";

	using namespace packet_CameraVC0706;

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 1",
		tVectorUInt8{ 0x56, 0x00, 0x11, 0x00 },
		tVectorUInt8{ 0x56, 0x00, 0x11, 0x00 });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 2",
		tVectorUInt8{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		tVectorUInt8{ 0x56, 0x00, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 3",
		tVectorUInt8{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		tVectorUInt8{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: Just a packet 4",
		tVectorUInt8{ 0x56, 0x21, 0x11,
					  0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0',
					  0x56, 0x21, 0x11 },
		tVectorUInt8{ 0x56, 0x21, 0x11, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketCmd>("tPacketCmd, Find: it can be an error, but it may not cause an exception 1 ",
		tVectorUInt8{ 0x56, 0x21, 0x11,
					  0x56, 0x21, 0x11 },
		tVectorUInt8{});

	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 1",
		tVectorUInt8{ 0x76, 0x00, 0x11, 0x00, 0x00 },
		tVectorUInt8{ 0x76, 0x00, 0x11, 0x00, 0x00 });

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 2",
		tVectorUInt8{ 0x76, 0x00, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		tVectorUInt8{ 0x76, 0x00, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	UnitTest_Packet_Find<tPacketRet>("tPacketRet, Find: Just a packet 3",
		tVectorUInt8{ 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' },
		tVectorUInt8{ 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' });

	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeSetSerialNumber",
		tVectorUInt8{ 0x56, 0x31, 0x21, 0x01, 0x32 },
		tPacketCmd::MakeSetSerialNumber(0x31, 0x32));

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: MakeSetPortUART",
		tVectorUInt8{ 0x56, 0x21, 0x24, 0x03, 0x01, 0x0D, 0xA6 },
		tPacketCmd::MakeSetPortUART(0x21, tUARTBaudrate::BR115200));

	UnitTest_Packet_Parse<tPacketCmd>("tPacketCmd Parse: MakeSetPortUARTHS",
		tVectorUInt8{ 0x56, 0x21, 0x24, 0x05, 0x02, 0x00, 0x01, 0x03, 0x53 },
		tPacketCmd::MakeSetPortUARTHS(0x21, tUARTHSBaudrate::BR921600));

	//... ReadDataReg

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg",
		tVectorUInt8{ 0x56, 0x00, 0x30, 0x04, 0x04, 0x04, 0x02, 0x40 },
		tPacketCmd::MakeReadDataReg(tMemoryDataReg::I2C_EEPROM, 0x00, {0x0240, 4}));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_Port",
		tVectorUInt8{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x07 },
		tPacketCmd::MakeReadDataReg_Port(tMemoryDataReg::I2C_EEPROM, 0x21));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_PortUART",
		tVectorUInt8{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x02, 0x00, 0x08 },
		tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_PortUARTHS",
		tVectorUInt8{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x04, 0x00, 0x0A },
		tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, 0x21));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_VideoResolution",
		tVectorUInt8{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x19 },
		tPacketCmd::MakeReadDataReg_VideoResolution(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadDataReg_VideoCompression",
		tVectorUInt8{ 0x56, 0x21, 0x30, 0x04, 0x04, 0x01, 0x00, 0x1A },
		tPacketCmd::MakeReadDataReg_VideoCompression(tMemoryDataReg::I2C_EEPROM, 0x21));
	
	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg",
		tVectorUInt8{ 0x56, 0x00, 0x31, 0x08, 0x04, 0x04, 0x02, 0x40, 0x11, 0x21, 0x22, 0x23 },
		tPacketCmd::MakeWriteDataReg(tMemoryDataReg::I2C_EEPROM, 0x00, { 0x0240, 4 }, { 0x11, 0x21, 0x22, 0x23 }));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_Port",
		tVectorUInt8{ 0x56, 0x21, 0x31, 0x05, 0x04, 0x01, 0x00, 0x07, 0x02 },
		tPacketCmd::MakeWriteDataReg_Port(tMemoryDataReg::I2C_EEPROM, 0x21, tPort::UARTHS));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_PortUART",
		tVectorUInt8{ 0x56, 0x21, 0x31, 0x06, 0x04, 0x02, 0x00, 0x08, 0x0D, 0xA6 },
		tPacketCmd::MakeWriteDataReg_PortUART(tMemoryDataReg::I2C_EEPROM, 0x21, tUARTBaudrate::BR115200));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_PortUARTHS",
		tVectorUInt8{ 0x56, 0x21, 0x31, 0x08, 0x04, 0x04, 0x00, 0x0A, 0x00, 0x01, 0x03, 0x53 },
		tPacketCmd::MakeWriteDataReg_PortUARTHS(tMemoryDataReg::I2C_EEPROM, 0x21, tUARTHSBaudrate::BR921600));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeWriteDataReg_VideoResolution",
		tVectorUInt8{ 0x56, 0x21, 0x31, 0x05, 0x04, 0x01, 0x00, 0x19, 0x22 },
		tPacketCmd::MakeWriteDataReg_VideoResolution(tMemoryDataReg::I2C_EEPROM, 0x21, tVideoResolution::VR160x120));
	
	//...

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeReadFBufCurrent",
		tVectorUInt8{ 0x56, 0x00, 0x32, 0x0C, 0x00, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00 },
		tPacketCmd::MakeReadFBufCurrent(0x00, 0x0100, 0x0200, 0x1000));//0x1000 = 40.96ms

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenCurrent",
		tVectorUInt8{ 0x56, 0x00, 0x34, 0x01, 0x00 },
		tPacketCmd::MakeGetFBufLenCurrent(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenNext",
		tVectorUInt8{ 0x56, 0x00, 0x34, 0x01, 0x01 },
		tPacketCmd::MakeGetFBufLenNext(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeGetFBufLenNext",
		tVectorUInt8{ 0x56, 0x00, 0x34, 0x01, 0x01 },
		tPacketCmd::MakeGetFBufLenNext(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeFBufCtrlStopCurrentFrame",
		tVectorUInt8{ 0x56, 0x00, 0x36, 0x01, 0x00 },
		tPacketCmd::MakeFBufCtrlStopCurrentFrame(0x00));

	UnitTest_Packet_Parse("tPacketCmd Parse: MakeFBufCtrlResumeFrame",
		tVectorUInt8{ 0x56, 0x00, 0x36, 0x01, 0x02 },
		tPacketCmd::MakeFBufCtrlResumeFrame(0x00));
	
	//////////////////////////////////////////////////////////////////////////////////////////////////

	UnitTest_Packet_Make("tPacketCmd Make MakeSetSerialNumber", tPacketCmd::MakeSetSerialNumber(0x31, 0x32));
	UnitTest_Packet_Make("tPacketCmd Make MakeReadFBufCurrent", tPacketCmd::MakeReadFBufCurrent(0x00, 0x0100, 0x0200, 0x1000));

	//////////////////////////////////////////////////////////////////////////////////////////////////

	{
		std::string Value;

		tVectorUInt8 Data = { 0x76, 0x21, 0x11, 0x00, 0x0B, 'V', 'C', '0', '7', '0', '3', ' ', '1', '.', '0', '0' };
		const size_t DataSize = Data.size();

		tPacketRet Packet;
		bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		if (Result)
			Result = tPacketRet::ParseGetVersion(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == "VC0703 1.00";

		utils::test::RESULT("tPayloadRet::ParseGetVersion", Result);
	}

	{
		tPort Value = tPort::UART;

		tVectorUInt8 Data = { 0x76, 0x21, 0x30, 0x00, 0x01, 0x02 };
		const size_t DataSize = Data.size();

		tPacketRet Packet;
		bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		if (Result)
			Result = tPacketRet::ParseReadDataReg_Port(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tPort::UARTHS;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_Port", Result);
	}

	{
		tUARTBaudrate Value = tUARTBaudrate::BR9600;

		tVectorUInt8 Data = { 0x76, 0x21, 0x30, 0x00, 0x02, 0x0D, 0xA6 };
		const size_t DataSize = Data.size();

		tPacketRet Packet;
		bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		if (Result)
			Result = tPacketRet::ParseReadDataReg_PortUART(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tUARTBaudrate::BR115200;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_PortUART", Result);
	}

	{
		tUARTHSBaudrate Value = tUARTHSBaudrate::BR921600;

		tVectorUInt8 Data = { 0x76, 0x21, 0x30, 0x00, 0x04, 0x00, 0x01, 0x03, 0x53 };
		const size_t DataSize = Data.size();

		tPacketRet Packet;
		bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		if (Result)
			Result = tPacketRet::ParseReadDataReg_PortUARTHS(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tUARTHSBaudrate::BR921600;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_PortUARTHS", Result);
	}

	{
		tVideoResolution Value = tVideoResolution::VR160x120;

		tVectorUInt8 Data = { 0x76, 0x21, 0x30, 0x00, 0x01, 0x11 };
		const size_t DataSize = Data.size();

		tPacketRet Packet;
		bool Result = tPacketRet::Find(Data, Packet) == DataSize;

		if (Result)
			Result = tPacketRet::ParseReadDataReg_VideoResolution(Packet, Value) == tMsgStatus::None;

		if (Result)
			Result = Value == tVideoResolution::VR320x240;

		utils::test::RESULT("tPayloadRet::ParseReadDataReg_VideoResolution", Result);
	}

	//Examples

	{
		tVectorUInt8 VectPacket = tPacketCmd::MakeGetVersion(2).ToVector();

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

			if (tPacketRet::ParseReadDataReg_Port(PacketRet, ValuePort) == tMsgStatus::None)
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
