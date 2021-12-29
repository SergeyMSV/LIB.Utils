#include "utilsChrono.h"

#include "utilsTest.h"

#include <iostream>
#include <thread>

namespace utils
{

void UnitTest_Chrono()
{
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


	std::cout << std::endl;
}

}
