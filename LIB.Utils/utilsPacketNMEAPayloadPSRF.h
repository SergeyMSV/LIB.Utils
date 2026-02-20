///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadPSRF
// 2026-02-20
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
namespace sirf
{
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadTXT : public type::tTypeVerified
{
	std::vector<std::string> Text;

	tPayloadTXT() = default;
	explicit tPayloadTXT(const tPayloadCommon::value_type& val)
	{
		if (val.size() < 2 || val[0] != GetID())
		{
			SetVerified(false);
			return;
		}

		for (std::size_t i = 1; i < val.size(); ++i)
		{
			Text.push_back(val[i]);
		}
	}

	static const char* GetID() { return "PSRFTXT"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back("PSRFTXT");
		for (auto i : Text)
		{
			Data.push_back(i);
		}
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
/*struct tPayloadVERSION_GET
{
	tPayloadVERSION_GET() = default;
	//explicit tPayloadVERSION_GET(const tPayloadCommon::value_type& val)
	//{
	//	if (val.size() != 3 || val[0] != "PTWS" || val[1] != "VERSION" || val[2] != "GET")
	//		SetVerified(false);
	//}

	bool IsVerified() const { return true; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back("PTWS");
		Data.push_back("VERSION");
		Data.push_back("GET");

		return Data;
	}
};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
/*struct tPayloadVERSION_VAL : public type::tTypeVerified
{
	using tVersion = std::string;

	tVersion Version;

	tPayloadVERSION_VAL() = default;
	explicit tPayloadVERSION_VAL(tVersion version)
		:Version(version)
	{}
	explicit tPayloadVERSION_VAL(const tPayloadCommon::value_type& val)
	{
		if (val.size() != 4 || val[0] != "PTWS" || val[1] != "VERSION" || val[2] != "VAL")
		{
			SetVerified(false);
			return;
		}
		Version = val[3];
	}

	bool IsVerified() const { return type::tTypeVerified::IsVerified() && !Version.empty(); }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back("PTWS");
		Data.push_back("VERSION");
		Data.push_back("VAL");
		Data.push_back(Version);
		return Data;
	}
};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
