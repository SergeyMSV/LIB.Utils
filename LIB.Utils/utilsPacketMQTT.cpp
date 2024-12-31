#include "utilsPacketMQTT.h"

namespace utils
{
namespace packet_MQTT
{

std::expected<tRemainingLength, tRemainingLength::tError> tRemainingLength::Parse(const std::vector<std::uint8_t>& data)
{
	if (data.empty())
		return std::unexpected(tError::TooShort);

	tRemainingLength Length(0);
	tLengthPart Part{};
	for (int i = 0; i < data.size() && i < SizeMax; ++i)
	{
		Part.Value = data[i];

		Length.Value |= (std::size_t)Part.Field.Num << (i * 7);

		if (!Part.Field.Continuation)
			return Length;
	}

	return std::unexpected(data.size() < SizeMax ? tError::NotAll : tError::TooLong);
}

}
}
