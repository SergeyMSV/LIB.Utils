#include "utilsTime.h"

#include <iostream>

namespace utils
{

void UnitTest_Time()
{
	{
		utils::time::tDateTime Dt;
		Dt = 100;
		Dt = utils::time::tDateTime::Now();
		std::string DtDtr = Dt.ToString("%Y-%m-%d * %H-%M-%S");
		std::cout << "ToString 1: " << DtDtr << '\n';
	}

	{
		utils::time::tDateTime Dt = utils::time::tDateTime::Now();
		std::string DtDtr = Dt.ToStringPath();
		std::cout << "ToString 2: " << DtDtr << '\n';
	}

	{
		utils::time::tDateTime Dt = utils::time::tDateTime::Now();
		std::string DtDtr = Dt.ToString();
		std::cout << "ToString 3: " << DtDtr << '\n';
	}

	{
		std::cout << "ToString X: " << utils::time::tDateTime::Now().ToString() << '\n';
	}


	{
		utils::time::tDateTime Dt = utils::time::tDateTime::Parse("2022-08-11_13-38-54", "%Y-%m-%d_%H-%M-%S");
		std::string DtDtr = Dt.ToString();
		std::cout << "Parse 1: " << DtDtr << '\n';
	}
	

	{
		utils::time::tDateTime Dt = utils::time::tDateTime::Parse("2024-11-12 * 14-53-26", "%Y-%m-%d * %H-%M-%S");
		std::string DtDtr = Dt.ToString();
		std::cout << "Parse 2: " << DtDtr << '\n';
	}

	{
		utils::time::tDateTime Dt = utils::time::tDateTime::Parse("2024-11-12 14:53:26");
		std::string DtDtr = Dt.ToString();
		std::cout << "Parse 3: " << DtDtr << '\n';
	}

	{
		utils::time::tDateTime Dt = utils::time::tDateTime::ParsePath("2024-11-12_14-53-26");
		std::string DtDtr = Dt.ToString();
		std::cout << "Parse 4: " << DtDtr << '\n';
	}
}

}