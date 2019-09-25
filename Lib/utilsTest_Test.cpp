#include "utilsTest.h"

#include <iostream>

namespace utils
{
	namespace unit_test
	{

void UnitTest_Test()
{
	using namespace test;

	std::cout << "\n""UnitTest" << std::endl;

	WARNING("TEST_1");
	WARNING("TEST_2", false);
	WARNING("TEST_3-second", true);

	RESULT("Result_1", true);
	RESULT("Result_2", false);

	ASSERT(true);

	std::cout << std::endl;
}

	}
}
