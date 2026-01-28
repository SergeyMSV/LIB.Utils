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
namespace hidden
{
std::pair<std::uint32_t, std::uint32_t> SplitDouble(double value, std::size_t precision);
double MakeDouble(std::int32_t valueInt, std::int32_t valueFract, std::size_t precision);
int CountDigits(std::int32_t num);
bool IsInteger(const std::string& value);
bool CheckSignedIntFixed(const std::string& value, std::size_t size);
bool CheckSignedInt(const std::string& value, std::size_t sizeMax);
bool IsChar(char ch);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
class tTypeVerified
{
	bool m_Verified = false;

public:
	tTypeVerified() = default;

	bool IsVerified() const { return m_Verified; }

protected:
	void SetVerified(bool verified) { m_Verified = verified; }
	void SetVerified() { m_Verified = true; }
};
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
struct tGNSS : public tTypeVerified
{
	tGNSS_State Value = tGNSS_State::None;

	tGNSS() = default;
	explicit tGNSS(tGNSS_State val) : Value(val) { SetVerified(); }
	explicit tGNSS(const std::string& val);

	bool IsEmpty() const { return Value == tGNSS_State::None; }

	int GetValue() const { return static_cast<int>(Value); }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class tValid : public tTypeVerified
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
	explicit tValid(bool val) : Value(val ? tValidity::Valid : tValidity::NotValid) { SetVerified(); }
	explicit tValid(const std::string& val);

	bool IsEmpty() const { return Value == tValidity::None; }

	int GetValue() const { return static_cast<int>(Value); }

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
class tIntFixed : public tTypeVerified
{
	template<std::size_t S>
	friend std::ostream& operator<<(std::ostream& out, const tIntFixed<S>& value);

	std::optional<std::int32_t> m_Value;

public:
	using value_type = std::int32_t;

