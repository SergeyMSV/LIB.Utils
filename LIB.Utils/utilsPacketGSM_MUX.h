///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketGSM_MUX.h
// 2012-03-28 - 2019-05-11 - 2026-11-02
// C++20
// 
// Supported modules: Cinterion BGS2R1, BGS2R2, EHS5R2
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <utilsCRC.h>

#include <vector>

namespace utils
{
namespace packet
{
namespace gsm_cinterion_bgs2r1_mux
{

const int ContainerSize = 6;
const int ContainerFrameSizeMax = 98;

union tPacketFieldAddress
{
	struct
	{
		unsigned char EA   : 1;//Extension bit, Always = 1
		unsigned char CR   : 1;//Command/Response
		unsigned char DLCI : 6;//Data Link Connector Identifier
	}Field;

	unsigned char Value;
};

union tPacketFieldControl//FrameType
{
	struct
	{
		unsigned char      : 4;
		unsigned char PF   : 1;//Poll/Final bit (For each DLCI, only one frame with P = 1 may ever be expected)
		unsigned char      : 3;
	}Field;

	unsigned char FrameType;

	unsigned char Value;
};

enum tPacketFieldControlFrameType
{
	tPacketFieldControlFrameType_SABM = 0x2F,
	tPacketFieldControlFrameType_UA = 0x63,
	tPacketFieldControlFrameType_DM = 0x0F,
	tPacketFieldControlFrameType_DISC = 0x43,
	tPacketFieldControlFrameType_UIH = 0xEF,
	//tPacketFieldControlFrameType_UI = 0xEF,
};

union tPacketFieldLength//Also length type - Length can be either 7 or 15 bit
{
	struct
	{
		unsigned char EA     : 1;//Always = ? (2nd octet (not supported by Cinterion wireless modules))
		unsigned char Length : 7;//Lenghth or part of length...
	}Field;

	unsigned char Value;
};

std::vector<char> FindPacket(std::vector<char>& receivedData);

class tPacket
{
	tPacketFieldAddress m_Address;
	tPacketFieldControl m_Control;
	std::vector<char> m_Payload;

public:
	tPacket();
	tPacket(tPacketFieldControlFrameType frameType, bool command, unsigned char dlci);
	tPacket(tPacketFieldControlFrameType frameType, bool command, unsigned char dlci, std::vector<char>& payload);

	static bool TryParse(const std::vector<char>& packetRaw, tPacket& packet);

	tPacketFieldAddress GetAddress() const { return m_Address; }
	tPacketFieldControl GetControl() const { return m_Control; }
	std::vector<char> GetPayload() const { return m_Payload; }

	std::vector<char> ToVector();
};

//The commands are sent as information in the multiplexer control channel frame.

union tMuxControlChannel
{
	struct
	{
		unsigned char EA   : 1;//Always = 1
		unsigned char CR   : 1;//Command/Response
		unsigned char Type : 6;
	}Field;

	unsigned char Value;
};

enum tMuxControlChannelType
{
	tMuxControlChannelType_CLD = 0x30,
	tMuxControlChannelType_TST = 0x08,
	tMuxControlChannelType_MSC = 0x38,
};

union tV24ControlSignals
{
	// Direction host application -> module (for request only) MUX V3:
	// RTC: mapped to DTR
	// RTR: mapped to RTS
	// Bit 5, 6, 7, 8 are not valid.

	// Direction module -> host application (for request only) MUX V3:
	// RTC: mapped to DSR
	// RTR: mapped to CTS
	// RING: mapped to RING (IC)
	// DCD: mapped to DCD (DV)
	// Bit 5, 6 are not valid

	struct
	{
		unsigned char EA   : 1;//Always = 1
		unsigned char FC   : 1;//The bit is set to 1(one) when the device is unable to accept frames.
		unsigned char RTC  : 1;//The bit is set to 1 when the device is ready to communicate.
		unsigned char RTR  : 1;//The bit is set to 1 when the device is ready to receive data.
		unsigned char      : 2;
		unsigned char RING : 1;
		unsigned char DCD  : 1;
	}Field;

	unsigned char Value;
};

union tMuxMSC//Modem Status Command
{
	struct tField
	{
		tMuxControlChannel Command;
		tPacketFieldLength Length;
		tPacketFieldAddress DLCI;
		tV24ControlSignals V24Signals;
	}Field;

	unsigned char Value[4];

	tMuxMSC()
	{
		Field.Command.Value = 0;
		Field.Command.Field.EA = 1;
		Field.Command.Field.CR = 1;
		Field.Command.Field.Type = tMuxControlChannelType_MSC;

		Field.Length.Field.EA = 1;
		Field.Length.Field.Length = 3;

		Field.DLCI.Value = 0;
		Field.DLCI.Field.EA = 1;
		Field.DLCI.Field.CR = 1;

		Field.V24Signals.Value = 0;
	}
};

union tMuxMSC_Break//Modem Status Command
{
	struct tField
	{
		tMuxControlChannel Command;
		tPacketFieldLength Length;
		tPacketFieldAddress DLCI;
		tV24ControlSignals V24Signals;
		unsigned char Break;

		//tField()
		//{
		//	Command.Value = 0;
		//	Command.Field.EA = 1;

		//	Length.Value = 0;

		//	DLCI.Value = 0;
		//	DLCI.Field.EA = 1;
		//	DLCI.Field.CR = 1;

		//	V24Signals.Value = 0;
		//}

	}Field;

	unsigned char Value[5];

	tMuxMSC_Break()
	{
		Field.Command.Value = 0;
		Field.Command.Field.EA = 1;
		Field.Command.Field.CR = 1;
		Field.Command.Field.Type = tMuxControlChannelType_MSC;

		Field.Length.Field.EA = 1;
		Field.Length.Field.Length = 3;

		Field.DLCI.Value = 0;
		Field.DLCI.Field.EA = 1;
		Field.DLCI.Field.CR = 1;

		Field.V24Signals.Value = 0;

		Field.Break = 0;
	}
};

}
}
}
