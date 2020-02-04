///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketNMEAType.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2020 01 27  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"

#include <cmath>
#include <cstdio>//DEPRECATED
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>

#include <iomanip>
#include <sstream>

namespace utils
{
	namespace packet_NMEA
	{
		namespace Type
		{
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tGNSS
{
	enum class tGNSS_State : tUInt8//It's like bitfield
	{
		UNKNOWN = 0,
		GPS = 1,    //0000'0001
		GLONASS,    //0000'0010
		GPS_GLONASS,//0000'0011
	};

	tGNSS_State Value = tGNSS_State::UNKNOWN;

	tGNSS() = default;
	explicit tGNSS(tGNSS_State val) :Value(val) {}
	explicit tGNSS(const std::string& val);

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tValid
{
	bool Value = false;
	bool Absent = true;

	tValid() = default;
	explicit tValid(bool val) :Value(val), Absent(false) {}
	explicit tValid(const std::string& val);

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tDate
{
	tUInt8 Year = 0;
	tUInt8 Month = 0;
	tUInt8 Day = 0;
	bool Absent = true;

	tDate() = default;//C++11
	tDate(tUInt8 year, tUInt8 month, tUInt8 day);
	explicit tDate(const std::string& val);	

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <int Size>
struct tTime
{
	tUInt8 Hour = 0;
	tUInt8 Minute = 0;
	double Second = 0;
	bool Absent = true;

	static_assert(Size >= 6, "tTime: Size");//C++11

	tTime() = default;
	tTime(tUInt8 hour, tUInt8 minute, double second) :Hour(hour), Minute(minute), Second(second), Absent(false) {}
	explicit tTime(const std::string& val)
	{
		if (val.size() == Size)
		{
			Absent = false;

			char Data[3]{};//C++11

			Data[0] = val[0];
			Data[1] = val[1];

			Hour = static_cast<tUInt8>(std::strtoul(Data, 0, 10));

			Data[0] = val[2];
			Data[1] = val[3];

			Minute = static_cast<tUInt8>(std::strtoul(Data, 0, 10));

			Second = std::strtod(val.c_str() + 4, 0);
		}
	}

	std::string ToString() const
	{
		if (!Absent && Hour < 24 && Minute < 60 && Second < 60)
		{
			std::stringstream Stream;

			Stream << std::setfill('0');
			Stream << std::setw(2) << static_cast<int>(Hour);
			Stream << std::setw(2) << static_cast<int>(Minute);

			int SizeFract = Size - 7;//sizeof(hhmmss.)=7

			if (SizeFract > 0)
			{
				Stream.setf(std::ios::fixed);

				Stream << std::setw(2 + SizeFract + 1) << std::setprecision(SizeFract) << Second;
			}
			else
			{
				Stream << std::setw(2) << static_cast<int>(Second);
			}

			return Stream.str();
		}

		return "";
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
struct tLatitude
{
	double Value = 0;
	bool Absent = true;

	tLatitude() = default;
	explicit tLatitude(double val) :Value(val), Absent(false) {}
	tLatitude(const std::string& val, const std::string& valSign)
	{
		static_assert(Size == 9 || Size == 11, "tLatitude: Size");//C++11

		if (val.size() == Size && valSign.size() == 1)
		{
			Absent = false;

			char Data[3]{};

			std::strncpy(Data, val.c_str(), sizeof(Data) - 1);

			Value = std::strtod(Data, 0);

			double Rest = std::strtod(val.c_str() + 2, 0);

			Value += Rest / 60;

			if (valSign[0] == 'S')
			{
				Value = -Value;
			}
		}
	}

	std::string ToStringValue() const
	{
		if (Absent) return "";

		double ValueAbs = std::abs(Value);

		tUInt8 Deg = static_cast<tUInt8>(ValueAbs);
		double Min = (ValueAbs - Deg) * 60;

		char Str[Size + 1]{};

		if (Deg < 100)
		{
			switch (Size)
			{
			case 9: std::sprintf(Str, "%02d%07.4f", Deg, Min); break;
			case 11: std::sprintf(Str, "%02d%09.6f", Deg, Min); break;
			}
		}

		return Str;
	}

	std::string ToStringHemisphere() const
	{
		if (Absent) return "";

		return Value < 0 ? "S" : "N";
	}

	std::string ToString() const
	{
		return ToStringValue() + ',' + ToStringHemisphere();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
struct tLongitude
{
	double Value = 0;
	bool Absent = true;

	tLongitude() = default;
	explicit tLongitude(double val) :Value(val), Absent(false) { }
	tLongitude(const std::string& val, const std::string& valSign)
	{
		static_assert(Size == 10 || Size == 12, "tLongitude: Size");//C++11

		if (val.size() == Size && valSign.size() == 1)
		{
			Absent = false;

			char Data[4]{};

			std::strncpy(Data, val.c_str(), sizeof(Data) - 1);

			Value = std::strtod(Data, 0);

			double Rest = std::strtod(val.c_str() + 3, 0);

			Value += Rest / 60;

			if (valSign[0] == 'W')
			{
				Value = -Value;
			}
		}
	}

	std::string ToStringValue() const
	{
		if (Absent) return "";

		double ValueAbs = std::abs(Value);

		tUInt16 Deg = static_cast<tUInt16>(ValueAbs);
		double Min = (ValueAbs - Deg) * 60;

		char Str[Size + 1]{};

		if (Deg < 1000)
		{
			switch (Size)
			{
			case 10: std::sprintf(Str, "%03d%07.4f", Deg, Min); break;
			case 12: std::sprintf(Str, "%03d%09.6f", Deg, Min); break;
			}
		}

		return Str;
	}

	std::string ToStringHemisphere() const
	{
		if (Absent) return "";

		return Value < 0 ? "W" : "E";
	}

	std::string ToString() const
	{
		return ToStringValue() + ',' + ToStringHemisphere();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt, std::size_t SizeFract>
struct tFloat
{
	double Value = 0;
	bool Absent = true;

private:
	static const std::size_t Size = SizeInt + SizeFract + 1;

public:
	tFloat() = default;
	explicit tFloat(double val) :Value(val), Absent(false) {}
	tFloat(const std::string& val)
	{
		if (val.size() == Size)
		{
			Absent = false;

			Value = std::strtod(val.c_str(), 0);
		}
	}

	std::string ToString() const
	{
		if (Absent) return "";

		const char StrFormat[] = { '%','0', static_cast<char>(0x30 + SizeInt + SizeFract) , '.', static_cast<char>(0x30 + SizeFract), 'f', 0 };

		char Str[Size + 1]{};

		std::sprintf(Str, StrFormat, Value);

		return Str;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeFract>
struct tFloat<0, SizeFract>
{
	double Value = 0;
	bool Absent = true;

private:
	static const std::size_t SizeInt = 6;
	static const std::size_t SizeMax = SizeInt + SizeFract + 1;

public:
	tFloat() = default;
	explicit tFloat(double val) :Value(val), Absent(false) {}
	tFloat(const std::string& val)
	{
		if (val.size() > 0 && val.size() < SizeMax)
		{
			Absent = false;

			Value = std::strtod(val.c_str(), 0);
		}
	}

	std::string ToString() const
	{
		if (Absent) return "";

		const char StrFormat[] = { '%', '.', static_cast<char>(0x30 + SizeFract), 'f', 0 };

		char Str[SizeMax + 1]{};

		std::sprintf(Str, StrFormat, Value);

		return Str;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <> struct tFloat<0, 0>;//Fractional part is just of max length (6 chars), therefore this specialisation makes no sense.
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t SizeInt> struct tFloat<SizeInt, 0>;
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tPositioning
{
	enum class tPositioning_State : tUInt8
	{
		UNKNOWN = 0,
		Autonomous,
		Differential,
		Estimated,
		ManualInput,
		Simulator,
		DataNotValid,
	};

	tPositioning_State Value = tPositioning_State::UNKNOWN;

	tPositioning() = default;
	explicit tPositioning(tPositioning_State val) :Value(val) {}
	explicit tPositioning(const std::string& val);

	std::string ToString() const;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <std::size_t Size>
struct tUInt
{
	tUInt32 Value = 0;
	bool Absent = true;

public:
	tUInt() = default;
	explicit tUInt(tUInt32 val) :Value(val), Absent(false) {}
	tUInt(const std::string& val)
	{
		if (val.size() == Size)
		{
			Absent = false;

			Value = std::strtol(val.c_str(), 0, 10);
		}
	}

	std::string ToString() const
	{
		if (Absent) return "";

		const char StrFormat[] = { '%', '0', static_cast<char>(0x30 + Size), 'd', 0 };

		char Str[Size + 1]{};

		std::sprintf(Str, StrFormat, Value);

		return Str;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <>
struct tUInt<0>
{
	tUInt32 Value = 0;
	bool Absent = true;

public:
	tUInt() = default;
	explicit tUInt(tUInt32 val) :Value(val), Absent(false) {}
	tUInt(const std::string& val)
	{
		if (val.size() > 0)
		{
			Absent = false;

			Value = std::strtol(val.c_str(), 0, 10);
		}
	}

	std::string ToString() const
	{
		if (Absent) return "";

		char Str[20]{};

		std::sprintf(Str, "%d", Value);

		return Str;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}
