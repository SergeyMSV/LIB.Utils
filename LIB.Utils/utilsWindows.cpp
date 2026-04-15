#include "utilsWindows.h"

namespace utils
{
namespace windows
{

std::optional<GUID> ParseGUID(std::string guidStr)
{
	if (guidStr.size() < 36)
		return {};
	if (guidStr.front() == '{' && guidStr.back() == '}')
		guidStr = guidStr.substr(1, guidStr.size() - 2);
	if (guidStr.size() != 36 || guidStr[8] != '-' || guidStr[13] != '-' || guidStr[18] != '-' || guidStr[23] != '-')
		return {};
	GUID Guid{};
	Guid.Data1 = std::strtoul(guidStr.substr(0, 8).c_str(), nullptr, 16);
	Guid.Data2 = static_cast<unsigned short>(std::strtoul(guidStr.substr(9, 4).c_str(), nullptr, 16));
	Guid.Data3 = static_cast<unsigned short>(std::strtoul(guidStr.substr(14, 4).c_str(), nullptr, 16));
	for (int i = 0, pos = 19; i < 8; ++i) // 8 - is equal to GUID::Data4 size.
	{
		if (i == 2)
			pos = 24;
		else if (i != 0)
			pos += 2;
		Guid.Data4[i] = static_cast<unsigned char>(std::strtoul(guidStr.substr(pos, 2).c_str(), nullptr, 16));
	}
	return Guid;
}

}
}
