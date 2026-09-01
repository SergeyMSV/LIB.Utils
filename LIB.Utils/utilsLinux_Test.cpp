#include "utilsBase.h"
#include "utilsLinux.h"
#include "utilsPath.h"
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

namespace linux
{

static std::string GetStringEnding(const std::string& pattern, const std::string& str)//utilsString
{
	std::size_t Pos = str.find(pattern);
	if (Pos == std::string::npos)
		return {};
	std::string Str = str.substr(Pos + pattern.size());
	std::string_view StrView = Str;
	StrView.remove_prefix(std::min(StrView.find_first_not_of(" "), StrView.size()));
	return StrView.data();
}

// Functions for tests
#if defined(_WIN32)
std::string CmdLineWinTest(const std::string& cmd)
{
	if (cmd == "free")
	{
		return
			"               total        used        free      shared  buff / cache   available\n\
Mem:          492116       33240      129108        1184      329768      445984\n\
Swap:              0           0           0";
	}

	std::string CmdValue = GetStringEnding("cat", cmd);
	if (!CmdValue.empty())
	{
		std::filesystem::path Path = path::GetPathNormal(CmdValue);
		if (Path.empty())
			return {};

		std::fstream File(Path, std::ios::in);
		if (!File.good())
			return {};

		std::string Data;

		while (!File.eof())
		{
			std::string Line;
			std::getline(File, Line);
			Data += Line + "\n";
		}

		File.close();

		return Data;
	}

	return {};
}
#endif
}

}