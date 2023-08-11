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

	std::cout << std::endl;
}

}