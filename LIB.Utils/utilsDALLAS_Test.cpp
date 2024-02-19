#include "devConfig.h"
#include "utilsChrono.h"
#include "utilsCRC.h"
#include "utilsDALLAS.h"
#include "utilsPortSerial.h"
#include "utilsTest.h"

#include <chrono>
#include <ctime>

#include <mutex>
#include <thread>

namespace utils
{

using namespace DALLAS;

std::string ToString(const tID& id)
{
	std::string Str;
	Str += "ID: ";
	Str += test::ToStringHEX(std::vector<std::uint8_t>(std::rbegin(id.Value), std::rend(id.Value)), false);
	return Str;
};

std::string ToString(const tROM& rom)
{
	std::string Str;
	Str = "FC: "; // "Family Code: ";
	Str += test::ToStringHEX(std::vector<std::uint8_t>{ (std::uint8_t)rom.Field.FamilyCode }, false);
	Str += "; ";
	Str += ToString(rom.Field.ID);
	return Str;
};

void UnitTest_DALLAS()
{
	std::cout << "\n""utils::port_serial::tPortSerial" << std::endl;

	using namespace std::chrono_literals;

	{ // Time scale
		tTimePoint TimeStart = tClock::now();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		test::RESULT("Sleep for 10 ms -> " + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms", true);
	}

	{
		tID ID{}; // 28ff640e7744c86e
		ID.Value[0] = 0xff;
		ID.Value[1] = 0x64;
		ID.Value[2] = 0x0e;
		ID.Value[3] = 0x77;
		ID.Value[4] = 0x44;
		ID.Value[5] = 0xc8;
		tROM ROM = MakeROM((tFamilyCode)0x28, ID);
		test::RESULT("MakeROM", ROM.Field.CRC == 0x6e);
	}

#ifdef UTILS_TEST_BASIC
	{ // 1-Wire Port DALLAS
		boost::asio::io_context ioc;
		const std::string PortID = "COM4";
		tDALLAS Port(ioc, PortID, tDALLAS::tSpeed::Fast); // Fast
		std::thread Thread_IO([&ioc]() { ioc.run(); });

		//std::vector<std::uint8_t> Rsp = Port.Transaction({ 0x81, 0xFF }, 0);
		//std::vector<std::uint8_t> Rsp = Port.Transaction({ 0x7E, 0x7E }, 0);

		{ // Reset
			tTimePoint TimeStart = tClock::now();
			const std::vector<std::uint8_t> Rsp_Reset = { 0x0F, 0xFF };
			tDALLAS::tStatus Status = Port.Reset();
			bool Result = Status == tDALLAS::tStatus::Success;
			test::RESULT("OneWire: Reset (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
			test::WARNING("1-Wire Port: check if DS1990A/DS18B20 is connected to " + PortID, !Result);
		}

		auto ShowROMs = [&](const std::string& test, const std::vector<tROM>& roms)
		{
			std::cout << test << '\n';
			for (const auto& i : roms)
			{
				std::cout << ToString(i) << '\n';
			}
		};

		{ // SearchROM
			tTimePoint TimeStart = tClock::now();
			auto Found = Port.Search();
			ShowROMs("Search All", Found);
			bool Result = !Found.empty();
			test::RESULT("Search All: (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
		}
		{
			tTimePoint TimeStart = tClock::now();
			auto Found = Port.Search(tFamilyCode::DS18B20);
			ShowROMs("Search DS18B20", Found);
			bool Result = !Found.empty();
			test::RESULT("Search DS18B20: (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
		}
		{
			tTimePoint TimeStart = tClock::now();
			auto Found = Port.Search(tFamilyCode::DS1990A);
			ShowROMs("Search DS1990A", Found);
			bool Result = !Found.empty();
			test::RESULT("Search DS1990A: (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
		}

		{ // SearchROM
			tTimePoint TimeStart = tClock::now();
			auto Found = Port.Search();
			ShowROMs("Search", Found);
			bool Result = !Found.empty();
			test::RESULT("Search: (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
			for (int i = 0; i < 5; ++i)
			{
				tTimePoint TimeStart = tClock::now();
				std::vector<tThermal> Thermo = Port.GetTemperature(Found);
				std::cout << '\n';
				for (auto& i : Thermo)
				{
					std::cout << ToString(i.ROM) << "Temperature = " << ToDouble(i.Temperature) << '\n';
				}
				test::RESULT("GetTemperature: (" + std::to_string(GetDuration<ttime_ms>(TimeStart, tClock::now())) + " ms)", Result);
			}
		}

		ioc.stop();

		Thread_IO.join();
	}
#endif // UTILS_TEST_BASIC

#ifdef UTILS_TEST_BUTTON_COPY
	{ // 1-Wire Port DALLAS
		boost::asio::io_context ioc;
		const std::string PortID = "COM4";
		tDALLAS Port(ioc, PortID, tDALLAS::tSpeed::Norm); // Norm - not all of keys work at the baudrate of 230400
		std::thread Thread_IO([&ioc]() { ioc.run(); });

		//Port.Test();

		auto WaitForKey = [&Port](tID& rom)->bool
		{
			std::cout << '\n';
			bool KeyFound = false;
			for (int i = 0; i < 100; ++i) // [#]
			{
				std::optional<tID> ROMRaw = Port.ReadKey();
				if (ROMRaw.has_value())
				{
					KeyFound = true;
					rom = *ROMRaw;
					break;
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(100));
				std::cout << '.';
			}
			std::cout << '\n';
			std::cout << "Key: " << (KeyFound ? ToString(rom) : "NOT FOUND") << '\n';
			return KeyFound;
		};

		std::cout << "\nEnter Key Source\n";
		tID KeyIDSrc;
		if (WaitForKey(KeyIDSrc))
		{
			std::cout << "\nEnter Key Destination\n";
			for (int i = 0; i < 100; ++i) // [#]
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // The source key should be removed.
				std::cout << '.';
				//if (WaitForKey(KeyROMDst)) // Key data can be wrong but nevertheless the key is alive.
				{
					const tCodeError CErr = Port.WriteKey(KeyIDSrc);
					if (CErr == tCodeError::Success)
					{
						tID KeyIDDst;
						WaitForKey(KeyIDDst);
						std::cout << "\nWritten: " << ToString(KeyIDDst) << '\n';
						break;
					}
					else if (CErr == tCodeError::SameKeyID)
					{
						std::cout << "\nKey ID: the same key ID\n";
					}
					else if (CErr == tCodeError::NotWritten)
					{
						std::cout << "\nNotWritten\n";
					}
					else if (CErr == tCodeError::WrongKeyID)
					{
						std::cout << "\nWrongKeyID\n";
					}
				}
			}
		}

		ioc.stop();

		Thread_IO.join();
	}
#endif // UTILS_TEST_BUTTON_COPY

	std::cout << std::endl;
}

}