///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketJSON
// 2025-10-29
// C++14
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace utils
{
namespace packet
{
namespace json
{

template <class TPayload>
struct tFormatJSON
{
	enum : std::uint8_t { STX = '{' };

protected:
	static std::vector<std::uint8_t> TestPacket(std::vector<std::uint8_t>::const_iterator cbegin, std::vector<std::uint8_t>::const_iterator cend)
	{
		const std::size_t Size = std::distance(cbegin, cend);

		if (Size < GetSize(0) || *cbegin != STX)
			return {};

		const std::size_t PacketSize = DetectPacketSize(cbegin, cend);
		if (!PacketSize)
			return {};

		return std::vector<std::uint8_t>(cbegin, cbegin + PacketSize);
	}

	static bool TryParse(const std::vector<std::uint8_t>& packetVector, TPayload& payload)
	{
		if (packetVector.size() < GetSize(0) || packetVector[0] != STX)
			return false;

		const std::size_t PacketSize = DetectPacketSize(packetVector);
		if (!PacketSize)
			return false;

		payload = TPayload(packetVector.cbegin(), packetVector.cbegin() + PacketSize);
		return true;
	}

	static std::size_t GetSize(std::size_t payloadSize) { return payloadSize + 2; } // 2 - for {}

	void Append(std::vector<std::uint8_t>& dst, const TPayload& payload) const
	{
		dst.reserve(GetSize(payload.size()));

		for (const auto i : payload)
		{
			dst.push_back(i);
		}
	}

private:
	static std::size_t DetectPacketSize(const std::vector<std::uint8_t>& data)
	{
		return DetectPacketSize(data.cbegin(), data.cend());
	}

	static std::size_t DetectPacketSize(const std::vector<std::uint8_t>::const_iterator& begin, const std::vector<std::uint8_t>::const_iterator& end)
	{
		std::size_t PacketSize = 0;
		int BracesCounter = 0;
		for (std::vector<std::uint8_t>::const_iterator i = begin; i != end; ++i, ++PacketSize)
		{
			if (*i == '{')
			{
				++BracesCounter;
				continue;
			}

			if (*i == '}')
			{
				--BracesCounter;
				continue;
			}

			if (!BracesCounter)
				break;
		}
		return BracesCounter ? 0 : PacketSize;
	}
};

struct tPayloadString
{
	typedef std::string value_type;
	typedef value_type::const_iterator iterator;

	value_type Value{};

	tPayloadString() = default;

	explicit tPayloadString(const value_type& value)
		:Value(value)
	{
	}

	tPayloadString(std::vector<std::uint8_t>::const_iterator cbegin, std::vector<std::uint8_t>::const_iterator cend)
	{
		Value.insert(Value.end(), cbegin, cend);
	}

	std::size_t size() const
	{
		return Value.size();
	}

	iterator begin() const
	{
		return Value.begin();
	}

	iterator end() const
	{
		return Value.end();
	}
};

}
}
}
