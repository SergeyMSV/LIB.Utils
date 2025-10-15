#include "utilsPacketNMEAType.h"

#include <iomanip>

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
namespace hidden
{

std::ostream& operator<<(std::ostream& out, const tNumberFixedItem& value)
{
	if (value.Fractional)
	{
		out << '.';
		std::string ValueStr = std::to_string(value.Value);
		if (ValueStr.size() > value.Size)
		{
			ValueStr.resize(value.Size);
			out << ValueStr;
			return out;
		}
	}
	out << std::setfill('0') << std::setw(value.Size) << value.Value;
	return out;
}

std::pair<std::uint32_t, std::uint32_t> SplitDouble(double value, std::uint32_t precision)
{
	const std::uint32_t Mult = std::pow(10, precision);
	const double Temp = std::round(value * Mult);
	const std::uint32_t ValInt = static_cast<std::uint32_t>(Temp / Mult);
	const std::uint32_t ValFract = static_cast<std::uint32_t>(Temp - ValInt * Mult);
	return { ValInt, ValFract };
}

}
///////////////////////////////////////////////////////////////////////////////////////////////////
tDate::tDate(const std::string& values) : tBase(values)
{
	if (!IsValid((*this)[0], (*this)[1], (*this)[2]))
		clear();
}

tDate::tDate(std::int8_t year, std::int8_t month, std::int8_t day)
{
	if (!IsValid(year, month, day))
		return;
	std::vector<std::uint32_t> Items;
	Items.push_back(year);
	Items.push_back(month);
	Items.push_back(day);
	*this = tDate(Items);
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
/*tSatellite::tSatellite(std::uint8_t id, std::uint8_t elevation, std::uint16_t azimuth, std::uint8_t snr)
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
}*/
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
