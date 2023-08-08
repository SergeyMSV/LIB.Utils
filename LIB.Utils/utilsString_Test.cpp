#include "utilsString.h"
#include "utilsTest.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace utils
{

void UnitTest_String()
{
	std::cout << "\n""utils::string\n";

	{
		char Nom[] = "preved medved";
		utils::string::tStringTruncated<25> NomTrunc(Nom);
		for (int i = 0; i < 15; ++i)
			std::cout << NomTrunc.GetNext() << '\n';
	}

	{
		char Nom[] = "123456789012345678901234567890";
		utils::string::tStringTruncated<25> NomTrunc(Nom);
		for (int i = 0; i < 15; ++i)
			std::cout << NomTrunc.GetNext() << '\n';
	}

	{
		char Nom[] = "12345678901234567890123";
		utils::string::tStringTruncated<25> NomTrunc(Nom);
		for (int i = 0; i < 15; ++i)
			std::cout << NomTrunc.GetNext() << '\n';
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "cam_", "pic_");
		utils::test::RESULT("Replace 1", Res.has_value() && *Res == "/tmpram/somefolder/pic_2023-07-07_21-00.jpg");
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "cam", "pic");
		utils::test::RESULT("Replace 2", Res.has_value() && *Res == "/tmpram/somefolder/pic_2023-07-07_21-00.jpg");
	}
	
	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "am", "pic");
		utils::test::RESULT("Replace 3", !Res.has_value());
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "some", "pic");
		utils::test::RESULT("Replace 4", !Res.has_value());
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "cam_", "picture_");
		utils::test::RESULT("Replace 5", Res.has_value() && *Res == "/tmpram/somefolder/picture_2023-07-07_21-00.jpg");
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::string::Replace(A, "cam_", "p");
		utils::test::RESULT("Replace 6", Res.has_value() && *Res == "/tmpram/somefolder/p2023-07-07_21-00.jpg");
	}


	//{
	//	time_t TimeNow = 1660214334;// std::time(nullptr);
	//	tm* DateTime1 = std::localtime(&TimeNow);

	//	std::string DateTimeStr = GetDateTime(*DateTime1);
	//	
	//	utils::test::RESULT("DateTime String", DateTimeStr == "2022-08-11_13-38-54");

	//	tm DateTime2 = GetDateTime(DateTimeStr);

	//	utils::test::RESULT("DateTime Parse", memcmp(DateTime1, &DateTime2, sizeof(tm)));
	//}

	std::cout << std::endl;
}

}