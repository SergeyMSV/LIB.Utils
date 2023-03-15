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
		auto ResRaw = linux::GetPathConfig("test1");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT(".conf", Res == "/etc/default/test1.conf");
	}

	{
		auto ResRaw = linux::GetPathConfig("test2");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT(".conf.json", Res == "/etc/default/test2.conf.json");
	}

	{
		auto ResRaw = linux::GetPathConfig("test3");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("~/.test3rc", Res == "/root/.test3rc");
	}

	{
		auto ResRaw = linux::GetPathConfig("test4");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("/etc/test4rc", Res == "/etc/test4rc");
	}

	{
		auto ResRaw = linux::GetPathConfig("test5");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("/etc/test5", Res == "/etc/test5");
	}

	{
		auto ResRaw = linux::GetPathConfig("test6");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("~/.test6", Res == "/root/.test6");
	}

	{
		auto ResRaw = linux::GetPathConfig("test7win");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("test7win", Res == "/../test7win.conf.json");
	}

	std::cout << "\n""utils::path::GetPath\n";

	{
		auto ResRaw = linux::GetPath("~/.test3rc");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT(".test3rc", Res == "/root/.test3rc");
	}

	{
		auto ResRaw = linux::GetPath("/etc/default/test1.conf");
		auto Res = utils::GetStringEnding("test_root_fs", ResRaw);
		utils::test::RESULT("test1.conf", Res == "/etc/default/test1.conf");
	}	

	std::cout << std::endl;
}

}