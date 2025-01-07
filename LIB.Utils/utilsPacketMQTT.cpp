#include "utilsPacketMQTT.h"

namespace utils
{
namespace packet_MQTT
{

std::vector<std::uint8_t> MQTTStringToVector(const std::string& value)
{
	std::vector<std::uint8_t> Data;
	tUInt16 StrSize = static_cast<std::uint8_t>(value.size());
	Data.push_back(StrSize.Field.MSB);
	Data.push_back(StrSize.Field.LSB);
	Data.insert(Data.end(), value.begin(), value.end());
	return Data;
}

tRemainingLengthParseExp tRemainingLength::Parse(const std::vector<std::uint8_t>& data)
{
	if (data.empty())
		return std::unexpected(tError::LengthTooShort);

	std::uint32_t Length = 0;
	tLengthPart Part{};
	for (int i = 0; i < data.size() && i < m_SizeMax; ++i)
	{
		Part.Value = data[i];

		Length |= (std::size_t)Part.Field.Num << (i * 7);

		if (!Part.Field.Continuation)
			return Length;
	}

	return std::unexpected(data.size() < m_SizeMax ? tError::LengthNotAll : tError::LengthTooLong);
}

tRemainingLengthToVectorExp tRemainingLength::ToVector(std::uint32_t value)
{
	std::vector<std::uint8_t> Data;
	for (int i = 0; i < m_SizeMax; ++i)
	{
		tLengthPart Part{};
		Part.Field.Num = value;

		value = value >> 7;
		if (value)
			Part.Field.Continuation = 1;

		Data.push_back(Part.Value);

		if (!value)
			break;
	}

	if (value)
		return std::unexpected(tError::LengthOverflow);

	return Data;
}

std::expected<tVariableHeaderCONNECT, tError> tVariableHeaderCONNECT::Parse(const std::vector<std::uint8_t>& data)
{
	constexpr std::size_t ProtocolNameLengthSize = 2; // sizeof(std::uint16_t)
	if (data.size() < ProtocolNameLengthSize)
		return std::unexpected(tError::Format);

	tUInt16 ProtocolNameLength;
	ProtocolNameLength.Field.MSB = data[0];
	ProtocolNameLength.Field.LSB = data[1];

	constexpr std::size_t RestSize = ProtocolNameLengthSize + sizeof(ProtocolLevel) + sizeof(ConnectFlags) + sizeof(KeepAlive);
	const std::size_t VHeaderSize = ProtocolNameLength.Value + RestSize;
	if (data.size() != VHeaderSize)
		return std::unexpected(tError::Format);

	tVariableHeaderCONNECT VHeader{};

	auto DataBegin = data.begin() + ProtocolNameLengthSize;
	auto DataEnd = DataBegin + ProtocolNameLength.Value;
	VHeader.ProtocolName = std::string(DataBegin, DataEnd);

	DataBegin = DataEnd;
	VHeader.ProtocolLevel = *DataBegin;

	VHeader.ConnectFlags.Value = *(++DataBegin);
	VHeader.KeepAlive.Field.MSB = *(++DataBegin);
	VHeader.KeepAlive.Field.LSB = *(++DataBegin);
	return VHeader;
}

std::vector<std::uint8_t> tVariableHeaderCONNECT::ToVector() const
{
	std::vector<std::uint8_t> Data = MQTTStringToVector(ProtocolName);
	Data.push_back(ProtocolLevel);
	Data.push_back(ConnectFlags.Value);
	Data.push_back(KeepAlive.Field.MSB);
	Data.push_back(KeepAlive.Field.LSB);
	return Data;
}

std::expected<tPayloadCONNECT, tError> tPayloadCONNECT::Parse(const std::vector<std::uint8_t>& data)
{
	//[TBD] it's needed to be aware of strings types
	return std::unexpected(tError::Format);
}

std::vector<std::uint8_t> tPayloadCONNECT::ToVector() const
{
	// These fields, if present, MUST appear in the order Client Identifier, Will Topic, Will Message, User Name, Password
	std::vector<std::uint8_t> Data = MQTTStringToVector(ClientId); // The Server MUST allow ClientIds which are between 1 and 23 UTF - 8 encoded bytes in length, and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".
	if (WillTopic.has_value())
		Data.append_range(MQTTStringToVector(*WillTopic));
	if (WillMessage.has_value())
		Data.append_range(MQTTStringToVector(*WillMessage));
	if (UserName.has_value())
		Data.append_range(MQTTStringToVector(*UserName));
	if (Password.has_value())
		Data.append_range(MQTTStringToVector(*Password));
	return Data;
}

void tPacketCONNECT::SetClientId(const std::string& value)
{
	// The Server MAY allow ClientId’s that contain more than 23 encoded bytes.
	// The Server MAY allow ClientId’s that contain characters not included in the list given above.
	// 
	// A Server MAY allow a Client to supply a ClientId that has a length of zero bytes,
	// however if it does so the Server MUST treat this as a special case and assign a unique ClientId to that Client.
	// It MUST then process the CONNECT packet as if the Client had provided that unique ClientId [MQTT - 3.1.3 - 6].
	// 
	// If the Client supplies a zero - byte ClientId, the Client MUST also set CleanSession to 1 [MQTT - 3.1.3 - 7].
	m_Payload->ClientId = value;
}

void tPacketCONNECT::SetWill(const std::string& topic, const std::string& message)
{
	m_VariableHeader->ConnectFlags.Field.WillFlag = !topic.empty() && !message.empty();

	if (m_VariableHeader->ConnectFlags.Field.WillFlag)
	{
		m_Payload->WillTopic = topic;
		m_Payload->WillMessage = message;
	}
	else
	{
		m_VariableHeader->ConnectFlags.Field.WillQoS = 0; // If the Will Flag is set to 0, then the Will QoS MUST be set to 0 (0x00) [MQTT-3.1.2-13].*/
		m_Payload->WillTopic.reset();
		m_Payload->WillMessage.reset();
	}
}

void tPacketCONNECT::SetUser(const std::string& name, const std::string& password)
{
	m_VariableHeader->ConnectFlags.Field.UserNameFlag = !name.empty();
	m_VariableHeader->ConnectFlags.Field.PasswordFlag = !name.empty() && !password.empty(); // [TBD] verify it (write here reference to the doc.)

	if (m_VariableHeader->ConnectFlags.Field.UserNameFlag)
	{
		m_Payload->UserName = name;
	}
	else
	{
		m_Payload->UserName.reset();
	}

	if (m_VariableHeader->ConnectFlags.Field.PasswordFlag)
	{
		m_Payload->Password = password;
	}
	else
	{
		m_Payload->Password.reset();
	}
}

std::expected<tPacketCONNECT, tError> tPacketCONNECT::Parse(const std::vector<std::uint8_t>& data)
{
	if (data.empty())
		return std::unexpected(tError::PacketTooShort);

	tPacketCONNECT Pack{};
	Pack.m_FixedHeader.Value = data[0];

	auto VarHeadExp = tVariableHeaderCONNECT::Parse(data);
	if (!VarHeadExp.has_value())
		return std::unexpected(VarHeadExp.error());
	Pack.m_VariableHeader = *VarHeadExp;

	const std::size_t RemoveBytes = 1 + Pack.m_VariableHeader->GetSize();
	if (RemoveBytes > data.size())
		return std::unexpected(tError::PacketTooShort);

	std::vector<std::uint8_t> PayloadVect(data.begin() + RemoveBytes, data.end());

	auto PayloadExp = tPayloadCONNECT::Parse(PayloadVect);
	if (!PayloadExp.has_value())
		return std::unexpected(PayloadExp.error());
	Pack.m_Payload = *PayloadExp;

	return Pack;
}

}
}
