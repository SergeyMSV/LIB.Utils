///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAType.h
// 2020-01-27
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iomanip>
#include <optional> // [TBD] REMOVE
#include <sstream>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <cstring>

namespace utils
{
namespace packet
{
namespace nmea
{
namespace type
{

template<class T>
struct tOptional : public std::optional<T>
{
	tOptional() :std::optional<T>() {}
	tOptional(T value) :std::optional<T>(value) {}

	std::string ToString() const
	{
		return this->has_value() ? (*this)->ToString() : T::ToStringEmpty();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tGNSS_State : std::uint8_t // it's like bitfield
{
	None = 0,
	GPS = 1,     // 0000'0001
	GLONASS,     // 0000'0010
	GPS_GLONASS, // 0000'0011
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tGNSS
{
	tGNSS_State Value = tGNSS_State::None;

	tGNSS() = default;
	explicit tGNSS(tGNSS_State val) :Value(val) {}
	explicit tGNSS(const std::string& val);

	bool IsEmpty() const { return Value == tGNSS_State::None; }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class tValid
{
	enum class tValidity
	{
		None = 0,
		Valid,
		NotValid,
	};

	tValidity Value = tValidity::None;

public:
	tValid() = default;
	explicit tValid(bool val) :Value(val ? tValidity::Valid : tValidity::NotValid) {}
	explicit tValid(const std::string& val);

	bool IsEmpty() const { return Value == tValidity::None; }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tDate
{
	std::int8_t Year = -1;
	std::int8_t Month = -1;
	std::int8_t Day = -1;

	tDate() = default;
	tDate(std::int8_t year, std::int8_t month, std::int8_t day);
	explicit tDate(const std::string& val);

	bool IsEmpty() const { return !IsValid(Year, Month, Day); }

	std::string ToString() const;

	static bool IsValid(std::int8_t year, std::int8_t month, std::int8_t day) { return year > -1 && year < 100 && month > 0 && month <= 12 && day > 0 && day <= 31; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <int SizeFract>
class tTime
{
	static_assert(SizeFract >= 0, "tTime: SizeFract"); // C++11
	static const std::size_t Size = SizeFract == 0 ? 6 : 7 + SizeFract; // sizeof(hhmmss.) = 7

public:
	std::uint8_t Hour = 0;
	std::uint8_t Minute = 0;
	double Second = 0;

	tTime() = default;
	tTime(std::uint8_t hour, std::uint8_t minute, double second) :Hour(hour), Minute(minute), Second(second) {}

	static tOptional<tTime> Parse(const std::string& val)
	{
		if (val.size() != Size)
			return {};
		auto Hour = static_cast<std::uint8_t>(std::strtoul(val.substr(0,2).c_str(), 0, 10));
		auto Minute = static_cast<std::uint8_t>(std::strtoul(val.substr(2, 2).c_str(), 0, 10));
		double Second = std::strtod(val.c_str() + 4, 0);
		return tTime(Hour, Minute, Second);
	}

	template <int SizeFract1>
	friend std::ostream& operator<< (std::ostream& out, const tTime<SizeFract1>& value);

	std::string ToString() const
	{
		std::stringstream Stream;
		Stream << *this;
		return Stream.str();
	}
	static std::string ToStringEmpty() { return ""; }
};

template <int SizeFract>
std::ostream& operator<< (std::ostream& out, const tTime<SizeFract>& value)
{
	if (value.Hour > 23 || value.Minute > 59 || value.Second > 59)
		return out;

	out << std::setfill('0');
	out << std::setw(2) << static_cast<int>(value.Hour);
	out << std::setw(2) << static_cast<int>(value.Minute);

	if (SizeFract > 0)
	{
		out.setf(std::ios::fixed);
		out << std::setw(3 + SizeFract) << std::setprecision(SizeFract) << value.Second;
		out.unsetf(std::ios::fixed);
	}
	else
	{
		out << std::setw(2) << static_cast<int>(value.Second);
	}
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// The valid range of latitude in degrees is - 90 and +90 for the southern and northern hemisphere, respectively.
// Longitude is in the range - 180 and +180 specifying coordinates west and east of the Prime Meridian, respectively.For reference,
// the Equator has a latitude of 0°, the North pole has a latitude of 90° north(written 90° N or +90°), and the South pole has a latitude of - 90°.

template <std::size_t SizeDeg, std::size_t SizeFract, char Positive, char Negative, int MaxAbs>
class tGeoDegree
{
	// Latitude -> sizeof(ddmm.) = 2 + 3 + fract.
	// Longitude -> sizeof(dddmm.) = 3 + 3 + fract.
	static constexpr std::size_t Size = SizeDeg + 3 + SizeFract; // "dd" and "ddd" stand for SizeDeg for Latitude and Longitude respectively.

	enum { Empty = -500 };

public:
	double Value = Empty;

	tGeoDegree() = default;
	explicit tGeoDegree(double val)
	{
		if (IsValid(val))
			Value = val;
	}
	tGeoDegree(const std::string& val, const std::string& valSign)
	{
		if (val.size() != Size || valSign.size() != 1 || (valSign[0] != Negative && valSign[0] != Positive))
			return;
		double Lat = std::strtod(val.substr(0, SizeDeg).c_str(), 0);
		const double Rest = std::strtod(val.c_str() + SizeDeg, 0);
		Lat += Rest / 60;
		if (!IsValid(Lat))
			return;
		Value = valSign[0] == Positive ? Lat : -Lat;
	}

	bool IsEmpty() const { return !IsValid(Value); }

	std::string ToStringValue() const
	{
		if (IsEmpty())
			return "";
		const double ValueAbs = std::abs(Value);
		const std::int8_t Deg = static_cast<std::int8_t>(ValueAbs);
		const double Min = (ValueAbs - Deg) * 60;
		std::stringstream SStream;
		SStream << std::setfill('0');
		SStream << std::setw(SizeDeg) << static_cast<int>(Deg);
		SStream.setf(std::ios::fixed);
		SStream << std::setw(3 + SizeFract) << std::setprecision(SizeFract) << Min;
		SStream.unsetf(std::ios::fixed);
		return SStream.str();
	}

	std::string ToStringHemisphere() const
	{
		if (IsEmpty())
			return "";
		return std::string(1, Value < 0 ? Negative : Positive);
	}

	std::string ToString() const { return ToStringValue() + ',' + ToStringHemisphere(); }

	static bool IsValid(double value) { return value >= -MaxAbs && value <= MaxAbs; }
};

using tLatitude2 = tGeoDegree<2, 2, 'N', 'S', 90>;
using tLongitude2 = tGeoDegree<3, 2, 'E', 'W', 180>;

using tLatitude4 = tGeoDegree<2, 4, 'N', 'S', 90>;
using tLongitude4 = tGeoDegree<3, 4, 'E', 'W', 180>;

using tLatitude6 = tGeoDegree<2, 6, 'N', 'S', 90>;
using tLongitude6 = tGeoDegree<3, 6, 'E', 'W', 180>;

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt, std::size_t SizeFract>
class tFloat
{
	static const std::size_t Size = SizeInt + SizeFract + 1;

public:
	double Value = 0;

	tFloat() = default;
	explicit tFloat(double val) :Value(val) {}

	static tOptional<tFloat> Parse(const std::string& val)
	{
		if (val.size() != Size)
			return {};
		return tFloat(std::strtod(val.c_str(), 0));
	}

	std::string ToString() const
	{
		std::stringstream SStream;
		SStream << std::setfill('0');
		SStream.setf(std::ios::fixed);
		SStream << std::setw(SizeInt + SizeFract + 1) << std::setprecision(SizeFract) << Value;
		SStream.unsetf(std::ios::fixed);
		return SStream.str();
	}
	static std::string ToStringEmpty() { return ""; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeFract>
class tFloat<0, SizeFract>
{
	static const std::size_t SizeInt = 6;
	static const std::size_t SizeMax = SizeInt + SizeFract + 1;

public:
	double Value = 0;

	tFloat() = default;
	explicit tFloat(double val) :Value(val) {}

	static tOptional<tFloat> Parse(const std::string& val)
	{
		if (val.empty() || val.size() >= SizeMax)
			return {};
		return tFloat(std::strtod(val.c_str(), 0));
	}

	std::string ToString() const
	{
		std::stringstream SStream;
		SStream.setf(std::ios::fixed);
		SStream << std::setw(SizeFract + 1) << std::setprecision(SizeFract) << Value;
		SStream.unsetf(std::ios::fixed);
		return SStream.str();
	}
	static std::string ToStringEmpty() { return ""; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <> class tFloat<0, 0>; // Fractional part is just of max length (6 chars), therefore this specialisation makes no sense.
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt> class tFloat<SizeInt, 0>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt, std::size_t SizeFract>
class tFloatUnit
{
	typedef tFloat<SizeInt, SizeFract> value_type;

	static const std::size_t SizeValue = SizeInt + SizeFract + 1;

public:
	value_type Value;
	char Unit = 0;

	tFloatUnit() = default;
	tFloatUnit(double val, char unit) :Value(val), Unit(unit) {}

	static tOptional<tFloatUnit> Parse(const std::string& val, const std::string& valSign)
	{
		if (val.size() != SizeValue || valSign.size() != 1)
			return {};
		auto Value = value_type::Parse(val);
		if (!Value.has_value())
			return {};
		return tFloatUnit(Value->Value, valSign[0]);
	}

	std::string ToStringValue() const
	{
		return Value.ToString();
	}

	std::string ToStringUnit() const
	{
		return std::string(1, Unit);
	}

	std::string ToString() const
	{
		return ToStringValue() + ',' + ToStringUnit();
	}
	static std::string ToStringEmpty() { return ","; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tModeIndicator
{
	char Value = 'N';

	tModeIndicator() = default;
	explicit tModeIndicator(const std::string& val);
	std::string ToStringValue() const { return std::string(1, Value); }
	std::string ToString() const;
	static std::string ToStringEmpty() { return ""; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t Size>
struct tUInt
{
	T Value = static_cast<T>(0);

public:
	tUInt() = default;
	explicit tUInt(T val) :Value(val) {}

	static tOptional<tUInt> Parse(const std::string& val)
	{
		if (val.size() != Size)
			return {};
		return tUInt(static_cast<T>(std::strtoul(val.c_str(), 0, 10)));
	}

	std::string ToString() const
	{
		std::stringstream SStream;
		SStream << std::setfill('0') << std::setw(Size) << static_cast<unsigned int>(Value);
		return SStream.str();
	}
	static std::string ToStringEmpty() { return ""; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TValue>
struct tUInt<TValue, 0>
{
	TValue Value = static_cast<TValue>(0);

public:
	tUInt() = default;
	explicit tUInt(TValue val) :Value(val) {}

	static tUInt Parse(const std::string& val)
	{
		if (val.empty())
			return {};
		return tUInt(static_cast<TValue>(std::strtoul(val.c_str(), 0, 10)));
	}

	std::string ToString() const
	{
		std::stringstream SStream;
		SStream << static_cast<unsigned int>(Value);
		return SStream.str();
	}
	static std::string ToStringEmpty() { return ""; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tSatellite
{
	typedef tUInt<std::uint8_t, 2> id_type;
	typedef tUInt<std::uint8_t, 2> elevation_type;
	typedef tUInt<std::uint16_t, 3> azimuth_type;
	typedef tUInt<std::uint8_t, 2> snr_type;

	tOptional<id_type> ID;
	tOptional<elevation_type> Elevation;
	tOptional<azimuth_type> Azimuth;
	tOptional<snr_type> SNR;

	tSatellite() = default;
	tSatellite(std::uint8_t id, std::uint8_t elevation, std::uint16_t azimuth, std::uint8_t snr);
	explicit tSatellite(const std::string& valID, const std::string& valElevation, const std::string& valAzimuth, const std::string& valSNR);

	std::string ToStringID() const;
	std::string ToStringElevation() const;
	std::string ToStringAzimuth() const;
	std::string ToStringSNR() const;

	std::string ToString() const;
	static std::string ToStringEmpty() { return ",,,"; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
