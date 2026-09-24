#include "utilsRand.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{

void UnitTest_Rand()
{
	{
		std::string UID = rand::MakeClientID(32);
		std::cout << UID << '\n';
	}

	{
		std::string UID = rand::MakeClientID(64);
		std::cout << UID << '\n';
	}

	std::cout << std::endl;
}

}
