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

}
}
