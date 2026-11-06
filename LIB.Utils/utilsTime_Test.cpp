#include "utilsTime.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{

void UnitTest_Parse(const std::string& a_msg, const std::string& a_str, const std::string& a_format, const std::string& a_res)
{
	utils::time::tDateTime Dt = utils::time::tDateTime::Parse(a_str, a_format);
	std::string res = Dt.ToString(a_format);
	if (res != a_res)
		std::cout << a_str << "  ----  " << res << '\n';
	utils::test::RESULT(a_msg, res == a_res);
}

void UnitTest_Parse(const std::string& a_msg, const std::string& a_str, const std::string& a_res)
{
	utils::time::tDateTime Dt = utils::time::tDateTime::Parse(a_str);
	std::string res = Dt.ToString();
	if (res != a_res)
		std::cout << a_res << "  ----  " << res << '\n';
	utils::test::RESULT(a_msg, res == a_res);
}

void UnitTest_ParsePath(const std::string& a_msg, const std::string& a_str, const std::string& a_res)
{
	utils::time::tDateTime Dt = utils::time::tDateTime::ParsePath(a_str);
	std::string res = Dt.ToStringPath();
	if (res != a_res)
		std::cout << a_res << "  ----  " << res << '\n';
	utils::test::RESULT(a_msg, res == a_res);
}

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


	UnitTest_Parse("Parse 1", "2022-08-11_13-38-54",		"%Y-%m-%d_%H-%M-%S",	"2022-08-11_13-38-54");
	UnitTest_Parse("Parse 2", "2024-11-12 * 14-53-26",		"%Y-%m-%d * %H-%M-%S",	"2024-11-12 * 14-53-26");
	UnitTest_Parse("Parse 3", "2024-11-12 *     14-53-26",	"%Y-%m-%d * %H-%M-%S",	"2024-11-12 * 14-53-26");
	//UnitTest_Parse("Parse 4", "2024-11-12 * 14-53-26",		"%Y-%m-%d %H-%M-%S",	"2024-11-12 14-53-26");

	UnitTest_Parse("Parse 10", "2024-11-12 14:53:26", "2024-11-12 14:53:26");
	UnitTest_ParsePath("Parse 11", "2024-11-12_14-53-26", "2024-11-12_14-53-26");

	std::cout << '\n';
}

}