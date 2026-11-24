///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadPUBX
// 2026-11-17
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsPacketNMEAPayload.h"

#include <vector>

namespace utils
{
namespace packet
{
namespace nmea
{
namespace ublox
{
	
constexpr char kProtocolID[] = "PUBX";

enum class tUBXMsgID : std::uint8_t // 04 Requested UBX Msg ID (Proprietary message identifier )
{
	LatLonPos,	// Lat/Lon Position Data
	UTMPos,		// UTM Position Da
	Sats = 3,	// Satellite Status
	Clock,		// Time of Day and Clock Information
	SetNMEAMessageOutputRate = 40,
	SetProtocolsAndBaudrate,
};

using tMsgID = type::tUIntFixedNoNull<2>; // 04 Requested UBX Msg ID (Proprietary message identifier )
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentPUBX_Poll : public type::tTypeVerified
{
	tMsgID MsgID;

	tContentPUBX_Poll() = default;
	explicit tContentPUBX_Poll(const std::vector<std::string>& val)
	{
		if (val.size() != 2 || val[0] != kProtocolID)
		{
			SetVerified(false);
			return;
		}
		MsgID = tMsgID(val[1]);
	}

	bool IsVerified() const { return type::tTypeVerified::IsVerified() && type::IsVerified(MsgID); }

	std::vector<std::string> GetPayload() const
	{
		if (!IsVerified())
			return {};
		std::vector<std::string> Data;
		Data.push_back(kProtocolID);
		Data.push_back(MsgID.ToString());
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentPUBX_Poll_LatLonPos : public tContentPUBX_Poll
{
	tContentPUBX_Poll_LatLonPos() = default;
	explicit tContentPUBX_Poll_LatLonPos(const std::vector<std::string>& val)
		:tContentPUBX_Poll(val)
	{
		if (IsVerified())
			SetVerified(MsgID.GetValue() == static_cast<int>(tUBXMsgID::LatLonPos));
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentPUBX_Poll_UTMPos : public tContentPUBX_Poll
{
	tContentPUBX_Poll_UTMPos() = default;
	explicit tContentPUBX_Poll_UTMPos(const std::vector<std::string>& val)
		:tContentPUBX_Poll(val)
	{
		if (IsVerified())
			SetVerified(MsgID.GetValue() == static_cast<int>(tUBXMsgID::UTMPos));
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentPUBX_Poll_Sats : public tContentPUBX_Poll
{
	tContentPUBX_Poll_Sats() = default;
	explicit tContentPUBX_Poll_Sats(const std::vector<std::string>& val)
		:tContentPUBX_Poll(val)
	{
		if (IsVerified())
			SetVerified(MsgID.GetValue() == static_cast<int>(tUBXMsgID::Sats));
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentPUBX_Poll_Clock : public tContentPUBX_Poll
{
	tContentPUBX_Poll_Clock() = default;
	explicit tContentPUBX_Poll_Clock(const std::vector<std::string>& val)
		:tContentPUBX_Poll(val)
	{
		if (IsVerified())
			SetVerified(MsgID.GetValue() == static_cast<int>(tUBXMsgID::Clock));
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tContentTXT : public base::tContentTXT
{
	enum class tMsgType
	{
		None,

		VersionHW,
		VersionExt,
		VersionInt,
		License,
		QDS,
		UnknownMessage,
	};

	tContentTXT() = delete;
	explicit tContentTXT(const base::tContentTXT& content)
		:base::tContentTXT(content)
	{
	}

	tMsgType GetMessageType() const
	{
		if (Msg.ends_with("unknown msg"))
			return tMsgType::UnknownMessage;
		return tMsgType::None;
	}

	std::string GetValue() const
	{
		return {};
	}
};

bool IsUnknownMessage(const base::tContentTXT& content);
bool IsVersionHW(const base::tContentTXT& content);
bool IsVersionExt(const base::tContentTXT& content);
bool IsVersionInt(const base::tContentTXT& content);
bool IsLicense(const base::tContentTXT& content);
bool IsQDS(const base::tContentTXT& content);
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
