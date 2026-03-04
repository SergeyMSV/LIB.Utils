///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadP
// 2026-02-22
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAType.h"

namespace utils
{
namespace packet
{
namespace nmea
{
namespace base
{
///////////////////////////////////////////////////////////////////////////////////////////////////
template <int preHeaderSize>
struct tContentP : public type::tTypeVerified // 'P' stands for proprietary
{
	std::vector<std::string> Value;

	tContentP() = default;
	explicit tContentP(const std::vector<std::string>& val)
	{
		if (val.empty())
			SetVerified(false);
		for (std::size_t i = 0; i < val.size(); ++i)
		{
			if (!i && val[i].size() > preHeaderSize)
			{
				Value.push_back(val[i].substr(0, preHeaderSize));
				Value.push_back(val[i].substr(preHeaderSize));
				continue;
			}
			Value.push_back(val[i]);
		}
	}

	std::vector<std::string> GetPayload() const
	{
		std::vector<std::string> Data;
		for (std::size_t i = 1; i < Value.size(); ++i)
		{
			if (i == 1)
			{
				Data.push_back(Value[0] + Value[1]);
				continue;
			}
			Data.push_back(Value[i]);
		}
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
