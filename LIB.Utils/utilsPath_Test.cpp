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

	std::cout << "\n""utils::linux::GetAppName\n";

	{
		auto Res = path::GetAppName("/qwert/appname_asd");
		utils::test::RESULT("GetAppName", Res == "appname_asd");
	}

	{
		auto Res = path::GetAppName("/qwert/appname_asd.exe");
		utils::test::RESULT("GetAppName ext.", Res == "appname_asd");
	}

	{
		auto Res = path::GetAppNameMain("/qwert/appname_asd");
		utils::test::RESULT("GetAppName", Res == "appname");
	}

	{
		auto Res = path::GetAppNameMain("/qwert/appname_asd.exe");
		utils::test::RESULT("GetAppName ext.", Res == "appname");
	}

	std::cout << "\n""utils::linux::GetPathNormal\n";

	{
		auto Res = path::GetPathNormal("~/temp/test_n1");
		utils::test::RESULT("/root/temp/test_n1", !Res.empty());
	}

	{
		auto Res = path::GetPathNormal("/tmpram");
		utils::test::RESULT("/tmpram", !Res.empty());
	}

	std::cout << "\n""utils::linux::GetPathConfig\n";

	{
		auto Res = path::GetPathConfig("test1");
		utils::test::RESULT(".conf", Res.filename().string() == "test1.conf");
	}

	{
		auto Res = path::GetPathConfig("test2");
		utils::test::RESULT(".conf.json", Res.filename().string() == "test2.conf.json");
	}

	{
		auto Res = path::GetPathConfig("test3");
		utils::test::RESULT("~/.test3rc", Res.filename().string() == ".test3rc");
	}

	{
		auto Res = path::GetPathConfig("test4");
		utils::test::RESULT("/etc/test4rc", Res.filename().string() == "test4rc");
	}

	{
		auto Res = path::GetPathConfig("test5");
		utils::test::RESULT("/etc/test5", Res.filename().string() == "test5");
	}

	{
		auto Res = path::GetPathConfig("test6");
		utils::test::RESULT("~/.test6", Res.filename().string() == ".test6");
	}

	{
		auto Res = path::GetPathConfig("test7win");
		utils::test::RESULT("test7win", Res.filename().string() == "test7win.conf.json");
	}

	std::cout << std::endl;
}

}