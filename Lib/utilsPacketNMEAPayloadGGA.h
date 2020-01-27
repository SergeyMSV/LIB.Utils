///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadGGA.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 31  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"
#include "utilsPacketNMEA.h"
#include "utilsPacketNMEAType.h"

namespace utils
{
	namespace packet_NMEA
	{

template
<
	std::size_t FieldQty,
	int TimeSize,
	int LatitudeSize,
	int LongitudeSize
>
struct tPayloadGGA
{
	typedef Type::tGNSS gnss_type;
	typedef Type::tTime<TimeSize> time_type;
	typedef Type::tLatitude<LatitudeSize> latitude_type;
	typedef Type::tLongitude<LongitudeSize> longitude_type;

	gnss_type GNSS;
	time_type Time;
	latitude_type Latitude;
	longitude_type Longitude;

	tPayloadGGA() = default;
	explicit tPayloadGGA(const tPayloadCommon::value_type& val)
	{
		if (val.size() == FieldQty && val[0].size() > 3 && !std::strcmp(&val[0][2],"GGA"))
		{
			GNSS = gnss_type(val[0]);
			Time = time_type(val[1]);
			Latitude = latitude_type(val[2], val[3]);
			Longitude = longitude_type(val[4], val[5]);
		}
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GNSS.ToString() + "GGA");
		Data.push_back(Time.ToString());
		Data.push_back(Latitude.ToStringValue());
		Data.push_back(Latitude.ToStringHemisphere());
		Data.push_back(Longitude.ToStringValue());
		Data.push_back(Longitude.ToStringHemisphere());
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");
		Data.push_back("");

		return Data;
	}
};

	}
}
