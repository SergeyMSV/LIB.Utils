///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsLog
// 2016-05-16 (before)
// Standard ISO/IEC 114882, C++11
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <cstdint>

#include <mutex>
#include <string>
#include <vector>

namespace utils
{

enum class tLogColour : std::uint8_t
{
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

class tLog
{
	mutable std::mutex m_Mtx;

	bool m_ColourEnabled = false;

public:
	tLog() = delete;
	explicit tLog(bool colourEnabled);
	virtual ~tLog() { }

	void ColourEnabled(bool state);

	void Write(bool timestamp, tLogColour textColour, const std::string& msg);

	void WriteLine();
	void WriteLine(bool timestamp, tLogColour textColour, const std::string& msg);

	void WriteHex(bool timestamp, tLogColour textColour, const std::string& msg, const std::vector<std::uint8_t>& data);

protected:
	virtual std::string GetLabel() const { return {}; }

	virtual void WriteLog(const std::string& msg) = 0;

private:
	virtual void WriteLog(bool timestamp, bool endl, tLogColour textColour, const std::string& msg);
};

#else//LIB_UTILS_LOG

class tLog
{
public:
	tLog() = delete;
	explicit tLog(bool colourEnabled) { }
	virtual ~tLog() { }

	void ColourEnabled(bool state) { }

	void Write(bool timestamp, tLogColour textColour, const std::string& msg) { }

	void WriteLine() { }
	void WriteLine(bool timestamp, tLogColour textColour, const std::string& msg) { }

	void WriteHex(bool timestamp, tLogColour textColour, const std::string& msg, const std::vector<std::uint8_t>& data) { }

protected:
	virtual std::string GetLabel() const { return {}; }

	virtual void WriteLog(const std::string& msg) = 0;
};

#endif//LIB_UTILS_LOG

}
