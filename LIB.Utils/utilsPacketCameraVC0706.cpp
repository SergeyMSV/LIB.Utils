#include "utilsPacketCameraVC0706.h"

namespace utils
{
	namespace packet_CameraVC0706
	{

tPacketCmd tPacketCmd::MakeGetVersion(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::GetVersion;
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeSetSerialNumber(std::uint8_t sn, std::uint8_t value)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::SetSerialNumber;
	Cmd.Payload.push_back(value);
	return tPacketCmd(Cmd);
}

#pragma pack(push, 1)
struct tSetPortUART_BR
{
	std::uint8_t S1RELH = 0;
	std::uint8_t S1RELL = 0;

	bool operator == (const tSetPortUART_BR& value) const
	{
		return S1RELH == value.S1RELH && S1RELL == value.S1RELL;
	}
};
#pragma pack(pop)

constexpr tSetPortUART_BR SetPortUART_BR[] = {
{ 0xAE, 0xC8 },//9600
{ 0x56, 0xE4 },//19200
{ 0x2A, 0xF2 },//38400
{ 0x1C, 0x1C },//57600
{ 0x0D, 0xA6 } //115200
};

#pragma pack(push, 1)
struct tSetPortUARTHS_BR
{
	std::uint8_t S1RELHH = 0;
	std::uint8_t S1RELHL = 0;
	std::uint8_t S1RELLH = 0;
	std::uint8_t S1RELLL = 0;

	bool operator == (const tSetPortUARTHS_BR& value) const
	{
		return S1RELHH == value.S1RELHH && S1RELHL == value.S1RELHL && S1RELLH == value.S1RELLH && S1RELLL == value.S1RELLL;
	}
};
#pragma pack(pop)

constexpr tSetPortUARTHS_BR SetPortUARTHS_BR[] = {
{ 0x00, 0x2B, 0x03, 0xC8 },//38400
{ 0x00, 0x1D, 0x01, 0x30 },//57600
{ 0x00, 0x0E, 0x02, 0x98 },//115200
{ 0x00, 0x03, 0x02, 0xA6 },//460800
{ 0x00, 0x01, 0x03, 0x53 } //921600
};

tPacketCmd tPacketCmd::MakeSetPortUART(std::uint8_t sn, tUARTBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUART_BR) / sizeof(tSetPortUART_BR)));

	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::SetPort;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tPort::UART));
	const tSetPortUART_BR Port = SetPortUART_BR[static_cast<std::size_t>(baudrate)];
	Cmd.Payload.push_back(Port.S1RELH);
	Cmd.Payload.push_back(Port.S1RELL);
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeSetPortUARTHS(std::uint8_t sn, tUARTHSBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUARTHS_BR) / sizeof(tSetPortUARTHS_BR)));

	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::SetPort;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tPort::UARTHS));
	const tSetPortUARTHS_BR Port = SetPortUARTHS_BR[static_cast<std::size_t>(baudrate)];
	Cmd.Payload.push_back(Port.S1RELHH);
	Cmd.Payload.push_back(Port.S1RELHL);
	Cmd.Payload.push_back(Port.S1RELLH);
	Cmd.Payload.push_back(Port.S1RELLL);
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeSystemReset(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::SystemReset;
	return tPacketCmd(Cmd);
}

constexpr tDataReg DataReg_Port{ 0x0007, 1 };
constexpr tDataReg DataReg_PortUART_BR{ 0x0008, 2 };
constexpr tDataReg DataReg_PortUARTHS_BR{ 0x000A, 4 };
constexpr tDataReg DataReg_VideoResolution{ 0x0019, 1 };
constexpr tDataReg DataReg_VideoCompression{ 0x001A, 1 };

tPacketCmd tPacketCmd::MakeReadDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::ReadDataReg;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(memory));
	Cmd.Payload.push_back(reg.Size);
	Cmd.Payload.push_back(static_cast<std::uint8_t>(reg.Address >> 8));
	Cmd.Payload.push_back(static_cast<std::uint8_t>(reg.Address));
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeReadDataReg_Port(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_Port);
}

tPacketCmd tPacketCmd::MakeReadDataReg_PortUART(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_PortUART_BR);
}

tPacketCmd tPacketCmd::MakeReadDataReg_PortUARTHS(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_PortUARTHS_BR);
}

tPacketCmd tPacketCmd::MakeReadDataReg_VideoResolution(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_VideoResolution);
}

