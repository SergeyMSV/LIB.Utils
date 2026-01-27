///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAPayload
// 2020-01-31, 2026-01-17 refact.
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
	typename TTime,
	typename TLatitude,
	typename TLongitude,
	typename TSatQty,
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
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	typename TMsgQty,
	typename TMsgNum,
	typename TSatQty,
	typename TSVID,
	typename TElevation,
	typename TAzimuth,
	typename TSNR
>
struct tPayloadGSV							// GNSS Satellites in View
{
	type::tGNSS GNSS;
	TMsgQty MsgQty;							// Total number of messages, 1 to 9
	TMsgNum MsgNum;							// Message number, 1 to 9
	TSatQty SatInView;						// Satellites in View 

	struct tSatellite
	{
		TSVID SVID;							// SV ID (GPS: 1-32, SBAS 33-64 (33=PRN120), GLONASS: 65-96) 
		TElevation Elevation;				// Degree (Maximum 90)
		TAzimuth Azimuth;					// Degree (Range 0 to 359)
		TSNR SNR;							// SNR (C/No) 00-99 dB-Hz, null when not tracking
	};
	std::vector<tSatellite> Sats;

	tPayloadGSV() = default;
	explicit tPayloadGSV(const tPayloadCommon::value_type& val)
	{
		if (val.size() < 4 || val[0].size() < 3 || std::strcmp(&val[0][2], GetID()))
			return;
		GNSS = type::tGNSS(val[0]);
		MsgQty = TMsgQty(val[1]);
		MsgNum = TMsgNum(val[2]);
		SatInView = TSatQty(val[3]);
		const int SatQtyMsg = static_cast<int>((val.size() - 3) / 4);
		for (int i = 1; i <= SatQtyMsg; ++i)
		{
			int Index = 4 * i;
			tSatellite Sat{};
			Sat.SVID = TSVID(val[Index]);
			Sat.Elevation = TElevation(val[Index + 1]);
			Sat.Azimuth = TAzimuth(val[Index + 2]);
			Sat.SNR = TSNR(val[Index + 3]);
			Sats.push_back(Sat);
		}
	}

	static const char* GetID() { return "GSV"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;
		Data.push_back(GNSS.ToString() + GetID());
		Data.push_back(MsgQty.ToString());
		Data.push_back(MsgNum.ToString());
		Data.push_back(SatInView.ToString());
		for (auto& i : Sats)
		{
			Data.push_back(i.SVID.ToString());
			Data.push_back(i.Elevation.ToString());
			Data.push_back(i.Azimuth.ToString());
			Data.push_back(i.SNR.ToString());
		}
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	typename TTime,
	typename TLatitude,
	typename TLongitude,
	typename TSpeed,
	typename TCourse,
	typename TDate
>
struct tPayloadRMC12						// Recommended Minimum Specific GNSS Data
{
	type::tGNSS GNSS;
	TTime Time;								// UTC Time
	char Status = 0;						// A = Data valid, V = Navigation receiver warning
	TLatitude Latitude;
	TLongitude Longitude;
	TSpeed Speed;							// Speed Over Ground, knots
	TCourse Course;							// Course Over Ground, degrees
	TDate Date;

	tPayloadRMC12() = default;
	explicit tPayloadRMC12(const tPayloadCommon::value_type& val)
	{
		if (val.size() < 12 ||
			val[0].size() < 3 || std::strcmp(&val[0][2], GetID())
			|| !val[10].empty() || !val[11].empty()) // 10 and 11 stand for Magnetic variation (not supported by receiver))
			return;
		GNSS = type::tGNSS(val[0]);
		Time = TTime(val[1]);
		if (val[2].size() == 1 && (val[2] == "A" || val[2] == "V"))
			Status = val[2][0];
		Latitude = TLatitude(val[3], val[4]);
		Longitude = TLongitude(val[5], val[6]);
		Speed = TSpeed(val[7]);
		Course = TCourse(val[8]);
		Date = TDate(val[9]);
	}

	static const char* GetID() { return "RMC"; }

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data;

		Data.push_back(GNSS.ToString() + GetID());
		Data.push_back(Time.ToString());
		Data.push_back(Status ? std::string(1, Status) : "");
		Data.push_back(Latitude.ToStringValue());
		Data.push_back(Latitude.ToStringHemisphere());
		Data.push_back(Longitude.ToStringValue());
		Data.push_back(Longitude.ToStringHemisphere());
		Data.push_back(Speed.ToString());
		Data.push_back(Course.ToString());
		Data.push_back(Date.ToString());
		Data.push_back("");
		Data.push_back("");

		return Data;
	}
};

template
<
	typename TTime,
	typename TLatitude,
	typename TLongitude,
	typename TSpeed,
	typename TCourse,
	typename TDate,
	typename TMode
>
struct tPayloadRMC13 : public tPayloadRMC12<TTime, TLatitude, TLongitude, TSpeed, TCourse, TDate>
{
	using tBase = tPayloadRMC12<TTime, TLatitude, TLongitude, TSpeed, TCourse, TDate>;

