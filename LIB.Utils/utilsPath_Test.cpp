#include "utilsPath.h"
#include "utilsTest.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace utils
{

void UnitTest_Path()
{
	std::cout << "\n""utils::path\n";

	{
		time_t TimeNow = 1660214334;// std::time(nullptr);
		tm* DateTime1 = std::localtime(&TimeNow);

		std::string DateTimeStr = GetDateTime(*DateTime1);
		
		utils::test::RESULT("DateTime String", DateTimeStr == "2022-08-11_13-38-54");

		tm DateTime2 = GetDateTime(DateTimeStr);

		utils::test::RESULT("DateTime Parse", memcmp(DateTime1, &DateTime2, sizeof(tm)));
	}

	std::cout << std::endl;
}

}