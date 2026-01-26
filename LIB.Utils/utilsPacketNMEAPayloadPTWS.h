///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadPTWS
// 2020-02-11
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
namespace telit
{
///////////////////////////////////////////////////////////////////////////////////////////////////
//template
//<
//	typename TTime,
//	typename TLatitude,
//	typename TLongitude,
//	typename TSatQty,
//	typename THDOP,
//	typename TAltitude,
//	typename TGeoidSeparation,
//	typename TDiffAge,
//	typename TDiffStation
//>

	
//$GNRMC,235953.800,V,,,,,0.00,0.00,050180,,,N*5C
//$GNVTG,0.00,T,,M,0.00,N,0.00,K,N*2C
//$PTWS,JAM,SIGNAL,VAL,INDEX,7,FREQ,1575.453369*60
//$PTWS,JAM,SIGNAL,VAL,INDEX,8,FREQ,1575.197876*67
//$PTWS,JAM,SIGNAL,VAL,INDEX,9,FREQ,1575.260742*65
//$PTWS,JAM,SIGNAL,VAL,INDEX,10,FREQ,1575.130615*58
//$PTWS,JAM,SIGNAL,VAL,INDEX,11,FREQ,1575.385498*52


struct tPayloadPTWS_JAM_SIGNAL_VAL
{
	using tIndex = type::tUInt<2>;
	using tFrequency = type::tFloatPrecisionFixed<4, 6>;

	tIndex Index;
	tFrequency Frequency;

	tPayloadPTWS_JAM_SIGNAL_VAL() = default;
	tPayloadPTWS_JAM_SIGNAL_VAL(std::uint8_t index, double frequency)
		:Index(index), Frequency(frequency)
	{
	}
	explicit tPayloadPTWS_JAM_SIGNAL_VAL(const tPayloadCommon::value_type& val)
	{
		if (val.size() != 8 || val[0] != GetID() || val[1] != "JAM" || val[2] != "SIGNAL" || val[3] != "VAL" || val[4] != "INDEX" || val[6] != "FREQ")
			return;
		Index = tIndex(val[5]);
		Frequency = tFrequency(val[7]);
	}

	static const char* GetID() { return "PTWS"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back(GetID());
		Data.push_back("JAM");
		Data.push_back("SIGNAL");
		Data.push_back("VAL");
		Data.push_back("INDEX");
		Data.push_back(Index.ToString());
		Data.push_back("FREQ");
		Data.push_back(Frequency.ToString());
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPTWS_VERSION_GET
{
	tPayloadPTWS_VERSION_GET() = default;
	//explicit tPayloadPTWS_VERSION_GET(const tPayloadCommon::value_type& val)
	//{
	//	if (val.size() != 3 || val[0] != GetID() || val[1] != "VERSION" || val[2] != "GET")
	//		return;
	//}

	static const char* GetID() { return "PTWS"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GetID());
		Data.push_back("VERSION");
		Data.push_back("GET");

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPTWS_VERSION_VAL
{
	using tVersion = std::string;

	tVersion Version;

	tPayloadPTWS_VERSION_VAL() = default;
	explicit tPayloadPTWS_VERSION_VAL(tVersion version)
		:Version(version)
	{}
	explicit tPayloadPTWS_VERSION_VAL(const tPayloadCommon::value_type& val)
	{
		if (val.size() != 4 || val[0] != GetID() || val[1] != "VERSION" || val[2] != "VAL")
			return;
		Version = val[3];
	}

	static const char* GetID() { return "PTWS"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back(GetID());
		Data.push_back("VERSION");
		Data.push_back("VAL");
		Data.push_back(Version);
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