	TMode Mode;								// Positioning system Mode Indicator

	tPayloadRMC13() = default;
	explicit tPayloadRMC13(const tPayloadCommon::value_type& val)
		:tBase(val)
	{
		if (val.size() != 13)
			return;
		Mode = TMode(val[12]);
	}

	tPayloadCommon::value_type GetPayload() const
	{
		tPayloadCommon::value_type Data = tBase::GetPayload();
		Data.push_back(Mode.ToString());
		return Data;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace generic
{
	using tMsgQty = type::tUIntFixed<1>;						// 0		Total number of messages, 1 to 9
	using tMsgNum = type::tUIntFixed<1>;						// 0		Message number, 1 to 9
	using tSatInViewQty = type::tUIntFixed<2>;					// 00 - 99
	using tSVID = type::tUIntFixed<2>;							// 00		SV ID (GPS: 1-32, SBAS 33-64 (33=PRN120), GLONASS: 65-96) 
	using tElevation = type::tUIntFixed<2>;						// 00		Degree (Maximum 90)
	using tAzimuth = type::tUIntFixed<3>;						// 000		Degree (Range 0 to 359)
	using tSNR = type::tUIntFixed<2>;							// 00		SNR (C/No) 00-99 dB-Hz, null when not tracking

	using tPayloadGSV = base::tPayloadGSV<tMsgQty, tMsgNum, tSatInViewQty, tSVID, tElevation, tAzimuth, tSNR>;
}

namespace mtk_eb500 // MT3329	AXN_1.30
{
	// Not valid
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,0,0,,1.0,M,10.0,M,,*??
	// $GPRMC,000000.000,V,0000.0000,N,00000.0000,E,0.00,0.00,000000,,,N*??
	//
	// Valid (DIFF)
	// $GPGGA,000000.000,0000.0000,N,00000.0000,E,0,0,1.23,123.4,M,12.3,M,0000,0000*??
	// $GPRMC,000000.000,A,0000.0000,N,00000.0000,E,123.00,123.00,000000,,,D*??
	//
	// $GPGSV,4,1,13,01,01,001,,02,02,002,22,03,33,003,33,04,04,004,*??
	using tTime = type::tTime<3>;									// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<4>;							// 0000.0000
	using tLongitude = type::tLongitude<4>;							// 00000.0000
	using tSatQty = type::tUInt<2>;									// 0 - 99
	using tHDOP = type::tFloatPrecisionFixed<2, 2>;					// ?.00
	using tAltitude = type::tFloatPrecisionFixedUnit<5, 1>;			// ?.0
	using tGeoidSeparation = type::tFloatPrecisionFixedUnit<4, 1>;	// ?.0
	using tDiffAge = type::tFloatPrecisionFixed<3, 1>;				// 0.0 - 999.9				[?]
	using tDiffStation = type::tUIntFixed<4>;						// 0000 - 9999
	using tSpeed = type::tFloatPrecisionFixed<4, 2>;				// 0.00 - 9999.99
	using tCourse = type::tFloatPrecisionFixed<3, 2>;				// 0.00 - 999.99
	using tMode = type::tMode;										// A

	using tPayloadGGA = base::tPayloadGGA<tTime, tLatitude, tLongitude, tSatQty, tHDOP, tAltitude, tGeoidSeparation, tDiffAge, tDiffStation>;

	using tPayloadGSV = generic::tPayloadGSV;
	
	using tPayloadRMC = base::tPayloadRMC13<tTime, tLatitude, tLongitude, tSpeed, tCourse, tDate, tMode>;
}

namespace mtk_eb800a // MT3339	AXN_3.8
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


	using tTime = type::tTime<3>;									// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<6>;							// 0000.000000
	using tLongitude = type::tLongitude<6>;							// 00000.000000
	using tSatQty = type::tUInt<2>;									// 0 - 99
	using tHDOP = type::tFloatPrecisionFixed<2, 2>;					// ?.00
	using tAltitude = type::tFloatPrecisionFixedUnit<5, 3>;			// ?.000
	using tGeoidSeparation = type::tFloatPrecisionFixedUnit<4, 3>;	// ?.000
	using tDiffAge = type::tFloatPrecisionFixed<3, 1>;				// 0.0 - 999.9			[?]
	using tDiffStation = type::tUIntFixed<4>;						// 0000 - 9999
	using tSpeed = type::tFloatPrecisionFixed<4, 2>;				// 0.00 - 9999.99
	using tCourse = type::tFloatPrecisionFixed<3, 2>;				// 0.00 - 999.99
	using tMode = type::tMode;										// A

	using tPayloadGGA = base::tPayloadGGA<tTime, tLatitude, tLongitude, tSatQty, tHDOP, tAltitude, tGeoidSeparation, tDiffAge, tDiffStation>;

	using tPayloadGSV = generic::tPayloadGSV;

	using tPayloadRMC = base::tPayloadRMC13<tTime, tLatitude, tLongitude, tSpeed, tCourse, tDate, tMode>;
}
//namespace mtk_axn_3_10
//{
// 
// 
//}
namespace mtk_sc872_a // MT3333	AXN_3.84
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


