#include "utilsPacketMQTT.h"

namespace utils
{
namespace packet_MQTT
{

std::expected<std::string, tError> tString::Parse(const std::vector<std::uint8_t>& data, std::size_t& offset)
{
	if (data.size() - offset < 2) // 2 = size of the string length
		return std::unexpected(tError::PayloadTooShort);

	tUInt16 ClientIdLength;
	ClientIdLength.Field.MSB = data[offset++];
	ClientIdLength.Field.LSB = data[offset++];

	if (data.size() - offset < ClientIdLength.Value)
		return std::unexpected(tError::PayloadTooShort);

	auto DataBegin = data.begin() + offset;
	auto DataEnd = DataBegin + ClientIdLength.Value;

	offset += ClientIdLength.Value;

	return std::string(DataBegin, DataEnd);
}

std::vector<std::uint8_t> tString::ToVector() const
{
	std::vector<std::uint8_t> Data;
	tUInt16 StrSize = static_cast<std::uint8_t>(size());
	Data.push_back(StrSize.Field.MSB);
	Data.push_back(StrSize.Field.LSB);
	Data.insert(Data.end(), begin(), end());
	return Data;
}

tRemainingLengthParseExp tRemainingLength::Parse(const std::vector<std::uint8_t>& data, std::size_t& offset)
{
	if (offset >= data.size())
		return std::unexpected(tError::LengthTooShort);

	auto DataBegin = data.begin() + offset;

	std::uint32_t Length = 0;
	tLengthPart Part{};
	for (std::size_t i = 0; DataBegin != data.end() && i < m_SizeMax; ++i)
	{
		Part.Value = *DataBegin++;

		Length |= (std::size_t)Part.Field.Num << (i * 7);

		if (!Part.Field.Continuation)
		{
			offset += i + 1;
			return Length;
		}
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

std::expected<tVariableHeaderCONNECT, tError> tVariableHeaderCONNECT::Parse(const std::vector<std::uint8_t>& data, std::size_t& offset)
{
	constexpr std::size_t ProtocolNameLengthSize = 2; // sizeof(std::uint16_t)
	const std::size_t DataSize = data.size() - offset;
	if (DataSize < ProtocolNameLengthSize)
		return std::unexpected(tError::VariableHeaderTooShort);

	tUInt16 ProtocolNameLength;
	ProtocolNameLength.Field.MSB = data[offset++];
	ProtocolNameLength.Field.LSB = data[offset++];

	constexpr std::size_t RestSize = ProtocolNameLengthSize + sizeof(ProtocolLevel) + sizeof(ConnectFlags) + sizeof(KeepAlive);
	const std::size_t VHeaderSize = ProtocolNameLength.Value + RestSize;
	if (DataSize < VHeaderSize)
		return std::unexpected(tError::VariableHeaderTooShort);

	tVariableHeaderCONNECT VHeader{};

	auto DataBegin = data.begin() + offset;
	auto DataEnd = DataBegin + ProtocolNameLength.Value;
	VHeader.ProtocolName = std::string(DataBegin, DataEnd);

	DataBegin = DataEnd;
	VHeader.ProtocolLevel = *DataBegin;

	VHeader.ConnectFlags.Value = *(++DataBegin);
	VHeader.KeepAlive.Field.MSB = *(++DataBegin);
	VHeader.KeepAlive.Field.LSB = *(++DataBegin);

	offset += VHeaderSize - ProtocolNameLengthSize;

	return VHeader;
}

std::vector<std::uint8_t> tVariableHeaderCONNECT::ToVector() const
{
	std::vector<std::uint8_t> Data = ProtocolName.ToVector();
	Data.push_back(ProtocolLevel);
	Data.push_back(ConnectFlags.Value);
	Data.push_back(KeepAlive.Field.MSB);
	Data.push_back(KeepAlive.Field.LSB);
	return Data;
}

bool tVariableHeaderCONNECT::operator==(const tVariableHeaderCONNECT& val) const
{
	return ProtocolName == val.ProtocolName && ProtocolLevel == val.ProtocolLevel && ConnectFlags.Value == val.ConnectFlags.Value && KeepAlive.Value == val.KeepAlive.Value;
}

std::expected<tPayloadCONNECT, tError> tPayloadCONNECT::Parse(const tVariableHeaderCONNECT& variableHeader, const std::vector<std::uint8_t>& data, std::size_t& offset)
{
	tPayloadCONNECT Payload{};
	// These fields, if present, MUST appear in the order Client Identifier, Will Topic, Will Message, User Name, Password
	auto StrExp = tString::Parse(data, offset);
	if (!StrExp.has_value())
		return std::unexpected(StrExp.error());
	Payload.ClientId = StrExp.value();

	if (variableHeader.ConnectFlags.Field.WillFlag)
	{
		StrExp = tString::Parse(data, offset);
		if (!StrExp.has_value())
			return std::unexpected(StrExp.error());
		Payload.WillTopic = StrExp.value();

		StrExp = tString::Parse(data, offset);
		if (!StrExp.has_value())
			return std::unexpected(StrExp.error());
		Payload.WillMessage = StrExp.value();
	}

	if (variableHeader.ConnectFlags.Field.UserNameFlag)
	{
		StrExp = tString::Parse(data, offset);
		if (!StrExp.has_value())
			return std::unexpected(StrExp.error());
		Payload.UserName = StrExp.value();

		StrExp = tString::Parse(data, offset);
		if (!StrExp.has_value())
			return std::unexpected(StrExp.error());
		Payload.Password = StrExp.value();
	}

	return Payload;
}

std::vector<std::uint8_t> tPayloadCONNECT::ToVector() const
{
	// These fields, if present, MUST appear in the order Client Identifier, Will Topic, Will Message, User Name, Password
	std::vector<std::uint8_t> Data = ClientId.ToVector(); // The Server MUST allow ClientIds which are between 1 and 23 UTF - 8 encoded bytes in length, and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".
	if (WillTopic.has_value())
		Data.append_range(WillTopic->ToVector());
	if (WillMessage.has_value())
		Data.append_range(WillMessage->ToVector());
	if (UserName.has_value())
		Data.append_range(UserName->ToVector());
	if (Password.has_value())
		Data.append_range(Password->ToVector());
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


std::expected<tVariableHeaderCONNACK, tError> tVariableHeaderCONNACK::Parse(const std::vector<std::uint8_t>& data, std::size_t& offset)
{
	const std::size_t DataSize = data.size() - offset;
	if (DataSize < GetSize())
		return std::unexpected(tError::VariableHeaderTooShort);

	tVariableHeaderCONNACK VHeader{};
	VHeader.ConnectAcknowledgeFlags.Value = data[offset++];
	VHeader.ConnectReturnCode = static_cast<tConnectReturnCode>(data[offset++]);

	offset += GetSize();

	return VHeader;
}

static std::size_t GetSize() { return 2; }

std::vector<std::uint8_t> tVariableHeaderCONNACK::ToVector() const
{
	std::vector<std::uint8_t> Data;
	Data.push_back(ConnectAcknowledgeFlags.Value);
	Data.push_back(static_cast<std::uint8_t>(ConnectReturnCode));
	return Data;
}

bool tVariableHeaderCONNACK::operator==(const tVariableHeaderCONNACK& val) const
{
	return ConnectAcknowledgeFlags.Value == val.ConnectAcknowledgeFlags.Value && ConnectReturnCode == val.ConnectReturnCode;
}

}
}
