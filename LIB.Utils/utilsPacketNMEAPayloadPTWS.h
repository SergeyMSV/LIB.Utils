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
struct tPayloadPTWS_JAM_SIGNAL_VAL
{
	/*type::tGNSS GNSS;
	TTime Time;								// UTC Time
	TLatitude Latitude;
	TLongitude Longitude;
	std::uint8_t FS = 0;					// Position Fix Indicator, 1 digit 
	TSatQty SatUsed = TSatQty(0);			// Satellites Used
	THDOP HDOP;								// Horizontal Dilution of Precision 
	TAltitude Altitude;						// Altitude re: mean-sea-level (geoid), meters
	TGeoidSeparation GeoidalSeparation;		// Geoidal Separation: the difference between the WGS-84 earth ellipsoid surface and mean-sea-level (geoid) surface, "-" = mean-sea-level surface below WGS - 84 ellipsoid surface.
	TDiffAge DiffAge;						// Age of Differential Corrections
	TDiffStation DiffStation;				// Diff. Reference Station ID

	tPayloadGGA() = default;
	explicit tPayloadGGA(const tPayloadCommon::value_type& val)
	{
		if (val.size() != 15 || val[0].size() < 3 || std::strcmp(&val[0][2], GetID()))
			return;
		GNSS = type::tGNSS(val[0]);
		Time = TTime(val[1]);
		Latitude = TLatitude(val[2], val[3]);
		Longitude = TLongitude(val[4], val[5]);
		FS = std::atoi(val[6].c_str());
		SatUsed = TSatQty(val[7]);
		HDOP = THDOP(val[8]);
		Altitude = TAltitude(val[9], val[10]);
		GeoidalSeparation = TGeoidSeparation(val[11], val[12]);
		DiffAge = TDiffAge(val[13]);
		DiffStation = TDiffStation(val[14]);
	}

	static const char* GetID() { return "GGA"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back(GNSS.ToString() + GetID());
		Data.push_back(Time.ToString());
		Data.push_back(Latitude.ToStringValue());
		Data.push_back(Latitude.ToStringHemisphere());
		Data.push_back(Longitude.ToStringValue());
		Data.push_back(Longitude.ToStringHemisphere());
		Data.push_back(std::to_string(FS));
		Data.push_back(SatUsed.ToString());
		Data.push_back(HDOP.ToString());
		Data.push_back(Altitude.ToStringValue());
		Data.push_back(Altitude.ToStringUnit());
		Data.push_back(GeoidalSeparation.ToStringValue());
		Data.push_back(GeoidalSeparation.ToStringUnit());
		Data.push_back(DiffAge.ToString());
		Data.push_back(DiffStation.ToString());
		return Data;
	}*/
};
/*struct tPayloadPTWS_JAM_SIGNAL_VAL
{
	typedef Type::tUInt<std::uint8_t, 0> index_type;
	typedef Type::tFloat<0, 6> frequency_type;

	index_type Index;
	frequency_type Frequency;

	tPayloadPTWS_JAM_SIGNAL_VAL() = default;
	explicit tPayloadPTWS_JAM_SIGNAL_VAL(std::uint8_t index, double frequency)
		:Index(index), Frequency(frequency)
	{}
	explicit tPayloadPTWS_JAM_SIGNAL_VAL(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			Index = index_type(val[5]);
			Frequency = frequency_type(val[7]);
		}
	}

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 8 &&
			!std::strcmp(val[0].c_str(), "PTWS") &&
			!std::strcmp(val[1].c_str(), "JAM") &&
			!std::strcmp(val[2].c_str(), "SIGNAL") &&
			!std::strcmp(val[3].c_str(), "VAL") &&
			!std::strcmp(val[4].c_str(), "INDEX") &&
			!std::strcmp(val[6].c_str(), "FREQ");
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back("PTWS");
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
	explicit tPayloadPTWS_VERSION_GET(const tPayloadCommon::value_type& val)
	{
		//if (Try(val))
		//{

		//}
	}

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 3 &&
			!std::strcmp(val[0].c_str(), "PTWS") &&
			!std::strcmp(val[1].c_str(), "VERSION") &&
			!std::strcmp(val[2].c_str(), "GET");
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back("PTWS");
		Data.push_back("VERSION");
		Data.push_back("GET");

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPayloadPTWS_VERSION_VAL
{
	typedef std::string version_type;

	version_type Version;

	tPayloadPTWS_VERSION_VAL() = default;
	explicit tPayloadPTWS_VERSION_VAL(version_type version)
		:Version(version)
	{}
	explicit tPayloadPTWS_VERSION_VAL(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			Version = val[3];
		}
	}

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == 4 &&
			!std::strcmp(val[0].c_str(), "PTWS") &&
			!std::strcmp(val[1].c_str(), "VERSION") &&
			!std::strcmp(val[2].c_str(), "VAL");
	}

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
