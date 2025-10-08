#include "utilsPacketNMEAType.h"

namespace utils
{
namespace packet
{
namespace nmea
{
namespace type
{
///////////////////////////////////////////////////////////////////////////////////////////////////
tGNSS::tGNSS(const std::string& val)
{
	if (val.size() != 2 || val[0] != 'G')
		return;

	switch (val[1])
	{
	case 'P': Value = tGNSS_State::GPS; break;
	case 'L': Value = tGNSS_State::GLONASS; break;
	case 'N': Value = tGNSS_State::GPS_GLONASS; break;
	}	
}

std::string tGNSS::ToString() const
{
	switch (Value)
	{
	case tGNSS_State::GPS: return "GP";
	case tGNSS_State::GLONASS: return "GL";
	case tGNSS_State::GPS_GLONASS: return "GN";
	default: return "--";
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tValid::tValid(const std::string& val)
{
	if (val.size() == 1 && (val[0] == 'A' || val[0] == 'V'))
		*this = tValid(val[0] == 'A' ? true : false);
}

std::string tValid::ToString() const
{
	switch (Value)
	{
	case tValidity::None: break;
	case tValidity::Valid: return "A";
	case tValidity::NotValid: return "V";
	}
	return "";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tDate::tDate(std::int8_t year, std::int8_t month, std::int8_t day)
{
	if (!IsValid(year, month, day))
		return;
	Year = year;
	Month = month;
	Day = day;
}

tDate::tDate(const std::string& val)
{
	if (val.size() != 6) // 260216
		return;
	std::int8_t ParsedDay = static_cast<std::int8_t>(std::strtoul(val.substr(0,2).c_str(), 0, 10));
	std::int8_t ParsedMonth = static_cast<std::int8_t>(std::strtoul(val.substr(2, 2).c_str(), 0, 10));
	std::int8_t ParsedYear = static_cast<std::int8_t>(std::strtoul(val.c_str() + 4, 0, 10));
	if (!IsValid(ParsedYear, ParsedMonth, ParsedDay))
		return;
	Day = ParsedDay;
	Month = ParsedMonth;
	Year = ParsedYear;
}

std::string tDate::ToString() const
{
	if (IsEmpty())
		return "";
	std::stringstream SStream;
	SStream << std::setfill('0');
	SStream << std::setw(2) << static_cast<int>(Day);
	SStream << std::setw(2) << static_cast<int>(Month);
	SStream << std::setw(2) << static_cast<int>(Year);
	return SStream.str();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tModeIndicator::tModeIndicator(const std::string& val)
{
	if (val.size() == 1 && val[0] >= 'A' && val[0] <= 'Z')
		Value = val[0];
}

std::string tModeIndicator::ToString() const
{
	switch (Value)
	{
	case 'A': return "Autonomous";
	case 'D': return "Differential";
	case 'E': return "Estimated";
	case 'M': return "Manual input";
	case 'S': return "Simulator";
	case 'N': return "Not valid";
	}
	return "Error";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tSatellite::tSatellite(std::uint8_t id, std::uint8_t elevation, std::uint16_t azimuth, std::uint8_t snr)
	:ID(id_type(id)), Elevation(elevation_type(elevation)), Azimuth(azimuth_type(azimuth)), SNR(snr_type(snr))
{

}

tSatellite::tSatellite(const std::string& valID, const std::string& valElevation, const std::string& valAzimuth, const std::string& valSNR)
{
	ID = id_type::Parse(valID);
	Elevation = elevation_type::Parse(valElevation);
	Azimuth = azimuth_type::Parse(valAzimuth);
	SNR = snr_type::Parse(valSNR);
}

std::string tSatellite::ToStringID() const
{
	return ID.ToString();
}

std::string tSatellite::ToStringElevation() const
{
	return Elevation.ToString();
}

std::string tSatellite::ToStringAzimuth() const
{
	return Azimuth.ToString();
}

std::string tSatellite::ToStringSNR() const
{
	return SNR.ToString();
}

std::string tSatellite::ToString() const
{
	return ToStringID() + ',' + ToStringElevation() + ',' + ToStringAzimuth() + ',' + ToStringSNR();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
