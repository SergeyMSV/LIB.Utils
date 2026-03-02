///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketJSON
// 2025-10-29
// C++17
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
	static std::optional<TPayload> Parse(const std::vector<std::uint8_t>& data, std::size_t& bytesToRemove)
	{
		if (data.size() < GetSize(0) || data[0] != STX)
			return {};
		const std::size_t PacketSize = DetectPacketSize(data);
		if (!PacketSize)
			return {};
		bytesToRemove = PacketSize;
		return TPayload(data.cbegin(), data.cbegin() + PacketSize);
	}

	static std::optional<TPayload> Parse(const std::vector<std::uint8_t>& data)
	{
		std::size_t BytesToRemove;
		return Parse(data, BytesToRemove);
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
