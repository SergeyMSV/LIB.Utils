#include "utilsLog.h"
#include "utilsSerialPort.h"
#include "utilsTest.h"
#include <chrono>
#include <ctime>

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

class tBoard : public utils::port_serial::tPortSerialAsync<>
{
	std::vector<std::uint8_t> m_DataReceived;
	mutable std::mutex m_Mtx;
public:
	tBoard(boost::asio::io_context& io, const std::string& id)
		:tPortSerialAsync(io, id, 115200)
	{
	}
	virtual ~tBoard()
	{
	}

	bool IsReceived() const { return !m_DataReceived.empty(); }
	std::vector<std::uint8_t> GetReceived()
	{
		std::lock_guard<std::mutex> Lock(m_Mtx);
		std::vector<std::uint8_t> Data = std::move(m_DataReceived);
		return Data;
	}

protected:
	void OnReceived(std::vector<std::uint8_t>& data) override
	{
		std::lock_guard<std::mutex> Lock(m_Mtx);
		m_DataReceived.insert(m_DataReceived.end(), data.begin(), data.end());
	}
};

namespace utils
{

void UnitTest_SerialPort()
{
	std::cout << "\n""utils::serial_port::tSerialPort" << std::endl;

	using namespace std::chrono_literals;

	{
		const std::vector<std::uint8_t> Data(100, '4');

		boost::asio::io_context ioc;

		const std::string PortID = "COM1";

		tBoard Port(ioc, PortID);

		std::thread Thread_IO([&ioc]() { ioc.run(); });

		Port.Send(Data);
		std::this_thread::sleep_for(1s);

		const bool Result = Port.IsReceived() && Port.GetReceived() == Data;

		test::RESULT("SerialPort: data received", Result);
		test::WARNING("SerialPort: check if loopback jumpers are connected to " + PortID, !Result);

		ioc.stop();

		Thread_IO.join();
	}

	std::cout << std::endl;
}

}
