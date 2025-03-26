#include "utilsVersion.h"
#include "utilsTest.h"

namespace utils
{

void UnitTest_Version()
{
	std::cout << "\n""utils::Version" << std::endl;

	//{
	//	bool Result = false;

	//	try
	//	{
	//		tVersion Value("123.456.dd");
	//	}
	//	catch (const std::exception& e)
	//	{
	//		Result = std::string(e.what()).find_first_of("format") != std::string::npos;
	//	}

	//	utils::test::RESULT("tVersion 123.456.dd", Result);
	//}

	//{
	//	bool Result = false;

	//	try
	//	{
	//		tVersion Value("123.");
	//	}
	//	catch (const std::exception& e)
	//	{
	//		Result = std::string(e.what()).find_first_of("format") != std::string::npos;
	//	}

	//	utils::test::RESULT("tVersion 123.", Result);
	//}

	//{
	//	bool Result = false;

	//	try
	//	{
	//		tVersion Value("123.456.");
	//	}
	//	catch (const std::exception& e)
	//	{
	//		Result = std::string(e.what()).find_first_of("format") != std::string::npos;
	//	}

	//	utils::test::RESULT("tVersion 123.456.", Result);
	//}

	{
		tVersion Value("asdf 123.456.7890");

		bool Result =
			Value.Major == 123 &&
			Value.Minor == 456 &&
			Value.Build == 7890;

		utils::test::RESULT("tVersion 123.456.7890", Result);
		utils::test::RESULT("tVersion 123.456.7890", "123.456.7890" == Value.ToString());
	}

	{
		tVersion Value1("0.1.1");
		tVersion Value2("0.1.1");
		utils::test::RESULT("tVersion !=", (Value1 != Value2) == false);
		utils::test::RESULT("tVersion ==", Value1 == Value2);
	}

	{
		tVersion Value1("0.1.1");
		tVersion Value2("0.1.2");
		utils::test::RESULT("tVersion build 1", Value1 > Value2 == false);
		utils::test::RESULT("tVersion build 2", Value2 < Value1 == false);
		utils::test::RESULT("tVersion build 3", Value1 < Value2);
		utils::test::RESULT("tVersion build 4", Value2 > Value1);
	}

	{
		tVersion Value1("0.1.1");
		tVersion Value2("0.2.1");
		utils::test::RESULT("tVersion minor 1", Value1 > Value2 == false);
		utils::test::RESULT("tVersion minor 2", Value2 < Value1 == false);
		utils::test::RESULT("tVersion minor 3", Value1 < Value2);
		utils::test::RESULT("tVersion minor 4", Value2 > Value1);
	}

	{
		tVersion Value1("0.1.1");
		tVersion Value2("1.1.1");
		utils::test::RESULT("tVersion major 1", Value1 > Value2 == false);
		utils::test::RESULT("tVersion major 2", Value2 < Value1 == false);
		utils::test::RESULT("tVersion major 3", Value1 < Value2);
		utils::test::RESULT("tVersion major 4", Value2 > Value1);
	}

	{
		tVersion Value1("0.0.3");
		tVersion Value2("0.1.1");
		utils::test::RESULT("tVersion 1", Value1 > Value2 == false);
		utils::test::RESULT("tVersion 2", Value1 < Value2);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
}

}
