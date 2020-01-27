///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayloadGSV.h
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
/*
template
<
	std::size_t FieldQty,
	int TimeSize,
	int LatitudeSize,
	int LongitudeSize
>
struct tPayloadRMC
{
	typedef Type::tGNSS gnss_type;
	typedef Type::tValid valid_type;
	typedef Type::tDate date_type;
	typedef Type::tTime<TimeSize> time_type;
	typedef Type::tLatitude<LatitudeSize> latitude_type;
	typedef Type::tLongitude<LongitudeSize> longitude_type;
	typedef Type::tFloat<2> speed_type;
	typedef Type::tFloat<2> course_type;
	typedef Type::tPositioning positioning_type;

	gnss_type GNSS;
	valid_type Valid;
	date_type Date;
	time_type Time;
	latitude_type Latitude;
	longitude_type Longitude;
	speed_type Speed;
	course_type Course;
	positioning_type Positioning;

	tPayloadRMC() = default;
	explicit tPayloadRMC(const tPayloadCommon::value_type& val)
	{
		if (val.size() == FieldQty && val[0].size() > 3 && !std::strcmp(&val[0][2],"RMC"))
		{
			GNSS = gnss_type(val[0]);
			Time = time_type(val[1]);
			Valid = valid_type(val[2]);
			Latitude = latitude_type(val[3], val[4]);
			Longitude = longitude_type(val[5], val[6]);
			Speed = speed_type(val[7]);
			Course = course_type(val[8]);
			Date = date_type(val[9]);
			Positioning = positioning_type(val[12]);
		}
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GNSS.ToString() + "RMC");
		Data.push_back(Time.ToString());
		Data.push_back(Valid.ToString());
		Data.push_back(Latitude.ToStringValue());
		Data.push_back(Latitude.ToStringHemisphere());
		Data.push_back(Longitude.ToStringValue());
		Data.push_back(Longitude.ToStringHemisphere());
		Data.push_back(Speed.ToString());
		Data.push_back(Course.ToString());
		Data.push_back(Date.ToString());
		Data.push_back("");//Magnetic variation, degrees
		Data.push_back("");//Magnetic variation, E/W
		Data.push_back(Positioning.ToString());

		return Data;
	}
};*/

	}
}