tPacketCmd tPacketCmd::MakeReadDataReg_VideoCompression(tMemoryDataReg memory, std::uint8_t sn)
{
	return MakeReadDataReg(memory, sn, DataReg_VideoCompression);
}

tPacketCmd tPacketCmd::MakeWriteDataReg(tMemoryDataReg memory, std::uint8_t sn, tDataReg reg, const tVectorUInt8& data)
{
	assert(reg.Size == data.size());

	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::WriteDataReg;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(memory));
	Cmd.Payload.push_back(reg.Size);
	Cmd.Payload.push_back(static_cast<std::uint8_t>(reg.Address >> 8));
	Cmd.Payload.push_back(static_cast<std::uint8_t>(reg.Address));
	Cmd.Payload.insert(Cmd.Payload.end(), data.cbegin(), data.cend());
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeWriteDataReg_Port(tMemoryDataReg memory, std::uint8_t sn, tPort port)
{
	return MakeWriteDataReg(memory, sn, DataReg_Port, { static_cast<std::uint8_t>(port) });
}

tPacketCmd tPacketCmd::MakeWriteDataReg_PortUART(tMemoryDataReg memory, std::uint8_t sn, tUARTBaudrate baudrate)
{
	assert(static_cast<std::size_t>(baudrate) < (sizeof(SetPortUART_BR) / sizeof(tSetPortUART_BR)));

	return MakeWriteDataReg(memory, sn, DataReg_PortUART_BR, utils::ToVector(SetPortUART_BR[static_cast<std::size_t>(baudrate)]));
}

tPacketCmd tPacketCmd::MakeWriteDataReg_PortUARTHS(tMemoryDataReg memory, std::uint8_t sn, tUARTHSBaudrate baudrate)
{
	return MakeWriteDataReg(memory, sn, DataReg_PortUARTHS_BR, utils::ToVector(SetPortUARTHS_BR[static_cast<std::size_t>(baudrate)]));
}

tPacketCmd tPacketCmd::MakeWriteDataReg_VideoResolution(tMemoryDataReg memory, std::uint8_t sn, tVideoResolution resolution)
{
	return MakeWriteDataReg(memory, sn, DataReg_VideoResolution, {static_cast<std::uint8_t>(resolution)});
}

enum class tFBufType : std::uint8_t
{
	Current,
	Next,
};

enum class tFBufTransferMode : std::uint8_t
{
	MCU,
	DMA,
};

#pragma pack(push, 1)
union tFBufControlModeRead
{
	struct
	{
		std::uint8_t TRANSFER_MODE : 1;//tFBufTransferMode
		std::uint8_t NONAME_1 : 2;
		std::uint8_t NONAME_2 : 1;
		std::uint8_t : 4;
	}Field;

	std::uint8_t Value;
};
#pragma pack(pop)

tPacketCmd tPacketCmd::MakeReadFBufCurrent(std::uint8_t sn, std::uint32_t address, std::uint32_t size, std::uint16_t delay)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::ReadFBuf;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Current));

	tFBufControlModeRead ControlModeRead;
	ControlModeRead.Value = 0;
	ControlModeRead.Field.TRANSFER_MODE = static_cast<std::uint8_t>(tFBufTransferMode::DMA);
	ControlModeRead.Field.NONAME_1 = 3;
	ControlModeRead.Field.NONAME_2 = 1;
	Cmd.Payload.push_back(ControlModeRead.Value);

	tVectorUInt8 LocalVec = utils::ToVector(address);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Payload.insert(Cmd.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	LocalVec = utils::ToVector(size);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Payload.insert(Cmd.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	LocalVec = utils::ToVector(delay);
	std::reverse(LocalVec.begin(), LocalVec.end());
	Cmd.Payload.insert(Cmd.Payload.end(), LocalVec.cbegin(), LocalVec.cend());

	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeGetFBufLenCurrent(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::GetFBufLength;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Current));
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeGetFBufLenNext(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::GetFBufLength;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tFBufType::Next));
	return tPacketCmd(Cmd);
}

enum class tFBufCtrlFrame : std::uint8_t
{
	StopCurrent,
	StopNext,
	Resume,
	Step,
};

tPacketCmd tPacketCmd::MakeFBufCtrlStopCurrentFrame(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::FBufCtrl;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tFBufCtrlFrame::StopCurrent));
	return tPacketCmd(Cmd);
}

