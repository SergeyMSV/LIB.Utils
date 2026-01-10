///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAType
// 2020-01-27
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
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
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tGNSS_State : std::uint8_t // It's like bitfield.
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
	explicit tGNSS(tGNSS_State val) : Value(val) {}
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
	explicit tValid(bool val) : Value(val ? tValidity::Valid : tValidity::NotValid) {}
	explicit tValid(const std::string& val);

	bool IsEmpty() const { return Value == tValidity::None; }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace hidden
{

struct tNumberFixedItem
{
	std::uint32_t Value;
	std::uint32_t Size;
	bool Fractional;

	tNumberFixedItem() = default;
	tNumberFixedItem(std::uint32_t value, std::uint32_t size, bool fract) : Value(value), Size(size), Fractional(fract) {}
};

template <std::uint32_t... ints>
class tNumberFixed
{
	using tVectorParts = std::vector<tNumberFixedItem>;

	tVectorParts m_Parts;

public:
	tNumberFixed() = default;
	explicit tNumberFixed(const std::string& values)
	{
		if (values.size() != GetValuesStringSize())
			return;
		int PartIndex = -1;
		std::uint32_t Position = values.size();
		if (((!Parse(values, ++PartIndex, ints, Position)) || ...))
			m_Parts.clear(); // That means that the values haven't been parsed due to wrong format or something like that.
	}
	explicit tNumberFixed(const std::vector<std::uint32_t>& values)
	{
		int PartIndex = -1;
		auto MakeParts = [this](int index, std::uint32_t size)
			{
				if (!size && !index)
					return;
				this->m_Parts.emplace_back(0, size, index == 0);
			};
		(MakeParts(++PartIndex, ints), ...);

		if (m_Parts.size() != values.size())
		{
			m_Parts.clear();
			return;
		}

		for (std::size_t i = 0; i < values.size(); ++i)
			m_Parts[i].Value = values[i];
	}

	void clear() { m_Parts.clear(); }
	bool empty() const { return m_Parts.empty(); }
	std::size_t size() const { return m_Parts.size(); }

	std::string ToString() const
	{
		tVectorParts Parts = m_Parts;
		std::reverse(Parts.begin(), Parts.end());
		std::stringstream SStr;
		std::for_each(Parts.cbegin(), Parts.cend(), [&SStr](const tNumberFixedItem& item)
			{
				SStr << item;
			});
		return SStr.str();
	}

protected:
	std::uint32_t operator[](std::size_t index) const
	{
		return index < m_Parts.size() ? m_Parts[index].Value : 0;
	}

private:
	bool Parse(const std::string& values, int partIndex, int size, std::uint32_t& position)
	{
		if (!partIndex)
			return ParseFract(values, size, position);

		if (position < size)
			return false;
		position -= size;
		m_Parts.emplace_back(strtoul(values.substr(position, size).c_str(), nullptr, 10), size, false);
		return true;
	}

	bool ParseFract(const std::string& values, int size, std::uint32_t& position)
	{
		if (!size)
			return true;
		std::size_t DotPos = values.find('.');
		if (DotPos == std::string::npos)
			return false;
		std::size_t FractSize = values.size() - DotPos - 1;
		if (FractSize != size)
			return false;
		m_Parts.emplace_back(strtoul(values.substr(DotPos + 1).c_str(), nullptr, 10), size, true);
		position = static_cast<std::uint32_t>(DotPos);
		return true;
	}

	static std::size_t GetValuesStringSize()
	{
		int Counter = 0;
		std::size_t Size = 0;
		(GetValuesStringPartSize(Counter++, ints, std::ref(Size)), ...);
		return Size;
	}

	static void GetValuesStringPartSize(std::uint32_t position, std::uint32_t sizePart, std::size_t& size)
	{
		size += !position && sizePart > 0 ? sizePart + 1 : sizePart;
	}
};

std::ostream& operator<<(std::ostream& out, const tNumberFixedItem& value);

std::pair<std::uint32_t, std::uint32_t> SplitDouble(double value, std::uint32_t precision);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
class tDate : public hidden::tNumberFixed<0, 2, 2, 2>
{
	using tBase = hidden::tNumberFixed<0, 2, 2, 2>;

	explicit tDate(std::vector<std::uint32_t>& values) : tBase(values) {}

public:
	tDate() = default;
	explicit tDate(const std::string& values);
	explicit tDate(std::int8_t year, std::int8_t month, std::int8_t day);

	std::uint8_t GetYear() const { return (*this)[0]; }
	std::uint8_t GetMonth() const { return (*this)[1]; }
	std::uint8_t GetDay() const { return (*this)[2]; }

private:
	static bool IsValid(std::int8_t year, std::int8_t month, std::int8_t day) { return year > -1 && year < 100 && month > 0 && month <= 12 && day > 0 && day <= 31; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::uint32_t Precision>
class tTime : public hidden::tNumberFixed<Precision, 2, 2, 2>
{
	using tBase = hidden::tNumberFixed<Precision, 2, 2, 2>;

	explicit tTime(std::vector<std::uint32_t>& values) : tBase(values) {}

public:
	tTime() = default;
	explicit tTime(const std::string& values) : tBase(values)
	{
		if (!IsValid((*this)[3], (*this)[2], (*this)[1]))
			this->clear();
	}
	explicit tTime(std::int8_t hour, std::int8_t minute, double second)
	{
		auto Sec = static_cast<std::int8_t>(second);
		if (!IsValid(hour, minute, Sec))
			return;
		std::vector<std::uint32_t> Items;
		if (Precision)
		{
			std::uint32_t SecFract = (second - Sec) * std::pow(10, Precision);
			Items.push_back(SecFract);
		}
		Items.push_back(Sec);
		Items.push_back(minute);
		Items.push_back(hour);
		*this = tTime(Items);
	}

	std::uint8_t GetHour() const { return (*this)[3]; }
	std::uint8_t GetMinute() const { return (*this)[2]; }
	double GetSecond() const { return static_cast<double>((*this)[1]) + (*this)[0] * std::pow(10, Precision); }

private:
	static bool IsValid(std::int8_t hour, std::int8_t minute, std::int8_t second) { return hour > -1 && hour < 24 && minute > -1 && minute < 60 && second > -1 && second < 60; }
};

using tTime0 = tTime<0>;
using tTime1 = tTime<1>;
using tTime2 = tTime<2>;
using tTime3 = tTime<3>;
///////////////////////////////////////////////////////////////////////////////////////////////////
// The valid range of latitude in degrees is - 90 and +90 for the southern and northern hemisphere, respectively.
// Longitude is in the range - 180 and +180 specifying coordinates west and east of the Prime Meridian, respectively.For reference,
// the Equator has a latitude of 0°, the North pole has a latitude of 90° north(written 90° N or +90°), and the South pole has a latitude of - 90°.
template <std::uint32_t SizeDeg, std::uint32_t Precision, char Positive, char Negative, int MaxAbs>
class tGeoDegree : public hidden::tNumberFixed<Precision, 2, SizeDeg>
{
	using tBase = hidden::tNumberFixed<Precision, 2, SizeDeg>;

	bool m_Negative;

	explicit tGeoDegree(std::vector<std::uint32_t>& values, bool negative) : tBase(values), m_Negative(negative) {}

public:
	tGeoDegree() = default;
	explicit tGeoDegree(const std::string& values, const std::string& sign)
		: tBase(values)
	{
		if (sign.size() != 1 || (sign[0] != Negative && sign[0] != Positive))
		{
			this->clear();
			return;
		}
		m_Negative = sign[0] == Negative;

		if (!IsValid(GetDegree()))
			this->clear();
	}
	explicit tGeoDegree(double degree)
	{
		if (!IsValid(degree))
			return;
		m_Negative = degree < 0;
		degree = std::abs(degree);
		const std::uint32_t Deg = static_cast<std::int8_t>(degree);
		const double Min1 = (degree - Deg) * 60;
		auto [Min, MinFract] = hidden::SplitDouble(Min1, Precision);
		std::vector<std::uint32_t> Items;
		Items.push_back(MinFract);
		Items.push_back(Min);
		Items.push_back(Deg);
		*this = tGeoDegree(Items, m_Negative);
	}

	double GetValue() const
	{
		return GetDegree() * m_Negative ? -1 : 1;
	}

	std::string ToString() const
	{
		if (this->empty())
			return ",";
		return tBase::ToString() + ',' + (m_Negative ? Negative : Positive);
	}

private:
	double GetDegree() const
	{
		return (*this)[2] + ((*this)[1] + (*this)[0] / std::pow(10, Precision)) / 60;
	}

	static bool IsValid(double value) { return value >= -MaxAbs && value <= MaxAbs; }
};

template <std::uint32_t Precision>
class tLatitude : public tGeoDegree<2, Precision, 'N', 'S', 90>
{
	using tBase = tGeoDegree<2, Precision, 'N', 'S', 90>;

public:
	tLatitude() = default;
	explicit tLatitude(const std::string& values, const std::string& sign) : tBase(values, sign) {}
	explicit tLatitude(double degree) : tBase(degree) {}
};

template <std::uint32_t Precision>
class tLongitude : public tGeoDegree<3, Precision, 'E', 'W', 180>
{
	using tBase = tGeoDegree<3, Precision, 'E', 'W', 180>;

public:
	tLongitude() = default;
	explicit tLongitude(const std::string& values, const std::string& sign) : tBase(values, sign) {}
	explicit tLongitude(double degree) : tBase(degree) {}
};

using tLatitude2 = tLatitude<2>;
using tLongitude2 = tLongitude<2>;

using tLatitude4 = tLatitude<4>;
using tLongitude4 = tLongitude<4>;

using tLatitude6 = tLatitude<6>;
using tLongitude6 = tLongitude<6>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
class tUInt : public hidden::tNumberFixed<0, Size>
{
	using tBase = hidden::tNumberFixed<0, Size>;

	explicit tUInt(std::vector<std::uint32_t>& values) : tBase(values) {}

public:
	tUInt() = default;
	explicit tUInt(const std::string& values) : tBase(values) {}
	explicit tUInt(std::uint32_t value)
	{
		std::vector<std::uint32_t> Items;
		Items.push_back(value);
		*this = tUInt(Items);
	}

	std::uint8_t GetValue() const { return (*this)[1]; }
};

using tUInt1 = tUInt<1>;
using tUInt2 = tUInt<2>;
using tUInt3 = tUInt<3>;
using tUInt4 = tUInt<4>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::uint32_t Precision, std::size_t Size>
class tFloat : public hidden::tNumberFixed<Precision, Size>
{
	using tBase = hidden::tNumberFixed<Precision, Size>;

	explicit tFloat(std::vector<std::uint32_t>& values) : tBase(values) {}

public:
	tFloat() = default;
	explicit tFloat(const std::string& values) : tBase(values) {}
	explicit tFloat(double value)
	{
		value = std::abs(value);
		auto [ValInt, ValFract] = hidden::SplitDouble(value, Precision);
		std::vector<std::uint32_t> Items;
		Items.push_back(ValFract);
		Items.push_back(ValInt);
		*this = tFloat(Items);
	}

	double GetValue() const { return static_cast<double>((*this)[1]) + (*this)[0] * std::pow(10, Precision); }
};

template <std::uint32_t Precision>
class tFloat<Precision, 0>
{
	static constexpr std::size_t SizeInt = 6;
	static constexpr std::size_t SizeMax = SizeInt + Precision + 1;

	std::optional<double> m_Value;

public:
	tFloat() = default;
	explicit tFloat(const std::string& values)
	{
		if (values.empty() || values.size() >= SizeMax)
			return;
		std::size_t DotPos = values.find('.');
		if (DotPos == std::string::npos || values.size() - DotPos != Precision + 1)
			return;
		m_Value = std::strtod(values.c_str(), nullptr);
	}
	explicit tFloat(double value) : m_Value(value) {}

	void clear() { m_Value.reset(); }
	bool empty() const { return !m_Value.has_value(); }

	double GetValue() const { return m_Value.value_or(0); }

	std::string ToString()
	{
		if (empty())
			return {};
		std::stringstream SStream;
		SStream.setf(std::ios::fixed);
		SStream << std::setw(Precision + 1) << std::setprecision(Precision) << m_Value.value_or(0);
		SStream.unsetf(std::ios::fixed);
		return SStream.str();
	}
};

template <> class tFloat<0, 0>; // Such an object cannot be created.

// 0 for integer part stands for any quantity of digits in this part.

using tFloat0x2 = tFloat<2, 0>; // 4.56		Its integer part is not of fixed size.
using tFloat0x3 = tFloat<3, 0>; // 4.567	Its integer part is not of fixed size.
using tFloat0x4 = tFloat<4, 0>; // 4.5736	Its integer part is not of fixed size.
using tFloat2x4 = tFloat<4, 2>; // 12.3456

using tFloat0x1 = tFloat<1, 0>; // 0.0		Altitude MTK, AXN_1.30
using tFloat5x1 = tFloat<1, 5>; // 00000.0	Altitude SiRF, GSU-7x

using tAltitude = tFloat0x1;

///////////////////////////////////////////////////////////////////////////////////////////////////
/*template <std::size_t SizeInt, std::size_t SizeFract>
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
};*/
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
/*template <typename T, std::size_t Size>
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
};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
/*template <typename TValue>
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
};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
