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
tOptional<tGNSS> tGNSS::Parse(const std::string& val)
{
	if (val.size() < 2 || val[0] != 'G')
		return {};

	switch (val[1])
	{
	case 'P': return tGNSS(tGNSS_State::GPS); break;
	case 'L': return tGNSS(tGNSS_State::GLONASS); break;
	case 'N': return tGNSS(tGNSS_State::GPS_GLONASS); break;
	}
	return {};
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
tOptional<tValid> tValid::Parse(const std::string& val)
{
	if (val.size() != 1)
		return {};
	return tValid(val[0] == 'A' ? true : false);
}

std::string tValid::ToString() const
{
	return Value ? "A" : "V";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tDate::tDate(std::uint8_t year, std::uint8_t month, std::uint8_t day)
	:Year(year), Month(month), Day(day)
{

}

tOptional<tDate> tDate::Parse(const std::string& val)
{
	if (val.size() != 6) // 260216
		return {};
	auto Day = static_cast<std::uint8_t>(std::strtoul(val.substr(0,2).c_str(), 0, 10));
	auto Month = static_cast<std::uint8_t>(std::strtoul(val.substr(2, 2).c_str(), 0, 10));
	auto Year = static_cast<std::uint8_t>(std::strtoul(val.c_str() + 4, 0, 10));
	return tDate(Year, Month, Day);
}

std::string tDate::ToString() const
{
	if (Year > 99 || Month > 12 || Day > 31)
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
