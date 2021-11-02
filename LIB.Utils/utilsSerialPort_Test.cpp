#include "utilsLog.h"
#include "utilsSerialPort.h"
#include "utilsTest.h"
#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <thread>

volatile bool g_DataReceived = false;
const std::vector<uint8_t> g_Data(100, '4');

const std::string g_COM = "COM1";

class tBoard : public utils::serial_port::tSerialPort<>
{
public:
	explicit tBoard(boost::asio::io_context& io)
		:tSerialPort(io, g_COM, 115200)
	{
	}
	virtual ~tBoard()
	{
	}

protected:
	void OnReceived(std::vector<uint8_t>& data) override
	{
		if (g_Data == data)
			g_DataReceived = true;
	}
};

namespace utils
{

void UnitTest_SerialPort()
{
	std::cout << "\n""utils::serial_port::tSerialPort" << std::endl;

	using namespace std::chrono_literals;

	boost::asio::io_context ioc;

	tBoard Port(ioc);

	std::thread Thread_IO([&ioc]() { ioc.run(); });

	Port.Send(g_Data);

	std::this_thread::sleep_for(1s);

	test::RESULT("SerialPort: data received", g_DataReceived);
	const std::string Warning = "SerialPort: check if loopback jumpers are connected to " + g_COM;
	test::WARNING(Warning.c_str(), !g_DataReceived);

	ioc.stop();

	Thread_IO.join();

	std::cout << std::endl;
}

}
