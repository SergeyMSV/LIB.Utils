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
	int LongitudeSize,
	int AltitudeIntSize,
	int AltitudeFactSize,
	int GeoidalSeparationIntSize,
	int GeoidalSeparationFactSize
>
struct tPayloadGGA
{
	typedef Type::tGNSS gnss_type;
	typedef Type::tTime<TimeSize> time_type;
	typedef Type::tLatitude<LatitudeSize> latitude_type;
	typedef Type::tLongitude<LongitudeSize> longitude_type;
	typedef Type::tFloatUnit<AltitudeIntSize, AltitudeFactSize> altitude_type;
	typedef Type::tFloatUnit<GeoidalSeparationIntSize, GeoidalSeparationFactSize> geoidal_separation_type;

	gnss_type GNSS;
	time_type Time;
	latitude_type Latitude;
	longitude_type Longitude;
	altitude_type Altitude;
	geoidal_separation_type GeoidalSeparation;

	tPayloadGGA() = default;
	explicit tPayloadGGA(const tPayloadCommon::value_type& val)
	{
		if (val.size() == FieldQty && val[0].size() > 3 && !std::strcmp(&val[0][2],"GGA"))
		{
			GNSS = gnss_type(val[0]);
			Time = time_type(val[1]);
			Latitude = latitude_type(val[2], val[3]);
			Longitude = longitude_type(val[4], val[5]);
			Altitude = altitude_type(val[9],val[10]);
			GeoidalSeparation = geoidal_separation_type(val[11], val[12]);
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
		Data.push_back(Altitude.ToStringValue());
		Data.push_back(Altitude.ToStringUnit());
		Data.push_back(GeoidalSeparation.ToStringValue());
		Data.push_back(GeoidalSeparation.ToStringUnit());
		Data.push_back("");
		Data.push_back("");

		return Data;
	}
};

	}
}
