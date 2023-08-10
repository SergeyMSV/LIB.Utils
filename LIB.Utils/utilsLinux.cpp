#include "utilsBase.h"
#include "utilsLinux.h"
#include "utilsPath.h"

#include <cerrno>
#include <cstdio>

#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

//#define LIB_UTILS_LINUX_LOG

#if defined(LIB_UTILS_LINUX_LOG)
#include <iostream>
#endif

namespace utils
{

namespace linux
{

#if defined(_WIN32)
static std::string CmdLineWinTest(const std::string& cmd);
#endif

std::string CmdLine(const std::string& cmd)
{
#if defined(_WIN32)
	return CmdLineWinTest(cmd);
#else
	FILE* File = popen(cmd.c_str(), "r");//File = popen("/bin/ls /etc/", "r");
	if (File == NULL)
		return {};

	std::string CmdRsp;

	char DataChunk[1035];
	while (fgets(DataChunk, sizeof(DataChunk), File) != NULL)
	{
		CmdRsp += DataChunk;
	}

	pclose(File);

	if (!CmdRsp.empty() && CmdRsp.back() == '\n')//Removes last '\n'
		CmdRsp.pop_back();

	return CmdRsp;
#endif
}

std::filesystem::path GetPathW(const std::filesystem::path& pathRaw)
{
	if (pathRaw.empty())
		return {};
	std::error_code ErrCode;//In order to avoid exceptions
	std::filesystem::path Path;
#if defined(_WIN32)
	Path = "test_root_fs";
#endif
	//path("foo") / ""      // the result is "foo/" (appends)
	//path("foo") / "/bar"; // the result is "/bar" (replaces)
	if (pathRaw.string()[0] == '/')
	{
		Path += pathRaw.lexically_normal();
	}
#if defined(_WIN32)
	else if (pathRaw.string()[0] == '~')
	{
		std::string PathRawStr = pathRaw.string();
		PathRawStr.replace(0, 1, "root");
		Path /= PathRawStr;
	}
#endif
	else
	{
		Path /= pathRaw.lexically_normal();
	}

	return std::filesystem::canonical(Path, ErrCode);
}

double GetUptime()
{
	std::filesystem::path Path = GetPathW("/proc/uptime");
	std::fstream File(Path, std::ios::in);
	if (!File.good())
		return {};

	double UptimeSeconds;
	File >> UptimeSeconds;

	File.close();

	return UptimeSeconds;
}

std::string UptimeToString(double uptime)
{
	int Utime_Day = static_cast<int>(uptime / 86400);
	int UptimeRemove = Utime_Day * 86400;
	int Utime_Hour = static_cast<int>((uptime - UptimeRemove) / 3600);
	UptimeRemove += Utime_Hour * 3600;
	int Utime_Min = static_cast<int>((uptime - UptimeRemove) / 60);
	UptimeRemove += Utime_Min * 60;
	int Utime_Sec = static_cast<int>(uptime - UptimeRemove);

	std::stringstream SStr;
	SStr << Utime_Day << " days ";
	SStr << std::setfill('0') << std::setw(2) << Utime_Hour << ":";
	SStr << std::setfill('0') << std::setw(2) << Utime_Min << ":";
	SStr << std::setfill('0') << std::setw(2) << Utime_Sec;

	return SStr.str();
}

std::string GetUptimeString()
{
	return UptimeToString(GetUptime());
}

tCpuInfo GetCpuInfo()
{
	std::filesystem::path Path = GetPathW("/proc/cpuinfo");
	std::fstream File(Path, std::ios::in);
	if (!File.good())
		return {};

	tCpuInfo CpuInfo{};

	std::deque<std::string> Strings;

	while (!File.eof())
	{
		std::string Line;
		std::getline(File, Line);
		Line.erase(std::remove_if(Line.begin(), Line.end(), [](char ch) { return ch == '\t'; }), Line.end());

		std::size_t Pos = Line.find(":", 0);
		std::string PrmName = Line.substr(0, Pos);

		auto GetValueString = [&Pos](const std::string& a_line)
		{
			std::string Value = a_line.substr(Pos, a_line.size());
			Value.erase(Value.begin(), std::find_if(Value.begin(), Value.end(), [](char ch) { return ch != ' ' && ch != ':'; }));
			return Value;
		};

		if (PrmName == "model name")
		{
			CpuInfo.ModelName = GetValueString(Line);
		}
		else if (PrmName == "BogoMIPS")
		{
			errno = 0;
			std::string Value = GetValueString(Line);
			double Num = strtod(Value.c_str(), nullptr);
			if (Num > 0 && errno != ERANGE)
				CpuInfo.BogoMIPS = Num;
			errno = 0;
		}
		else if (PrmName == "Hardware")
		{
			CpuInfo.Hardware = GetValueString(Line);
		}
	}

	File.close();

	return CpuInfo;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace PathConfig
{

const std::vector<std::string> g_PathConfig =
{
	{"."},
	{"../etc"},
	{"/etc"},
	{"~/"},
	{"/etc/default"},
	{"/usr/local/etc"},
#if defined(_WIN32)
	{".."}, // $(ProjectDir)
#endif
};

static bool TestPathFile(std::filesystem::path path, const std::string& filename, std::filesystem::path& pathFull)
{
	path.append(filename);
#if defined(LIB_UTILS_LINUX_LOG)
	std::cout << "TestFile: " << path.string() << '\n';
#endif
	std::fstream File = std::fstream(path, std::ios::in);
	if (!File.good())
		return false;
	File.close();
	pathFull = path;
	return true;
}

static std::filesystem::path TestPath(const std::filesystem::path& path, std::string filename, bool currPath, bool testDir)
{
	std::filesystem::path PathFull;

	// It can find itself. This statement in order to avoid that situation.
	if (!currPath && TestPathFile(path, filename, PathFull))
		return PathFull;

	if (TestPathFile(path, "." + filename, PathFull)) // hidden file
		return PathFull;
	if (TestPathFile(path, filename + "rc", PathFull))
		return PathFull;
	if(TestPathFile(path, "." + filename + "rc", PathFull)) // hidden file
		return PathFull;
	if(TestPathFile(path, filename + ".conf", PathFull))
		return PathFull;
	if (TestPathFile(path,filename + ".conf.json", PathFull))
		return PathFull;

	if (!testDir)
		return {};
	std::filesystem::path PathDir = path;
	PathDir.append(filename);
	return TestPath(PathDir, filename, false, false);
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::filesystem::path GetPathConfig(const std::string& filename)
{
	if (filename.empty())
		return {};

	for (const auto& i : PathConfig::g_PathConfig)
	{
		bool CurrPath = i == ".";
#if defined(_WIN32)
		if (!CurrPath)
			CurrPath = i == ".."; // $(ProjectDir)
#endif
		std::filesystem::path PathItem = GetPathW(i);
		if (PathItem.empty())
			continue;
		std::filesystem::path Path = PathConfig::TestPath(PathItem, filename, CurrPath, true);
		if (!Path.empty())
			return Path;
	}

	return {};
}

std::filesystem::path GetPathConfigExc(const std::string& filename)
{
	std::filesystem::path Str = GetPathConfig(filename);
	if (Str.empty())
		throw std::runtime_error("File not found: " + filename);
	return Str;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions for tests
#if defined(_WIN32)
static std::string CmdLineWinTest(const std::string& cmd)
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
		std::filesystem::path Path = GetPathW(CmdValue);
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
///////////////////////////////////////////////////////////////////////////////////////////////////
}

}
