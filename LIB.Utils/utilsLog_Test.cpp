#include "utilsLog.h"
#include "utilsTest.h"
#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <thread>

class tLogger : public utils::tLog
{
public:
	explicit tLogger(bool colourEnabled)
		:utils::tLog(colourEnabled)
	{
	}

protected:
	virtual void WriteLog(const std::string& msg) override final
	{
		std::cout << msg;
	}
};

namespace utils
{

void UnitTest_tLog()
{
	std::cout << "\n""utils::tLog" << std::endl;

	using namespace std::chrono_literals;

	tLogger Log(true);

	Log.Write(true, tLogColour::LightCyan, "Write light cyan");
	Log.Write(false, tLogColour::LightMagenta, "Write light magenta");
	Log.Write(false, tLogColour::LightGreen, "Write light green");
	Log.WriteLine();

	Log.WriteLine(true, tLogColour::Blue, "Write blue");
	Log.WriteLine(false, tLogColour::Yellow, "Write yellow");
	Log.WriteLine(true, tLogColour::LightYellow, std::string("Write light yellow"));
	Log.WriteLine(true, tLogColour::LightGreen, std::string(" -- Write light green... MAY BE IMPORTANT INFORMATION\n""that's something instead of info-block...\n""in previous versions..."));
	Log.WriteLine(false, tLogColour::Magenta, "One more info block...\n""with another colour :)");
	Log.WriteHex(true, tLogColour::Green, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F });
	std::this_thread::sleep_for(1s);
	Log.WriteHex(true, tLogColour::Cyan, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C });
	Log.WriteHex(false, tLogColour::Default, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16 });

	Log.ColourEnabled(false);

	Log.Write(true, tLogColour::LightCyan, "Write light cyan");
	Log.Write(false, tLogColour::LightMagenta, "Write light magenta");
	Log.Write(false, tLogColour::LightGreen, "Write light green");
	Log.WriteLine();

	Log.ColourEnabled(true);

	Log.Write(true, tLogColour::LightCyan, "Write light cyan");
	Log.Write(false, tLogColour::LightMagenta, "Write light magenta");
	Log.Write(false, tLogColour::LightGreen, "Write light green");
	Log.WriteLine();

	std::cout << std::endl;
}

}
