#include "utilsPacketMQTT.h"

namespace utils
{
namespace packet_MQTT
{

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

}
}