	using tTime = type::tTime<3>;									// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<4>;							// 0000.0000
	using tLongitude = type::tLongitude<4>;							// 00000.0000
	using tSatQty = type::tUInt<2>;									// 0 - 99
	using tHDOP = type::tFloatPrecisionFixed<2, 2>;					// ?.00
	using tAltitude = type::tFloatPrecisionFixedUnit<5, 1>;			// ?.0
	using tGeoidSeparation = type::tFloatPrecisionFixedUnit<4, 1>;	// ?.0
	using tDiffAge = type::tFloatPrecisionFixed<3, 1>;				// 0.0 - 999.9			[?]
	using tDiffStation = type::tUIntFixed<4>;						// 0000 - 9999
	using tSpeed = type::tFloatPrecisionFixed<4, 2>;				// 0.00 - 9999.99
	using tCourse = type::tFloatPrecisionFixed<3, 2>;				// 0.00 - 999.99
	using tMode = type::tMode;										// A

	using tPayloadGGA = base::tPayloadGGA<tTime, tLatitude, tLongitude, tSatQty, tHDOP, tAltitude, tGeoidSeparation, tDiffAge, tDiffStation>;

	using tPayloadGSV = generic::tPayloadGSV;

	using tPayloadRMC = base::tPayloadRMC13<tTime, tLatitude, tLongitude, tSpeed, tCourse, tDate, tMode>;
}

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

	using tTime = type::tTime<3>;									// 000000.000
	using tDate = type::tDate;
	using tLatitude = type::tLatitude<4>;							// 0000.0000
	using tLongitude = type::tLongitude<4>;							// 00000.0000
	using tSatQty = type::tUIntFixed<2>;							// 00
	using tHDOP = type::tFloatFixed<2, 1>;							// 00.0
	using tAltitude = type::tFloatFixedUnit<5, 1>;					// 00000.0
	using tGeoidSeparation = type::tFloatFixedUnit<4, 1>;			// 0000.0
	using tDiffAge = type::tFloatFixed<3, 1>;						// 000.0
	using tDiffStation = type::tUIntFixed<4>;						// 0000
	using tSpeed = type::tFloatFixed<4, 2>;							// 9999.99
	using tCourse = type::tFloatFixed<3, 2>;						// 999.99

	using tPayloadGGA = base::tPayloadGGA<tTime, tLatitude, tLongitude, tSatQty, tHDOP, tAltitude, tGeoidSeparation, tDiffAge, tDiffStation>;

	using tPayloadGSV = generic::tPayloadGSV;

	using tPayloadRMC = base::tPayloadRMC12<tTime, tLatitude, tLongitude, tSpeed, tCourse, tDate>;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
