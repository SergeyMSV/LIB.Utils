#include "utilsChrono.h"

#include "utilsTest.h"

#include <iomanip>
#include <iostream>
#include <thread>

namespace utils
{

void UnitTest_Chrono()
{
	using namespace utils::chrono;

	std::cout << "\n""utils::Chrono" << std::endl;

	{
		tTimePoint TimeStart = tClock::now();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		auto Duration = GetDuration<ttime_ms>(TimeStart, tClock::now());

		test::RESULT("GetDuration 1", Duration > 0);
	}

	{
		tTimePoint TimeStart = tClock::now();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		auto Duration = GetDuration<ttime_ms>(tClock::now(), TimeStart);

		test::RESULT("GetDuration 2", Duration == 0);
	}

	std::cout << "\n""utils::Chrono::tTimePeriod\n";

	{
		tTimePeriod TimePeriod(true);
		TimePeriod.Set(2, true);

		std::time_t t = std::time(nullptr);
		std::cout << 0 << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';

		for (int i = 0, j = 0; i < 1000; ++i)
		{
			if (!TimePeriod.IsReady())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			t = std::time(nullptr);
			std::cout << ++j << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';
		}
	}

	std::cout << "\n""utils::Chrono::tTimePeriodCount 1\n";

	{
		tTimePeriodCount TimePeriod(true);
		TimePeriod.Set(15, 2, 3, true);

		std::time_t t = std::time(nullptr);
		std::cout << 0 << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';

		for (int i = 0, j = 0; i < 3000; ++i)
		{
			if (!TimePeriod.IsReady())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			t = std::time(nullptr);
			std::cout << ++j << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';
		}
	}

	std::cout << "\n""utils::Chrono::tTimePeriodCount 2\n";

	{
		tTimePeriodCount TimePeriod(true);
		TimePeriod.Set(15, 2, 3, true);

		std::time_t t = std::time(nullptr);
		std::cout << 0 << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';

		for (int i = 0, j = 0; i < 3000; ++i)
		{
			if (!TimePeriod.IsReady())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			t = std::time(nullptr);
			std::cout << ++j << " > " << std::put_time(std::gmtime(&t), "%T") << '\n';

			if (j == 2)
			{
				j = 0;
				TimePeriod.Complete();
			}
		}
	}


	std::cout << std::endl;
}

}