tPacketCmd tPacketCmd::MakeFBufCtrlResumeFrame(std::uint8_t sn)
{
	tPayloadCmd::value_type Cmd;
	Cmd.SerialNumber = sn;
	Cmd.MsgId = tMsgId::FBufCtrl;
	Cmd.Payload.push_back(static_cast<std::uint8_t>(tFBufCtrlFrame::Resume));
	return tPacketCmd(Cmd);
}


tMsgId tPacketRet::GetMsgId() const
{
	return GetPayloadValue().MsgId;
}

tMsgStatus tPacketRet::GetMsgStatus() const
{
	return GetPayloadValue().MsgStatus;
}

tMsgStatus tPacketRet::ParseGetVersion(const tPacketRet& packet, std::string& version)
{
	const tPacketRet::payload_value_type& PayloadValue = packet.GetPayloadValue();

	tMsgStatus Status = Check(PayloadValue, tMsgId::GetVersion);
	if (Status != tMsgStatus::None)
		return Status;

	version = std::string(PayloadValue.Payload.cbegin(), PayloadValue.Payload.cend());

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::ParseReadDataReg_Port(const tPacketRet& packet, tPort& port)
{
	const tPacketRet::payload_value_type& PayloadValue = packet.GetPayloadValue();

	tMsgStatus Status = Check(PayloadValue, tMsgId::ReadDataReg, 1);
	if (Status != tMsgStatus::None)
		return Status;

	port = static_cast<tPort>(packet.GetPayloadValue().Payload[0]);

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::ParseReadDataReg_PortUART(const tPacketRet& packet, tUARTBaudrate& baudrate)
{
	const tPacketRet::payload_value_type& PayloadValue = packet.GetPayloadValue();

	tMsgStatus Status = Check(PayloadValue, tMsgId::ReadDataReg, 2);
	if (Status != tMsgStatus::None)
		return Status;

	tSetPortUART_BR Data;
	Data.S1RELH = PayloadValue.Payload[0];
	Data.S1RELL = PayloadValue.Payload[1];

	int BrIndex = 0;
	for (auto& i : SetPortUART_BR)
	{
		if (i == Data)
			break;

		++BrIndex;
	}

	baudrate = static_cast<tUARTBaudrate>(BrIndex);

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::ParseReadDataReg_PortUARTHS(const tPacketRet& packet, tUARTHSBaudrate& baudrate)
{
	const tPacketRet::payload_value_type& PayloadValue = packet.GetPayloadValue();

	tMsgStatus Status = Check(PayloadValue, tMsgId::ReadDataReg, 4);
	if (Status != tMsgStatus::None)
		return Status;

	tSetPortUARTHS_BR Data;
	Data.S1RELHH = PayloadValue.Payload[0];
	Data.S1RELHL = PayloadValue.Payload[1];
	Data.S1RELLH = PayloadValue.Payload[2];
	Data.S1RELLL = PayloadValue.Payload[3];

	int BrIndex = 0;
	for (auto& i : SetPortUARTHS_BR)
	{
		if (i == Data)
			break;

		++BrIndex;
	}

	baudrate = static_cast<tUARTHSBaudrate>(BrIndex);

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::ParseReadDataReg_VideoResolution(const tPacketRet& packet, tVideoResolution& resolution)
{
	const tPacketRet::payload_value_type& PayloadValue = packet.GetPayloadValue();

	tMsgStatus Status = Check(PayloadValue, tMsgId::ReadDataReg, 1);
	if (Status != tMsgStatus::None)
		return Status;

	resolution = static_cast<tVideoResolution>(PayloadValue.Payload[0]);

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::Check(const tPacketRet::payload_value_type& payloadValue, tMsgId msgId)
{
	if (payloadValue.MsgId != msgId)
		return tMsgStatus::WrongPacket;

	if (payloadValue.MsgStatus != tMsgStatus::None)
		return payloadValue.MsgStatus;

	if (payloadValue.Payload.size() > ContainerPayloadSizeMax)
		return tMsgStatus::WrongDataSize;

	return tMsgStatus::None;
}

tMsgStatus tPacketRet::Check(const tPacketRet::payload_value_type& payloadValue, tMsgId msgId, std::size_t dataSize)
{
	const tMsgStatus Status = Check(payloadValue, msgId);
	if (Status != tMsgStatus::None)
		return Status;

	if (payloadValue.Payload.size() != dataSize)
		return tMsgStatus::WrongDataSize;

	return tMsgStatus::None;
}

	}
}