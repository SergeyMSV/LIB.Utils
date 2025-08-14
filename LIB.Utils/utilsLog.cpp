#include "utilsLog.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <time.h>

#ifdef LIB_UTILS_LOG

namespace utils
{
namespace log
{

void tLog::Write(bool timestamp, tColor colorText, const std::string& msg)
{
	WriteLog(timestamp, false, colorText, msg);
}

void tLog::WriteLine()
{
	WriteLog(false, true, tColor::Default, "");
}

void tLog::WriteLine(bool timestamp, tColor colorText, const std::string& msg)
{
	WriteLog(timestamp, true, colorText, msg);
}

static bool IsSymbol(char value)
{
	return value > 0x20 && value != 0x25 && value != 0x7F;
}

template <typename T>
static std::enable_if<std::is_same<T, char>::value || std::is_same<T, unsigned char>::value, std::string>::type MakeStringHex(const std::vector<T>& data, std::size_t linesMax)
{
	std::stringstream Stream;

	std::string Substr;

	std::size_t LinesCounter = 0;

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		Stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]) << ' ';

		if (!IsSymbol(data[i]))
		{
			Substr += '.';
		}
		else
		{
			Substr += data[i];
		}

		if (((i + 1) % 16) == 0)
		{
			Stream << "  " + Substr;

			Substr.clear();

			if (i < data.size() - 1)//It's not needed for the last string
			{
				Stream << '\n';
				if (linesMax && ++LinesCounter == linesMax)
				{
					const std::size_t Bytes = data.size() - (i + 1);
					std::size_t Lines = Bytes / 16;
					if (Bytes % 16)
						++Lines;

					Stream << "< " + std::to_string(Lines) + " lines are hidden >";
					return Stream.str();
				}
			}
		}
		else if (((i + 1) % 8) == 0)
		{
			Stream << "  ";
		}
	}

	const std::size_t Remains = data.size() % 16;

	if (Remains)
	{
		std::size_t Empty = (16 - Remains) * 3 + (Remains > 8 ? 2 : 4);

		for (std::size_t i = 0; i < Empty; ++i)
		{
			Stream << ' ';
		}

		Stream << Substr;
	}

	return Stream.str();
}

void tLog::WriteHex(bool timestamp, tColor colorText, const std::string& msg, const std::vector<std::uint8_t>& data)
{
	WriteHex(timestamp, colorText, msg, colorText, data);
}

void tLog::WriteHex(bool timestamp, tColor colorMsg, const std::string& msg, tColor colorData, const std::vector<std::uint8_t>& data, std::size_t dataLinesMax)
{
	WriteLog(timestamp, true, colorMsg, msg);
	WriteLog(false, true, colorData, MakeStringHex(data, dataLinesMax));
}

void tLog::WriteHex(bool timestamp, tColor colorMsg, const std::string& msg, tColor colorData, const std::vector<std::uint8_t>& data)
{
	WriteHex(timestamp, colorMsg, msg, colorData, data, 0);
}

void tLog::WriteLog(bool timestamp, bool endl, tColor colorText, const std::string& msg)
{
	std::lock_guard<std::mutex> Lock(m_Mtx);

	std::stringstream Stream;

	if (timestamp)
	{
		const auto TimeNow = std::chrono::system_clock::now();
#ifdef LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		const auto Time_us = std::chrono::time_point_cast<std::chrono::microseconds>(TimeNow);
		const auto TimeFract = static_cast<unsigned int>(Time_us.time_since_epoch().count() % 1000000);
		const int Digits = 6;
#else // LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		const auto Time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(TimeNow);
		const auto TimeFract = static_cast<unsigned int>(Time_ms.time_since_epoch().count() % 1000);
		const int Digits = 3;
#endif // LIB_UTILS_LOG_TIMESTAMP_MICROSECONDS
		time_t Time = std::chrono::system_clock::to_time_t(TimeNow);

		tm TmBuf;
#ifdef _WIN32
		localtime_s(&TmBuf, &Time);
#else // _WIN32
		localtime_r(&Time, &TmBuf);
#endif // _WIN32
		Stream << std::put_time(&TmBuf, "%T") << '.';
		Stream << std::setfill('0') << std::setw(Digits) << TimeFract << ' ';

		const std::string Label = GetLabel();
		if (!Label.empty())
			Stream << std::setfill(' ') << std::setw(4) << Label << ' ';
	}

#ifdef LIB_UTILS_LOG_COLOR
	Stream << "\x1b[";

	switch (colorText)
	{
	case tColor::Black: Stream << "30"; break;
	case tColor::Red: Stream << "31"; break;
	case tColor::Green: Stream << "32"; break;
	case tColor::Yellow: Stream << "33"; break;
	case tColor::Blue: Stream << "34"; break;
	case tColor::Magenta: Stream << "35"; break;
	case tColor::Cyan: Stream << "36"; break;
	case tColor::White: Stream << "37"; break;
	case tColor::Default: Stream << "39"; break;
	case tColor::LightGray: Stream << "90"; break;
	case tColor::LightRed: Stream << "91"; break;
	case tColor::LightGreen: Stream << "92"; break;
	case tColor::LightYellow: Stream << "93"; break;
	case tColor::LightBlue: Stream << "94"; break;
	case tColor::LightMagenta: Stream << "95"; break;
	case tColor::LightCyan: Stream << "96"; break;
	case tColor::LightWhite: Stream << "97"; break;
	default: Stream << "39"; break;
	}

	Stream << "m" + msg + "\x1b[0m";
#else // LIB_UTILS_LOG_COLOR
	Stream << msg;
#endif // LIB_UTILS_LOG_COLOR

	if (endl)
		Stream << '\n';

	WriteLog(Stream.str());
}

}
}

#endif//LIB_UTILS_LOG
