#include "utilsTest.h"

#include <iostream>

namespace utils
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

	RESULT_Total();

	std::cout << std::endl;
}

}
