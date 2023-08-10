#include "utilsBase.h"
#include "utilsLinux.h"
#include "utilsTest.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace utils
{

void UnitTest_Linux()
{
	std::cout << "\n""utils::linux\n";

	{
		std::string Res = linux::CmdLine("cat /proc/cpuinfo");
		utils::test::RESULT("cat /proc/cpuinfo", Res.find("i.MX6") != std::string::npos);
	}

	{
		std::string Res = linux::CmdLine("free");
		utils::test::RESULT("free", Res.find("445984") != std::string::npos);
	}

	std::cout << "\n""utils::linux::Get..\n";

	{
		linux::tCpuInfo CpuInfo
		{
			"ARMv7 Processor rev 5 (v7l)",
			64.00,
			"Freescale i.MX6 Ultralite (Device Tree)"
		};

		auto Res = linux::GetCpuInfo();
		utils::test::RESULT("cpuinfo", Res == CpuInfo);
	}

	{
		auto Res = linux::GetUptime();
		utils::test::RESULT("uptime", Res == 2789.21);
	}

	std::cout << "\n""utils::linux::GetPathConfig\n";

	{
		auto Res = linux::GetPathConfig("test1");
		utils::test::RESULT(".conf", Res.filename().string() == "test1.conf");
	}

	{
		auto Res = linux::GetPathConfig("test2");
		utils::test::RESULT(".conf.json", Res.filename().string() == "test2.conf.json");
	}

	{
		auto Res = linux::GetPathConfig("test3");
		utils::test::RESULT("~/.test3rc", Res.filename().string() == ".test3rc");
	}

	{
		auto Res = linux::GetPathConfig("test4");
		utils::test::RESULT("/etc/test4rc", Res.filename().string() == "test4rc");
	}

	{
		auto Res = linux::GetPathConfig("test5");
		utils::test::RESULT("/etc/test5", Res.filename().string() == "test5");
	}

	{
		auto Res = linux::GetPathConfig("test6");
		utils::test::RESULT("~/.test6", Res.filename().string() == ".test6");
	}

	{
		auto Res = linux::GetPathConfig("test7win");
		utils::test::RESULT("test7win", Res.filename().string() == "test7win.conf.json");
	}

	std::cout << std::endl;
}

}