///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAType
// 2020-01-27, 2026-01-17 refact.
// C++17
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <algorithm>
#include <iomanip>
#include <ostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

namespace utils
{
namespace packet
{
namespace nmea
{
namespace type
{
///////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<std::uint32_t, std::uint32_t> SplitDouble(double value, std::size_t precision);
double MakeDouble(std::int32_t valueInt, std::int32_t valueFract, std::size_t precision);
///////////////////////////////////////////////////////////////////////////////////////////////////
enum class tGNSS_State : std::uint8_t // It's like bitfield.
{
	None = 0,
	GPS = 1,			// GP	0000'0001
	GLONASS,			// GL	0000'0010
	//Galileo,			// GA
	//QZSS				// GQ
	//BeiDou			// BD
	GlobalNavigation,	// GN	0000'0011		If a solution is obtained and combined from multiple systems.
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tGNSS
{
	tGNSS_State Value = tGNSS_State::None;

	tGNSS() = default;
	explicit tGNSS(tGNSS_State val) : Value(val) {}
	explicit tGNSS(const std::string& val);

	bool IsEmpty() const { return Value == tGNSS_State::None; }

	int GetValue() const { return static_cast<int>(Value); }

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

	int GetValue() const { return static_cast<int>(Value); }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
class tIntFixed
{
	template<std::size_t S>
	friend std::ostream& operator<<(std::ostream& out, const tIntFixed<S>& value);

	std::optional<std::int32_t> m_Value;

public:
	using value_type = std::int32_t;

	tIntFixed() = default;
	explicit tIntFixed(const std::string& value)
	{
		if (value.size() != Size && (value.size() != Size + 1 || value[0] != '-'))
			return;
		for (std::size_t i = value.size() - Size; i < value.size(); ++i)
		{
			if (!std::isdigit(static_cast<unsigned char>(value[i])))
				return;
		}
		m_Value = std::strtol(value.c_str(), nullptr, 10);
	}
	explicit tIntFixed(std::int32_t value)
	{
		if (CountDigits(value) > Size)
			return;
		m_Value = value;
	}

	bool IsEmpty() const { return !m_Value.has_value(); }

	static constexpr std::size_t GetSize() { return Size; }

	std::int32_t GetValue() const { return m_Value.value_or(0); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}

protected:
	static int CountDigits(std::int32_t num)
	{
		int Count = 0;
		while (num != 0)
		{
			num /= 10;
			++Count;
		}
		return Count;
	}
};

template<std::size_t Size>
std::ostream& operator<<(std::ostream& out, const tIntFixed<Size>& value)
{
	if (value.IsEmpty())
		return out;
	std::int32_t Val = value.GetValue();
	if (Val < 0)
		out << '-';
	out << std::setfill('0') << std::setw(Size) << std::abs(Val);
	//out << std::setfill('0') << std::setw(Size) << value.GetValue(); // 0000-1.34
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class tUnsigned
{
	template<typename S>
	friend std::ostream& operator<<(std::ostream& out, const tUnsigned<S>& value);

	T m_Value;

public:
	using TValue = typename T::value_type;

	tUnsigned() = default;
	explicit tUnsigned(std::string value)
	{
		if (value.size() != T::GetSize())
			return;
		m_Value = T(value);
	}
	explicit tUnsigned(TValue value)
	{
		if (value < 0)
			return;
		m_Value = T(value);
	}

	bool IsEmpty() const { return m_Value.IsEmpty(); }

	static constexpr std::size_t GetSize() { return T::GetSize(); }

	TValue GetValue() const { return m_Value.GetValue(); }

	std::string ToString() const { return m_Value.ToString(); }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const tUnsigned<T>& value)
{
	out << value.m_Value;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
using tUIntFixed = tUnsigned<tIntFixed<Size>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt, std::size_t Precision>
class tFloatFixed
{
	static constexpr std::size_t Size = SizeInt + 1 + Precision;

	using tValueInt = tIntFixed<SizeInt>;
	using tValueFract = tUIntFixed<Precision>; // Fractional

	template<std::size_t S, std::size_t P>
	friend std::ostream& operator<<(std::ostream& out, const tFloatFixed<S, P>& value);

	tValueInt m_ValueInt;
	tValueFract m_ValueFract;

public:
	using value_type = double;

	tFloatFixed() = default;
	explicit tFloatFixed(std::string value)
	{
		if (value.size() != Size && (value.size() != Size + 1 || value[0] != '-'))
			return;
		const std::size_t DotPos = value.find('.');
		if (DotPos == std::string::npos || value.size() - DotPos != Precision + 1)
			return;
		m_ValueInt = tValueInt(value.substr(0, DotPos));
		m_ValueFract = tValueFract(value.substr(DotPos + 1));
		CheckValues();
	}
	explicit tFloatFixed(double value)
	{
		std::pair<std::int32_t, std::int32_t> Data = SplitDouble(std::abs(value), Precision);
		if (value < 0)
			Data.first *= -1;
		m_ValueInt = tValueInt(Data.first);
		m_ValueFract = tValueFract(Data.second);
		CheckValues();
	}

	bool IsEmpty() const { return m_ValueInt.IsEmpty() || m_ValueFract.IsEmpty(); }

	static constexpr std::size_t GetSize() { return Size; }

	double GetValue() const { return MakeDouble(m_ValueInt.GetValue(), m_ValueFract.GetValue(), Precision); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}

private:
	void CheckValues()
	{
		if (!IsEmpty())
			return;
		m_ValueInt = tValueInt();
		m_ValueFract = tValueFract();
	}
};

template <std::size_t SizeInt, std::size_t Precision>
std::ostream& operator<<(std::ostream& out, const tFloatFixed<SizeInt, Precision>& value)
{
	if (value.IsEmpty())
		return out;
	out << value.m_ValueInt << '.' << value.m_ValueFract;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt, std::size_t Precision>
using tUFloatFixed = tUnsigned<tFloatFixed<SizeInt, Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t Precision>
class tPrecisionFixed
{
	static constexpr std::size_t SizeMin = 2 + Precision; // 2 stands for "0."

	using tValueFract = tUIntFixed<Precision>;

	template<typename U, std::size_t P>
	friend std::ostream& operator<<(std::ostream& out, const tPrecisionFixed<U, P>& value);

	std::int32_t m_ValueInt = 0;
	tValueFract m_ValueFract;

public:
	using value_type = double;

	tPrecisionFixed() = default;
	explicit tPrecisionFixed(std::string value)
	{
		if (value.size() < SizeMin)
			return;
		if (std::is_unsigned<T>::value && value[0] == '-')
			return;
		const std::size_t DotPos = value.find('.');
		if (DotPos == std::string::npos || value.size() - DotPos != Precision + 1)
			return;
		std::string ValIntStr = value.substr(0, DotPos);
		m_ValueInt = std::strtol(ValIntStr.c_str(), nullptr, 10);
		m_ValueFract = tValueFract(value.substr(DotPos + 1));
	}
	explicit tPrecisionFixed(double value)
	{
		if (std::is_unsigned<T>::value && value < 0)
			return;
		std::pair<std::int32_t, std::int32_t> Data = SplitDouble(std::abs(value), Precision);
		if (value < 0)
			Data.first *= -1;
		m_ValueInt = Data.first;
		m_ValueFract = tValueFract(Data.second);
	}

	bool IsEmpty() const { return m_ValueFract.IsEmpty(); }

	static constexpr std::size_t GetSize() { return SizeMin; }
	static std::size_t CheckSize(std::size_t size) { return size <= SizeMin; }

	double GetValue() const { return MakeDouble(m_ValueInt, m_ValueFract.GetValue(), Precision); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}
};

template<typename T, std::size_t Precision>
std::ostream& operator<<(std::ostream& out, const tPrecisionFixed<T, Precision>& value)
{
	if (value.IsEmpty())
		return out;
	out << value.m_ValueInt << '.' << value.m_ValueFract;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t Precision>
using tFloatPrecisionFixed = tPrecisionFixed<std::int32_t, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t Precision>
using tUFloatPrecisionFixed = tPrecisionFixed<std::uint32_t, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class tUnit
{
	template<typename U>
	friend std::ostream& operator<<(std::ostream& out, const tUnit<U>& value);

	T m_Value;
	char m_Unit = 0;

public:
	using TValue = typename T::value_type;

	tUnit() = default;
	tUnit(const std::string& value, const std::string& unit)
	{
		if (T::CheckSize(value.size()) || unit.size() != 1)
			return;
		m_Unit = unit[0];
		m_Value = T(value);
	}
	tUnit(TValue value, char unit)
	{
		m_Value = T(value);
		m_Unit = unit;
	}

	bool IsEmpty() const { return m_Value.IsEmpty() || !m_Unit; }

	static constexpr std::size_t GetSize() { return 0; }

	double GetValue() const { return m_Value.GetValue(); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}

	std::string ToStringValue() const
	{
		std::stringstream SStr;
		SStr << m_Value;
		return SStr.str();
	}

	std::string ToStringUnit() const
	{
		if (IsEmpty())
			return {};
		return m_Unit;
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const tUnit<T>& value)
{
	out << value.m_Value << ',';
	if (value.m_Unit)
		out << value.m_Unit;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t Precision>
using tFloatPrecisionFixedUnit = tUnit<tFloatPrecisionFixed<Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t Precision>
using tUFloatPrecisionFixedUnit = tUnit<tUFloatPrecisionFixed<Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
class tDate
{
	using tValue = tUIntFixed<2>;

	friend std::ostream& operator<<(std::ostream& out, const tDate& value);

	tValue m_Day;
	tValue m_Month;
	tValue m_Year;

public:
	tDate() = default;
	tDate(const std::string& value);
	explicit tDate(std::int8_t year, std::int8_t month, std::int8_t day);
	explicit tDate(std::time_t value);

	bool IsEmpty() const { return m_Day.IsEmpty() || m_Month.IsEmpty() || m_Year.IsEmpty(); }

	std::time_t GetValue() const;

	std::uint8_t GetYear() const { return static_cast<std::uint8_t>(m_Day.GetValue()); }
	std::uint8_t GetMonth() const { return static_cast<std::uint8_t>(m_Month.GetValue()); }
	std::uint8_t GetDay() const { return static_cast<std::uint8_t>(m_Year.GetValue()); }

	std::string ToString() const;

private:
	static bool IsValid(std::uint8_t year, std::uint8_t month, std::uint8_t day) { return year < 100 && month > 0 && month <= 12 && day > 0 && day <= 31; }
};

std::ostream& operator<<(std::ostream& out, const tDate& value);
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TSecond>
class tTimeBase
{
	using tValue = tUIntFixed<2>;

	template<typename S>
	friend std::ostream& operator<<(std::ostream& out, const tTimeBase<S>& value);

	tValue m_Hour;
	tValue m_Minute;
	TSecond m_Second;

public:
	tTimeBase() = default;
	explicit tTimeBase(const std::string& value)
	{
		if (value.size() != 4 + TSecond::GetSize())
			return;
		m_Hour = tValue(value.substr(0, 2));
		m_Minute = tValue(value.substr(2, 2));
		m_Second = TSecond(value.substr(4));
		if (IsValid(m_Hour.GetValue(), m_Minute.GetValue(), static_cast<std::uint8_t>(m_Second.GetValue())))
			return;
		m_Hour = tValue();
		m_Minute = tValue();
		m_Second = TSecond();
	}
	explicit tTimeBase(std::uint8_t hour, std::uint8_t minute, double second)
	{
		if (!IsValid(hour, minute, static_cast<std::uint8_t>(second)))
			return;
		m_Hour = tValue(hour);
		m_Minute = tValue(minute);
		m_Second = TSecond(second);
	}
	explicit tTimeBase(std::time_t value)
	{
		m_Hour = tValue(value / 3600);
		value -= m_Hour.GetValue() * 3600;
		m_Minute = tValue(value / 60);
		value -= m_Minute.GetValue() * 60;
		m_Second = TSecond(value);
	}

	bool IsEmpty() const { return m_Hour.IsEmpty() || m_Minute.IsEmpty() || m_Second.IsEmpty(); }

	std::time_t GetValue() const
	{
		return m_Hour.GetValue() * 3600 + m_Minute.GetValue() * 60 + static_cast<std::time_t>(m_Second.GetValue());
	}

	std::uint8_t GetHour() const { return static_cast<std::uint8_t>(m_Hour.GetValue()); }
	std::uint8_t GetMinute() const { return static_cast<std::uint8_t>(m_Minute.GetValue()); }
	double GetSecond() const { return m_Second.GetValue(); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}

private:
	static bool IsValid(std::uint8_t hour, std::uint8_t minute, std::uint8_t second) { return hour < 24 && minute < 60 && second < 60; }
};

template <typename TSecond>
std::ostream& operator<<(std::ostream& out, const tTimeBase<TSecond>& value)
{
	if (value.IsEmpty())
		return out;
	out << value.m_Hour << value.m_Minute << value.m_Second;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t Precision>
struct tTimeHelper
{
	using type = tTimeBase<tUFloatFixed<2, Precision>>;
};

template<>
struct tTimeHelper<0>
{
	using type = tTimeBase<tUIntFixed<2>>;
};

template<std::size_t Precision>
using tTime = typename tTimeHelper<Precision>::type;
///////////////////////////////////////////////////////////////////////////////////////////////////
// The valid range of latitude in degrees is - 90 and +90 for the southern and northern hemisphere, respectively.
// Longitude is in the range - 180 and +180 specifying coordinates west and east of the Prime Meridian, respectively.For reference,
// the Equator has a latitude of 0°, the North pole has a latitude of 90° north(written 90° N or +90°), and the South pole has a latitude of - 90°.
template <std::size_t SizeDeg, std::size_t Precision>
class tGeoDegree
{
	using tDegree = tUIntFixed<SizeDeg>;
	using tMinute = tUFloatFixed<2, Precision>;

	static constexpr std::size_t Size = tDegree::GetSize() + tMinute::GetSize();
	static constexpr int MaxAbs = SizeDeg == 3 ? 180 : 90;

	template<std::size_t S, std::size_t P>
	friend std::ostream& operator<<(std::ostream& out, const tGeoDegree<S, P>& value);

	tDegree m_Deg;
	tMinute m_Min;
	bool m_Negative = false;

public:
	static constexpr char SignNegative = SizeDeg == 3 ? 'W' : 'S';
	static constexpr char SignPositive = SizeDeg == 3 ? 'E' : 'N';

	tGeoDegree() = default;
	tGeoDegree(const std::string& value, const std::string& sign)
	{
		if (value.size() != Size || sign.size() != 1 || (sign[0] != SignNegative && sign[0] != SignPositive))
			return;
		m_Negative = sign[0] == SignNegative;
		m_Deg = tDegree(value.substr(0, SizeDeg));
		m_Min = tMinute(value.substr(SizeDeg));
		if (IsValid(GetValue()))
			return;
		m_Deg = tDegree();
		m_Min = tMinute();
	}
	explicit tGeoDegree(double degree)
	{
		if (!IsValid(degree))
			return;
		m_Negative = degree < 0;
		degree = std::abs(degree);
		m_Deg = tDegree(static_cast<std::int32_t>(degree));
		degree -= static_cast<std::int32_t>(degree);
		degree *= 60;
		m_Min = tMinute(degree);
	}

	bool IsEmpty() const { return m_Deg.IsEmpty() || m_Min.IsEmpty(); }

	static constexpr std::size_t GetSize() { return Size; }

	double GetValue() const { return m_Deg.GetValue() + m_Min.GetValue() / 60; }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}

	std::string ToStringValue() const
	{
		std::stringstream SStr;
		SStr << m_Deg << m_Min;
		return SStr.str();
	}

	std::string ToStringHemisphere() const
	{
		if (IsEmpty())
			return {};
		return m_Negative ? SignNegative : SignPositive;
	}

private:
	static bool IsValid(double value) { return value >= -MaxAbs && value <= MaxAbs; }
};

template <std::size_t SizeDeg, std::size_t Precision>
std::ostream& operator<<(std::ostream& out, const tGeoDegree<SizeDeg, Precision>& value)
{
	using tDeg = tGeoDegree<SizeDeg, Precision>;

	if (value.IsEmpty())
	{
		out << ',';
		return out;
	}
	out << value.m_Deg << value.m_Min << ',' << (value.m_Negative ? tDeg::SignNegative : tDeg::SignPositive);
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Precision>
using tLatitude = tGeoDegree<2, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Precision>
using tLongitude = tGeoDegree<3, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////




/*namespace hidden
{

struct tNumberFixedItem
{
	std::uint32_t Value;
	std::uint32_t Size;
	bool Fractional;

	tNumberFixedItem() = default;
	tNumberFixedItem(std::uint32_t value, std::uint32_t size, bool fract) : Value(value), Size(size), Fractional(fract) {}
};

std::ostream& operator<<(std::ostream& out, const tNumberFixedItem& value);

template <std::uint32_t... ints>
class tNumberFixed
{
	using tVectorParts = std::vector<tNumberFixedItem>;

	tVectorParts m_Parts;

public:
	tNumberFixed() = default;
	explicit tNumberFixed(const std::string& value)
	{
		if (value.size() != GetValuesStringSize())
			return;
		int PartIndex = -1;
		std::size_t Position = value.size();
		if (((!Parse(value, ++PartIndex, ints, Position)) || ...))
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
	bool Parse(const std::string& values, int partIndex, int size, std::size_t& position)
	{
		if (!partIndex)
			return ParseFract(values, size, position);

		if (position < size)
			return false;
		position -= size;
		m_Parts.emplace_back(strtoul(values.substr(position, size).c_str(), nullptr, 10), size, false);
		return true;
	}

	bool ParseFract(const std::string& values, int size, std::size_t& position)
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
		position = DotPos;
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

std::pair<std::uint32_t, std::uint32_t> SplitDouble(double value, std::uint32_t precision);
int CountDigits(std::uint32_t num);
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
			std::uint32_t SecFract = static_cast<std::uint32_t>((second - Sec) * std::pow(10, Precision));
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

	bool m_Negative = false;

	explicit tGeoDegree(std::vector<std::uint32_t>& values, bool negative) : tBase(values), m_Negative(negative) {}

public:
	tGeoDegree() = default;
	explicit tGeoDegree(const std::string& values, const std::string& sign)
		:tBase(values)
	{
		if (!IsValid(GetDegree()) || sign.size() != 1 || (sign[0] != Negative && sign[0] != Positive))
		{
			this->clear();
			m_Negative = false;
			return;
		}
		m_Negative = sign[0] == Negative;
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
		return GetDegree() * (m_Negative ? -1 : 1);
	}

	std::string ToString() const
	{
		if (this->empty())
			return ","; // [TBD] 0000.00,N or ,N ?
		return tBase::ToString() + ',' + (m_Negative ? Negative : Positive);
	}

	std::string ToStringValue() const
	{
		return tBase::ToString();
	}

	std::string ToStringHemisphere() const
	{
		if (this->empty())
			return {};
		return std::string(1, m_Negative ? Negative : Positive);
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
/*template <std::size_t Size>
class tUIntFixed : public hidden::tNumberFixed<0, Size>
{
	using tBase = hidden::tNumberFixed<0, Size>;

	explicit tUIntFixed(std::vector<std::uint32_t>& values) : tBase(values) {}

public:
	tUIntFixed() = default;
	explicit tUIntFixed(const std::string& values) : tBase(values) {}
	explicit tUIntFixed(std::uint32_t value)
	{
		if (hidden::ÑountDigits(value) > Size)
			return;
		std::vector<std::uint32_t> Items;
		Items.push_back(value);
		*this = tUIntFixed(Items);
	}

	std::uint8_t GetValue() const { return (*this)[1]; }
};

using tUIntFixed1 = tUIntFixed<1>; // 0
using tUIntFixed2 = tUIntFixed<2>; // 00
using tUIntFixed3 = tUIntFixed<3>; // 000
using tUIntFixed4 = tUIntFixed<4>; // 0000

template <std::size_t SizeMax>
class tUInt // It can consist of any quantity of digits from 1 upto Size.
{
	std::optional<std::uint32_t> m_Value;

public:
	tUInt() = default;
	explicit tUInt(const std::string& values)
	{
		if (values.empty() || values.size() > SizeMax)
			return;
		m_Value = std::strtol(values.c_str(), nullptr, 10);
	}
	explicit tUInt(std::uint32_t value)
	{
		if (hidden::ÑountDigits(value) > SizeMax)
			return;
		m_Value = value;
	}

	void clear() { m_Value.reset(); }
	bool empty() const { return !m_Value.has_value(); }

	std::uint32_t GetValue() const { return m_Value.value_or(0); }

	std::string ToString() const
	{
		if (empty())
			return {};
		return std::to_string(GetValue());
	}
};

template <> class tUInt<0>; // Such an object cannot be created.

using tUInt1 = tUInt<1>; // from 0 upto 9
using tUInt2 = tUInt<2>; // from 0 upto 99
using tUInt3 = tUInt<3>; // from 0 upto 999
using tUInt4 = tUInt<4>; // from 0 upto 9999*/
///////////////////////////////////////////////////////////////////////////////////////////////////
/*template <std::uint32_t Precision, std::size_t Size>
class tFloatFixed
{
	using tBase = hidden::tNumberFixed<Precision, Size>;

	tBase m_Value;
	bool m_Negative = false;

	explicit tFloatFixed(std::vector<std::uint32_t>& values)
	{
		m_Value = tBase(values);
	}

public:
	tFloatFixed() = default;
	explicit tFloatFixed(const std::string& value)
	{
		// [TBD] sign
		m_Value = tBase(value);
	}

	explicit tFloatFixed(double value)
	{
		value = std::abs(value);
		auto [ValInt, ValFract] = hidden::SplitDouble(value, Precision);
		if (hidden::CountDigits(ValInt) > Size)
			return;
		std::vector<std::uint32_t> Items;
		Items.push_back(ValFract);
		Items.push_back(ValInt);
		*this = tFloatFixed(Items);
	}

	double GetValue() const { return static_cast<double>((*this)[1]) + (*this)[0] * std::pow(10, Precision); }

	std::string ToString() const
	{
		if (m_Value.empty())
			return {};
		std::string Str;
		if (m_Negative)
			Str += '-';
		Str += m_Value.ToString();
		return Str;
	}
};

template <std::uint32_t Precision>
class tFloatFixed<Precision, 0> // Its integer part is not of fixed size.
{
	static constexpr std::size_t SizeInt = 6;
	static constexpr std::size_t SizeMax = SizeInt + Precision + 1;

	std::optional<double> m_Value;

public:
	tFloatFixed() = default;
	explicit tFloatFixed(const std::string& value)
	{
		if (value.empty() || value.size() >= SizeMax)
			return;
		std::size_t DotPos = value.find('.');
		if (DotPos == std::string::npos || value.size() - DotPos != Precision + 1)
			return;
		m_Value = std::strtod(value.c_str(), nullptr);
	}
	explicit tFloatFixed(double value) : m_Value(value) {}

	void clear() { m_Value.reset(); }
	bool empty() const { return !m_Value.has_value(); }

	double GetValue() const { return m_Value.value_or(0); }

	std::string ToString() const
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

template <> class tFloatFixed<0, 0>; // Such an object cannot be created.

// 0 for integer part stands for any quantity of digits in this part.

using tFloatFixed2 = tFloatFixed<2, 0>; // 4.56		Its integer part is not of fixed size.
using tFloatFixed3 = tFloatFixed<3, 0>; // 4.567	Its integer part is not of fixed size.
using tFloatFixed4 = tFloatFixed<4, 0>; // 4.5736	Its integer part is not of fixed size.

using tFloatFixed2x4 = tFloatFixed<4, 2>; // 12.3456

using tFloatFixed1 = tFloatFixed<1, 0>; // 0.0		Altitude MTK, AXN_1.30
using tFloatFixed5x1 = tFloatFixed<1, 5>; // 00000.0	Altitude SiRF, GSU-7x

//using tAltitude = tFloatFixed1;

///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::uint32_t Precision, std::size_t Size>
class tFloatUnit
{
	using tValue = tFloatFixed<Precision, Size>;

public:
	tValue m_Value; // optional inside
	char m_Unit = 0;

	tFloatUnit() = default;
	tFloatUnit(double value, char unit) :m_Value(value), m_Unit(unit) {}
	explicit tFloatUnit(const std::string& value, const std::string& unit)
		:m_Value(value)
	{
		if (unit.size() == 1)
			m_Unit = unit[0];
	}

	std::string ToStringValue() const { return m_Value.ToString(); }
	std::string ToStringUnit() const { return std::string(1, m_Unit); }
	std::string ToString() const
	{
		return ToStringValue() + ',' + ToStringUnit();
	}
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
