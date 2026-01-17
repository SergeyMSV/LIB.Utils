///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayload
// 2020-01-31
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
template
<
	std::size_t FieldQty,
	typename TTime,
	typename TLatitude,
	typename TLongitude,
	typename TSatUsed,
	typename THDOP,
	typename TAltitude,
	typename TGeoidSeparation,
	typename TDiffAge,
	typename TDiffStation
>
struct tPayloadGGA							// Global Positioning System Fix Data
{
	type::tGNSS GNSS;
	TTime Time;								// UTC Time
	TLatitude Latitude;
	TLongitude Longitude;
	std::uint8_t FS = 0;					// Position Fix Indicator, 1 digit 
	TSatUsed NoSV = TSatUsed(0);			// Satellites Used
	THDOP HDOP;								// Horizontal Dilution of Precision 
	TAltitude Altitude;						// Altitude re: mean-sea-level (geoid), meters
	TGeoidSeparation GeoidalSeparation;		// Geoidal Separation: the difference between the WGS-84 earth ellipsoid surface and mean-sea-level (geoid) surface, "-" = mean-sea-level surface below WGS - 84 ellipsoid surface.
	TDiffAge DiffAge;						// Age of Differential Corrections
	TDiffStation DiffStation;				// Diff. Reference Station ID

	tPayloadGGA() = default;
	explicit tPayloadGGA(const tPayloadCommon::value_type& val)
	{
		if (!Try(val))
			return;
		GNSS = type::tGNSS(val[0]);
		Time = TTime(val[1]);
		Latitude = TLatitude(val[2], val[3]);
		Longitude = TLongitude(val[4], val[5]);
		FS = std::atoi(val[6].c_str());
		NoSV = TSatUsed(val[7]);
		HDOP = THDOP(val[8]);
		Altitude = TAltitude(val[9], val[10]);
		GeoidalSeparation = TGeoidSeparation(val[11], val[12]);
	}

