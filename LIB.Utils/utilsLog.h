///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsLog
// 2016-05-16 (before) - 2026-11-06
// C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <mutex>
#include <string>
#include <vector>

#include <cstdint>

namespace utils
{
namespace log
{

enum class tColor
{
	None,
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	Default,
	LightGray,
	LightRed,
	LightGreen,
	LightYellow,
	LightBlue,
	LightMagenta,
	LightCyan,
	LightWhite,
};

#ifdef LIB_UTILS_LOG
#define LIB_UTILS_IMPL ;
#else // LIB_UTILS_LOG
#define LIB_UTILS_IMPL {}
#endif // LIB_UTILS_LOG

class tLog
{
	mutable std::mutex m_Mtx;

public:
	tLog() = default;
	virtual ~tLog() {}

	void Write(bool timestamp, const std::string& msg, tColor color) LIB_UTILS_IMPL
	void Write(bool timestamp, const std::string& msg) LIB_UTILS_IMPL
#ifdef LIB_UTILS_LOG_DEPRECATED
	void Write(bool timestamp, tColor colorText, const std::string& msg) LIB_UTILS_IMPL // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteLine() LIB_UTILS_IMPL
	void WriteLine(bool timestamp, const std::string& msg, tColor color) LIB_UTILS_IMPL
	void WriteLine(bool timestamp, const std::string& msg) LIB_UTILS_IMPL
#ifdef LIB_UTILS_LOG_DEPRECATED
	void WriteLine(bool timestamp, tColor colorText, const std::string& msg) LIB_UTILS_IMPL // DEPRECATED

	void WriteHex(bool timestamp, tColor colorText, const std::string& msg, const std::vector<std::uint8_t>& data) LIB_UTILS_IMPL // DEPRECATED
#endif // LIB_UTILS_LOG_DEPRECATED

	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd) LIB_UTILS_IMPL
	void WriteHex(const std::vector<std::uint8_t>& data, tColor dataColor) LIB_UTILS_IMPL
	void WriteHex(const std::vector<std::uint8_t>& data) LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor, int dataLinesBegin, int dataLinesEnd)  LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, tColor msgColor, const std::vector<std::uint8_t>& data, tColor dataColor) LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color, int dataLinesBegin, int dataLinesEnd) LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, tColor color) LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data, int dataLinesBegin, int dataLinesEnd) LIB_UTILS_IMPL
	void WriteHex(bool timestamp, const std::string& msg, const std::vector<std::uint8_t>& data) LIB_UTILS_IMPL

protected:
	virtual std::string GetLabel() const { return {}; }

	virtual void WriteLog(const std::string& text) = 0;
	virtual void WriteLogFile(const std::string& text) {}

private:
	virtual void WriteLog(bool a_timestamp, bool a_endl, const std::string& a_text, tColor a_textColor) LIB_UTILS_IMPL
};

std::string GetLogMessage(const std::string& msg, const std::string& filename, int line);

}
}