	tIntFixed() = default;
	explicit tIntFixed(const std::string& value)
	{
		if (!hidden::CheckSignedIntFixed(value, Size))
			return;
		m_Value = std::strtol(value.c_str(), nullptr, 10);
		SetVerified();
	}
	explicit tIntFixed(std::int32_t value)
	{
		if (hidden::CountDigits(value) > Size)
			return;
		m_Value = value;
		SetVerified();
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
class tUnsigned : public tTypeVerified
{
	template<typename S>
	friend std::ostream& operator<<(std::ostream& out, const tUnsigned<S>& value);

	T m_Value;

public:
	using value_type = typename T::value_type;

	tUnsigned() = default;
	explicit tUnsigned(std::string value)
	{
		if (value.empty() || value[0] == '-')
			return;
		m_Value = T(value);
		SetVerified(m_Value.IsVerified());
	}
	explicit tUnsigned(value_type value)
	{
		if (value < 0)
			return;
		m_Value = T(value);
		SetVerified(m_Value.IsVerified());
	}

	bool IsEmpty() const { return m_Value.IsEmpty(); }

	static constexpr std::size_t GetSize() { return T::GetSize(); }

	value_type GetValue() const { return m_Value.GetValue(); }

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
class tFloatFixed : public tTypeVerified
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
		const std::size_t DotPos = value.find('.');
		if (DotPos == std::string::npos || value.size() - DotPos != Precision + 1)
			return;
		m_ValueInt = tValueInt(value.substr(0, DotPos));
		m_ValueFract = tValueFract(value.substr(DotPos + 1));
		CheckValues();
	}
	explicit tFloatFixed(double value)
	{
		std::pair<std::int32_t, std::int32_t> Data = hidden::SplitDouble(std::abs(value), Precision);
		if (value < 0)
			Data.first *= -1;
		m_ValueInt = tValueInt(Data.first);
		m_ValueFract = tValueFract(Data.second);
		CheckValues();
	}

	bool IsEmpty() const { return m_ValueInt.IsEmpty() || m_ValueFract.IsEmpty(); }

	static constexpr std::size_t GetSize() { return Size; }

	double GetValue() const { return hidden::MakeDouble(m_ValueInt.GetValue(), m_ValueFract.GetValue(), Precision); }

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
		SetVerified(m_ValueInt.IsVerified() && m_ValueFract.IsVerified());
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
template <std::size_t SizeMax>
class tInt : public tTypeVerified // It can consist of any quantity of digits from 1 upto Size.
{
	template <std::size_t S>
	friend std::ostream& operator<<(std::ostream& out, const tInt<S>& value);

	std::optional<std::int32_t> m_Value;

public:
	using value_type = std::int32_t;

	tInt() = default;
	explicit tInt(const std::string& value)
	{
		if (!hidden::CheckSignedInt(value, SizeMax))
			return;
		m_Value = std::strtol(value.c_str(), nullptr, 10);
		SetVerified();
	}
	explicit tInt(std::uint32_t value)
	{
		if (hidden::CountDigits(value) > SizeMax)
			return;
		m_Value = value;
		SetVerified();
	}

	bool IsEmpty() const { return !m_Value.has_value(); }

	static constexpr std::size_t GetSize() { return SizeMax; }

	std::int32_t GetValue() const { return m_Value.value_or(0); }

	std::string ToString() const
	{
		std::stringstream SStr;
		SStr << *this;
		return SStr.str();
	}
};

template <> class tInt<0>; // Such an object cannot be created.

template <std::size_t SizeMax>
std::ostream& operator<<(std::ostream& out, const tInt<SizeMax>& value)
{
	if (value.IsEmpty())
		return out;
	out << *value.m_Value;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeMax>
using tUInt = tUnsigned<tInt<SizeMax>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t Precision>
class tPrecisionFixed : public tTypeVerified
{
	static constexpr std::size_t SizeMin = 2 + Precision; // 2 stands for "0."

	using tValueFract = tUIntFixed<Precision>;

	template<typename U, std::size_t P>
	friend std::ostream& operator<<(std::ostream& out, const tPrecisionFixed<U, P>& value);

	T m_ValueInt;
	tValueFract m_ValueFract;

public:
	using value_type = double;

	tPrecisionFixed() = default;
	explicit tPrecisionFixed(std::string value)
	{
		if (value.size() < SizeMin)
			return;
		const std::size_t DotPos = value.find('.');
		if (DotPos == std::string::npos || value.size() - DotPos != Precision + 1)
			return;
		std::string ValIntStr = value.substr(0, DotPos);
		m_ValueInt = T(ValIntStr);
		m_ValueFract = tValueFract(value.substr(DotPos + 1));
		CheckValues();
	}
	explicit tPrecisionFixed(double value)
	{
		std::pair<std::int32_t, std::int32_t> Data = hidden::SplitDouble(std::abs(value), Precision);
		if (value < 0)
			Data.first *= -1;
		m_ValueInt = T(Data.first);
		m_ValueFract = tValueFract(Data.second);
		CheckValues();
	}

	bool IsEmpty() const { return m_ValueInt.IsEmpty() || m_ValueFract.IsEmpty(); }

	static constexpr std::size_t GetSize() { return SizeMin; }

	double GetValue() const { return hidden::MakeDouble(m_ValueInt.GetValue(), m_ValueFract.GetValue(), Precision); }

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
		m_ValueInt = T();
		m_ValueFract = tValueFract();
		SetVerified(m_ValueInt.IsVerified() && m_ValueFract.IsVerified());
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
template<std::size_t SizeIntMax, std::size_t Precision>
using tFloatPrecisionFixed = tPrecisionFixed<tInt<SizeIntMax>, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t SizeIntMax, std::size_t Precision>
using tUFloatPrecisionFixed = tPrecisionFixed<tUInt<SizeIntMax>, Precision>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class tUnit : public tTypeVerified
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
		if (unit.size() != 1)
			return;
		m_Unit = unit[0]; // It should be parsed in any case even if Value is null.
		m_Value = T(value);
		SetVerified(m_Unit && m_Value.IsVerified());
	}
	tUnit(TValue value, char unit)
	{
		m_Value = T(value);
		m_Unit = unit;
		SetVerified(m_Unit && m_Value.IsVerified());
	}

	bool IsEmpty() const { return m_Value.IsEmpty() || !hidden::IsChar(m_Unit); }

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
		if (!hidden::IsChar(m_Unit))
			return {};
		return std::string(1, m_Unit);
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const tUnit<T>& value)
{
	out << value.m_Value << ',';
	if (hidden::IsChar(value.m_Unit))
		out << value.m_Unit;
	return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t SizeInt, std::size_t Precision>
using tFloatFixedUnit = tUnit<tFloatFixed<SizeInt, Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t SizeInt, std::size_t Precision>
using tUFloatFixedUnit = tUnit<tUFloatFixed<SizeInt, Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t SizeIntMax, std::size_t Precision>
using tFloatPrecisionFixedUnit = tUnit<tFloatPrecisionFixed<SizeIntMax, Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t SizeIntMax, std::size_t Precision>
using tUFloatPrecisionFixedUnit = tUnit<tUFloatPrecisionFixed<SizeIntMax, Precision>>;
///////////////////////////////////////////////////////////////////////////////////////////////////
class tDate : public tTypeVerified
{
	using tValue = tUIntFixed<2>;

	friend std::ostream& operator<<(std::ostream& out, const tDate& value);

	tValue m_Day;
	tValue m_Month;
	tValue m_Year;

public:
	tDate() = default;
	explicit tDate(const std::string& value);
	tDate(std::int8_t year, std::int8_t month, std::int8_t day);
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
class tTimeBase : public tTypeVerified
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
		if (!IsValid(m_Hour.GetValue(), m_Minute.GetValue(), static_cast<std::uint8_t>(m_Second.GetValue())))
		{
			m_Hour = tValue();
			m_Minute = tValue();
			m_Second = TSecond();
			return;
		}
		SetVerified(m_Hour.IsVerified() && m_Minute.IsVerified() && m_Second.IsVerified());
	}
	tTimeBase(std::uint8_t hour, std::uint8_t minute, double second)
	{
		if (!IsValid(hour, minute, static_cast<std::uint8_t>(second)))
			return;
		m_Hour = tValue(hour);
		m_Minute = tValue(minute);
		m_Second = TSecond(static_cast<typename TSecond::value_type>(second));
		SetVerified(m_Hour.IsVerified() && m_Minute.IsVerified() && m_Second.IsVerified());
	}
	explicit tTimeBase(std::uint32_t value) // value is in seconds
	{
		m_Hour = tValue(value / 3600);
		value -= m_Hour.GetValue() * 3600;
		m_Minute = tValue(value / 60);
		value -= m_Minute.GetValue() * 60;
		m_Second = TSecond(value);
	}

	bool IsEmpty() const { return m_Hour.IsEmpty() || m_Minute.IsEmpty() || m_Second.IsEmpty(); }

	std::uint32_t GetValue() const // in seconds
	{
		return m_Hour.GetValue() * 3600 + m_Minute.GetValue() * 60 + static_cast<std::uint32_t>(m_Second.GetValue());
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
// The valid range of latitude in degrees is -90 and +90 for the southern and northern hemisphere, respectively.
// Longitude is in the range -180 and +180 specifying coordinates west and east of the Prime Meridian, respectively.For reference,
// the Equator has a latitude of 0, the North pole has a latitude of 90 north(written 90 N or +90), and the South pole has a latitude of -90.
template <std::size_t SizeDeg, std::size_t Precision>
class tGeoDegree : public tTypeVerified
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
		if (!IsValid(GetValue()))
		{
			m_Deg = tDegree();
			m_Min = tMinute();
			return;
		}
		SetVerified(m_Deg.IsVerified() && m_Min.IsVerified());
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
		SetVerified(m_Deg.IsVerified() && m_Min.IsVerified());
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
		return std::string(1, m_Negative ? SignNegative : SignPositive);
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
struct tMode : public tTypeVerified
{
	char Value = 0;

	tMode() = default;
	explicit tMode(const std::string& val);

	bool IsEmpty() const { return Value == 0; }

	char GetValue() const { return Value; }

	std::string ToString() const;
	std::string ToStringEx() const;
};

std::ostream& operator<<(std::ostream& out, const tMode& value);
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