	static const char* GetID() { return "GGA"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == FieldQty && val[0].size() > 3 && !std::strcmp(&val[0][2], GetID());
	}

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
		Data.push_back(NoSV.ToString());
		Data.push_back(HDOP.ToString());
		Data.push_back(Altitude.ToStringValue());
		Data.push_back(Altitude.ToStringUnit());
		Data.push_back(GeoidalSeparation.ToStringValue());
		Data.push_back(GeoidalSeparation.ToStringUnit());
		Data.push_back("");
		Data.push_back("");

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
/*struct tPayloadGSV
{
	typedef Type::tGNSS gnss_type;
	typedef Type::tUInt<std::uint8_t, 1> counter_type;
	typedef Type::tUInt<std::uint8_t, 2> satellite_counter_type;
	typedef Type::tSatellite satellite_type;
	typedef std::vector<satellite_type> satellite_collection_type;

	Type::tOptional<gnss_type> GNSS;
	Type::tOptional<counter_type> MsgQty;
	Type::tOptional<counter_type> MsgNum;
	Type::tOptional<satellite_counter_type> SatelliteQty;
	satellite_collection_type Satellite;

	tPayloadGSV() = default;
	explicit tPayloadGSV(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			GNSS = gnss_type::Parse(val[0]);
			MsgQty = counter_type::Parse(val[1]);
			MsgNum = counter_type::Parse(val[2]);
			SatelliteQty = satellite_counter_type::Parse(val[3]);

			int SatQty = static_cast<int>((val.size() - 3) / 4);

			for (int i = 1; i <= SatQty; ++i)
			{
				int Index = 4 * i;
				Satellite.push_back(satellite_type(val[Index], val[Index + 1], val[Index + 2], val[Index + 3]));
			}
		}
	}

	static const char* GetID() { return "GSV"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() > 3 && val[0].size() > 3 && !std::strcmp(&val[0][2], GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GNSS.ToString() + GetID());
		Data.push_back(MsgQty.ToString());
		Data.push_back(MsgNum.ToString());
		Data.push_back(SatelliteQty.has_value() ? SatelliteQty->ToString() : "");

		for (auto& i : Satellite) // C++11
		{
			Data.push_back(i.ToString());
		}

		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	std::size_t FieldQty,
	int TimeSizeFract,
	int LatitudeSizeFract,
	int LongitudeSizeFract
>
struct tPayloadRMC
{
	typedef Type::tGNSS gnss_type;
	typedef Type::tValid valid_type;
	typedef Type::tDate date_type;
	typedef Type::tTime<TimeSizeFract> time_type;
	typedef Type::tLatitude<LatitudeSizeFract> latitude_type;
	typedef Type::tLongitude<LongitudeSizeFract> longitude_type;
	typedef Type::tFloat<0, 2> speed_type;
	typedef Type::tFloat<0, 2> course_type;
	typedef Type::tModeIndicator mode_indicator_type;

	Type::tOptional<gnss_type> GNSS;
	Type::tOptional<valid_type> Valid;
	Type::tOptional<date_type> Date;
	time_type Time;
	latitude_type Latitude;
	longitude_type Longitude;
	speed_type Speed;
	course_type Course;
	mode_indicator_type ModeIndicator;

	tPayloadRMC() = default;
	explicit tPayloadRMC(const tPayloadCommon::value_type& val)
	{
		if (Try(val))
		{
			GNSS = gnss_type::Parse(val[0]);
			Time = time_type(val[1]);
			Valid = valid_type::Parse(val[2]);
			Latitude = latitude_type(val[3], val[4]);
			Longitude = longitude_type(val[5], val[6]);
			Speed = speed_type(val[7]);
			Course = course_type(val[8]);
			Date = date_type::Parse(val[9]);
			ModeIndicator = mode_indicator_type(val[12]);
		}
	}

	static const char* GetID() { return "RMC"; }

	static bool Try(const tPayloadCommon::value_type& val)
	{
		return val.size() == FieldQty && val[0].size() > 3 && !std::strcmp(&val[0][2], GetID());
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GNSS.ToString() + GetID());
		Data.push_back(Time.ToString());
		Data.push_back(Valid.ToString());
		Data.push_back(Latitude.ToStringValue());
		Data.push_back(Latitude.ToStringHemisphere());
		Data.push_back(Longitude.ToStringValue());
		Data.push_back(Longitude.ToStringHemisphere());
		Data.push_back(Speed.ToString());
		Data.push_back(Course.ToString());
		Data.push_back(Date.ToString());
		Data.push_back("");
		Data.push_back("");
		Data.push_back(ModeIndicator.ToStringValue());

		return Data;
	}
};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace mtk_axn_1_30_eb500 // PMTK705,AXN_1.30,5023,EB500,1.0
{
	// Not valid
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,0,0,,1.0,M,10.0,M,,*??
	// $GPRMC,000000.000,V,0000.0000,N,00000.0000,E,0.00,0.00,000000,,,N*??
	//
	// Valid
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,0,0,1.23,123.4,M,12.3,M,0000,0000*??
	// $GPRMC,000000.000,A,0000.0000,N,00000.0000,E,123.00,123.00,000000,,,D*??
	//
	// $GPGSV,4,1,13,01,01,001,,02,02,002,22,03,33,003,33,04,04,004,*??
	using tTime = type::tTime<3>;									// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<4>;							// 0000.0000
	using tLongitude = type::tLongitude<4>;							// 00000.0000
	using tSatUsed = type::tUInt<2>;								// 0 - 99
	using tHDOP = type::tFloatPrecisionFixed<2, 2>;					// ?.00
	using tAltitude = type::tFloatPrecisionFixedUnit<5, 1>;			// ?.0
	using tGeoidSeparation = type::tFloatPrecisionFixedUnit<4, 1>;	// ?.0

	//using tPayloadGGA = base::tPayloadGGA <15, tTime, tLatitude, tLongitude, tSatUsed, tHDOP, tAltitude, tGeoidSeparation>;
}
//namespace mtk_axn_1_80
//{
//}
//namespace mtk_axn_telit_jupiter
//{
// 
// using tDiffStation = type::tUIntFixed<4>;			// 0000-1023
//}
//namespace mtk_axn_3_8//4
//{
//	// ---- Not valid
//	// ---- $GNGGA,000000.000,,,,,0,0,,,M,,M,,*50
//	// ---- $GNRMC,000000.000,V,,,,,0.00,0.00,120180,,,N*5F
//	// ---- 
//	// ---- Valid
//	// ---- $GNGGA,000000.000,0000.0000,N,00000.0000,E,0,0,0.00,000.0,M,00.0,M,,*??
//	// ---- $GNRMC,000000.000,A,0000.0000,N,00000.0000,E,0.00,12.34,000000,,,A*??
//	// ---- 
//	// ---- $GPGSV,2,1,06,01,11,111,11,02,22,222,22,03,33,333,33,04,44,444,44*??
//	// ---- $GPGSV,2,2,06,09,99,999,,10,10,110,*??
//	// ---- $GLGSV,1,1,03,01,11,111,11,02,11,222,22,03,33,333,*??
//}

namespace mtk_axn_3_8_eb800a
{
	// $GNGGA,235949.799,,,,,0,0,,,M,,M,,*51
	// $GPGSV,1,1,04,01,,,44,02,,,30,03,,,45,17,,,38*72
	// $GLGSV,1,1,00*65
	// $GNRMC,235949.799,V,,,,,0.00,0.00,050180,,,N*58
	// $GNGGA,235950.277,,,,,0,0,,,M,,M,,*5C
	// $GPGSV,1,1,04,01,,,44,02,,,31,03,,,45,04,,,04*??
	// $GLGSV,1,1,00*65

	// $GNRMC,225721.000,A,1234.567890,N,01234.567890,E,0.80,337.72,120126,,,A*??
	// $GNGGA,225722.000,1234.567890,N,01234.567890,E,1,4,1.23,123.456,M,12.456,M,,*??
	// $GPGSV,2,1,06,01,01,001,01,02,02,002,02,03,03,003,,04,,,04*??
	// $GPGSV,2,2,06,05,,,05,06,,,06*??
	// $GLGSV,2,1,05,65,65,065,65,66,66,066,66,67,67,067,,68,68,068,68*??
	// $GLGSV,2,2,05,69,69,069,69*??


	using tTime = type::tTime<3>;											// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<6>;									// 0000.000000
	using tLongitude = type::tLongitude<6>;									// 00000.000000
	using tSatUsed = type::tUInt<2>;										// 0 - 99
	using tHDOP = type::tFloatPrecisionFixed<2, 2>;							// ?.00
	using tAltitude = type::tFloatPrecisionFixedUnit<5, 3>;					// ?.000
	using tGeoidSeparation = type::tFloatPrecisionFixedUnit<4, 3>;			// ?.000
	using tDiffAge = type::tFloatFixed<3, 1>;								// 000.0
	using tDiffStation = type::tUIntFixed<4>;								// 0000	

	//using tDiffAge = type::tFloat<1, 3>;				// 000.0
	//using tDiffStation = type::tUInt<4>;				// 0000				[TBD] Verify format.
	//using tSatUsed = type::tUInt<2>;					// 0 - 99

	//using tPayloadGGA = base::tPayloadGGA <15, tTime, tLatitude, tLongitude, tSatUsed, tHDOP, tAltitude, tGeoidSeparation>;
}
//namespace mtk_axn_3_10
//{
// 
// 
//}
namespace sirf_gsu_7x
{
	// Not valid
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,0,00,99.9,00000.0,M,0000.0,M,000.0,0000*??
	// $GPRMC,000000.000,V,0000.0000,N,00000.0000,E,9999.99,999.99,000000,,*??
	// 
	// Valid
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,1,12,00.1,00123.4,M,0012.3,M,000.0,0000*??
	// $GPRMC,000000.000,A,0000.0000,N,00000.0000,E,0012.34,123.45,000000,,*??
	// 
	// $GPGSV,3,1,12,01,01,001,,02,02,002,,03,03,003,,04,04,004,*??

	using tTime = type::tTime<3>;							// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<4>;					// 0000.0000
	using tLongitude = type::tLongitude<4>;					// 00000.0000
	using tSatUsed = type::tUIntFixed<2>;					// 00
	using tHDOP = type::tFloatFixed<2, 1>;					// 00.0
	using tAltitude = type::tFloatFixedUnit<5, 1>;			// 00000.0
	using tGeoidSeparation = type::tFloatFixedUnit<4, 1>;	// 0000.0
	using tDiffAge = type::tFloatFixed<3, 1>;				// 000.0
	using tDiffStation = type::tUIntFixed<4>;				// 0000

	using tPayloadGGA = base::tPayloadGGA <15, tTime, tLatitude, tLongitude, tSatUsed, tHDOP, tAltitude, tGeoidSeparation, tDiffAge, tDiffStation>;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
