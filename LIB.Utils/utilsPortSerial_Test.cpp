#include "utilsChrono.h"
#include "utilsCRC.h"
#include "utilsPortSerial.h"
#include "utilsTest.h"

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

class tBoard : public utils::port::serial::tPortSerialAsync<>
{
	std::vector<std::uint8_t> m_DataReceived;
	mutable std::mutex m_DataReceivedMtx;

public:
	tBoard(boost::asio::io_context& io, const std::string& id)
		:tPortSerialAsync(io, id, 115200)
	{
	}

	std::vector<std::uint8_t> GetReceived()
	{
		std::lock_guard<std::mutex> Lock(m_DataReceivedMtx);
		if (m_DataReceived.empty())
			return {};
		std::vector<std::uint8_t> Data = std::move(m_DataReceived);
		return Data;
	}

protected:
	void OnReceived(const std::vector<std::uint8_t>& data) override
	{
		std::lock_guard<std::mutex> Lock(m_DataReceivedMtx);
		m_DataReceived.insert(m_DataReceived.end(), data.begin(), data.end());
	}
};

using tBoardOneWire = utils::port::serial::tPortOneWireSync;

namespace utils
{

void UnitTest_PortSerial()
{
	std::cout << "\n""utils::port::serial::tPortSerial" << std::endl;

	using namespace std::chrono_literals;

	{
		const std::vector<std::uint8_t> Data(100, '4');

		boost::asio::io_context ioc;

		const std::string PortID = "COM1";

		tBoard Port(ioc, PortID);

		std::thread Thread_ioc([&ioc]() { ioc.run(); });

		Port.Send(Data);
		std::this_thread::sleep_for(1s);

		const bool Result = Port.GetReceived() == Data;

		test::RESULT("SerialPort: data received", Result);
		test::WARNING("SerialPort: check if loopback jumpers are connected to " + PortID, !Result);

		ioc.stop();

		Thread_ioc.join();
	}

	using namespace utils::chrono;

	{ // Time scale
		tTimePoint TimeStart = tClock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		test::RESULT("Sleep for 10 ms -> " + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms", true);
	}

	{ // 1-Wire Port DALLAS
		boost::asio::io_context ioc;
		const std::string PortID = "COM4";
		tBoardOneWire Port(ioc, PortID, tBoardOneWire::tSpeed::Norm);// tBoardOneWire::tSpeed::Fast);
		std::thread Thread_ioc([&ioc]() { ioc.run(); });

		//std::vector<std::uint8_t> Rsp = Port.Transaction({ 0x81, 0xFF }, 0);
		//std::vector<std::uint8_t> Rsp = Port.Transaction({ 0x7E, 0x7E }, 0);

		{ // Reset
			tTimePoint TimeStart = tClock::now();
			const std::vector<std::uint8_t> Rsp_Reset = { 0x0F, 0xFF };
			tBoardOneWire::tStatus Status = Port.Reset();
			bool Result = Status == tBoardOneWire::tStatus::Success;
			test::RESULT("OneWire: Reset (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
			test::WARNING("1-Wire Port: check if DS1990A/DS18B20 is connected to " + PortID, !Result);
		}

		auto OW_Reset = [&Port]()->bool
		{
			tTimePoint TimeStartOper = tClock::now();
			tBoardOneWire::tStatus Status = Port.Reset();
			bool Result = Status == tBoardOneWire::tStatus::Success;
			test::RESULT("Reset (" + std::to_string(GetDuration<ttime_ms>(TimeStartOper, tClock::now())) + " ms)", Result);
			return Result;
		};

		OW_Reset();

		{ // ReadROM
			tTimePoint TimeStart = tClock::now();
			std::cout << "ReadROM\n";
			std::vector<std::uint8_t> Rsp = Port.Transaction({ 0x33 }, 8);
			bool Result = false;
			if (!Rsp.empty())
			{
				std::uint8_t CRC = utils::crc::CRC08_DALLAS(Rsp.data(), Rsp.size() - 1);
				Result =
					Rsp.size() == 8 &&
					Rsp[Rsp.size() - 1] == CRC;
				if (Result)
				{
					std::vector<std::uint8_t> SN(Rsp.rbegin() + 1, Rsp.rend() - 1);
					std::cout << "Family Code: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)Rsp[0] << "; SN: " << utils::test::ToStringHEX(SN, true) << '\n';
				}
			}
			test::RESULT("OneWire: DS1990A/DS18B20 ReadROM (0x33) (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
		}

		ioc.stop();

		Thread_ioc.join();
	}

	std::cout << std::endl;
}

}
