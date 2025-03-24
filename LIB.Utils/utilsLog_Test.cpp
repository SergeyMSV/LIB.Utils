#include "utilsLog.h"
#include "utilsTest.h"
#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <thread>

class tLogger : public utils::log::tLog
{
protected:
	std::string GetLabel() const override final
	{
		return "SIGN";
	}

	void WriteLog(const std::string& msg) override final
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

	tLogger Log;

	Log.Write(true, log::tColor::LightCyan, "Write light cyan");
	Log.Write(false, log::tColor::LightMagenta, "Write light magenta");
	Log.Write(false, log::tColor::LightGreen, "Write light green");
	Log.WriteLine();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	Log.WriteLine(true, log::tColor::Blue, "Write blue");
	Log.WriteLine(false, log::tColor::Yellow, "Write yellow");
	Log.WriteLine(true, log::tColor::LightYellow, std::string("Write light yellow"));
	Log.WriteLine(true, log::tColor::LightGreen, std::string(" -- Write light green... MAY BE IMPORTANT INFORMATION\n""that's something instead of info-block...\n""in previous versions..."));
	Log.WriteLine(false, log::tColor::Magenta, "One more info block...\n""with another colour :)");
	Log.WriteHex(true, log::tColor::Green, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F });
	std::this_thread::sleep_for(1s);
	Log.WriteHex(true, log::tColor::Cyan, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C });
	Log.WriteHex(false, log::tColor::Default, "WriteHex", { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16 });

	Log.Write(true, log::tColor::LightCyan, "Write light cyan");
	Log.Write(false, log::tColor::LightMagenta, "Write light magenta");
	Log.Write(false, log::tColor::LightGreen, "Write light green");
	Log.WriteLine();

	std::cout << std::endl;
}

}
