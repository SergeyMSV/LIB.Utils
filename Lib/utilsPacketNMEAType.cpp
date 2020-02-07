#include "utilsPacketNMEAType.h"

namespace utils
{
	namespace packet_NMEA
	{
		namespace Type
		{
///////////////////////////////////////////////////////////////////////////////////////////////////
tGNSS::tGNSS(const std::string& val)
{
	if (val.size() >= 2 && val[0] == 'G')
	{
		switch (val[1])
		{
		case 'P': Value = tGNSS_State::GPS; break;
		case 'L': Value = tGNSS_State::GLONASS; break;
		case 'N': Value = tGNSS_State::GPS_GLONASS; break;
		}
	}
}

std::string tGNSS::ToString() const
{
	switch (Value)
	{
	case tGNSS_State::GPS: return "GP";
	case tGNSS_State::GLONASS: return "GL";
	case tGNSS_State::GPS_GLONASS: return "GN";
	}

	return "-";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tValid::tValid(const std::string& val)
{
	if (val.size() == 1)
	{
		Absent = false;

		Value = val[0] == 'A' ? true : false;
	}
}

std::string tValid::ToString() const
{
	if (Absent) return "";

	return Value ? "A" : "V";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tDate::tDate(tUInt8 year, tUInt8 month, tUInt8 day)
	:Year(year), Month(month), Day(day), Absent(false)
{

}

tDate::tDate(const std::string& val)
{
	if (val.size() == 6)//260216
	{
		Absent = false;

		char Data[3]{};//C++11

		Data[0] = val[0];
		Data[1] = val[1];

		Day = static_cast<tUInt8>(std::strtoul(Data, 0, 10));

		Data[0] = val[2];
		Data[1] = val[3];

		Month = static_cast<tUInt8>(std::strtoul(Data, 0, 10));

		Data[0] = val[4];
		Data[1] = val[5];

		Year = static_cast<tUInt8>(std::strtoul(Data, 0, 10));
	}
}

std::string tDate::ToString() const
{
	if (Absent) return "";

	char Str[10]{};

	if (Year < 100 && Month < 13 && Day < 32)
	{
		std::sprintf(Str, "%02d%02d%02d", Day, Month, Year);
	}

	return Str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tPositioning::tPositioning(const std::string& val)
{
	if (val.size() == 1)
	{
		switch (val[0])
		{
		case 'A': Value = tPositioning_State::Autonomous; break;
		case 'D': Value = tPositioning_State::Differential; break;
		case 'E': Value = tPositioning_State::Estimated; break;
		case 'M': Value = tPositioning_State::ManualInput; break;
		case 'S': Value = tPositioning_State::Simulator; break;
		case 'N': Value = tPositioning_State::DataNotValid; break;
		}
	}
}

std::string tPositioning::ToString() const
{
	switch (Value)
	{
	case tPositioning_State::Autonomous: return "A";
	case tPositioning_State::Differential: return "D";
	case tPositioning_State::Estimated: return "E";
	case tPositioning_State::ManualInput: return "M";
	case tPositioning_State::Simulator: return "S";
	case tPositioning_State::DataNotValid: return "N";
	}

	return "-";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tSatellite::tSatellite(tUInt8 id, tUInt8 elevation, tUInt16 azimuth, tUInt8 snr)
	:ID(id), Elevation(elevation), Azimuth(azimuth), SNR(snr), Absent(false)
{

}

tSatellite::tSatellite(const std::string& valID, const std::string& valElevation, const std::string& valAzimuth, const std::string& valSNR)
{
	if (valID.size() || valElevation.size() || valAzimuth.size() || valSNR.size())
	{
		Absent = false;

		ID = static_cast<tUInt8>(std::strtoul(valID.c_str(), 0, 10));
		Elevation = static_cast<tUInt8>(std::strtoul(valElevation.c_str(), 0, 10));
		Azimuth = static_cast<tUInt16>(std::strtoul(valAzimuth.c_str(), 0, 10));
		SNR = static_cast<tUInt8>(std::strtoul(valSNR.c_str(), 0, 10));
	}
}

std::string tSatellite::ToStringID() const
{
	if (Absent) return "";

	char Str[10]{};

	std::sprintf(Str, "%02d", ID);

	return Str;
}

std::string tSatellite::ToStringElevation() const
{
	if (Absent) return "";

	char Str[10]{};

	std::sprintf(Str, "%02d", Elevation);

	return Str;
}

std::string tSatellite::ToStringAzimuth() const
{
	if (Absent) return "";

	char Str[10]{};

	std::sprintf(Str, "%03d", Azimuth);

	return Str;
}

std::string tSatellite::ToStringSNR() const
{
	if (Absent) return "";

	char Str[10]{};

	std::sprintf(Str, "%02d", SNR);

	return Str;
}

std::string tSatellite::ToString() const
{
	return ToStringID() + ',' + ToStringElevation() + ',' + ToStringAzimuth() + ',' + ToStringSNR();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}
