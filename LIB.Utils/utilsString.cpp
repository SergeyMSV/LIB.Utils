#include "utilsString.h"

namespace utils
{
	namespace string
	{

std::optional<std::string> Replace(const std::string& str, const std::string& from, const std::string& to)
{
	size_t Pos = str.find(from);
	if (Pos == std::string::npos)
		return std::nullopt;
	std::string StrRes = str;
	return StrRes.replace(Pos, from.length(), to);
}

std::string GetStringEnding(const std::string& pattern, const std::string& str)
{
	size_t Pos = str.find(pattern);
	if (Pos == std::string::npos)
		return {};
	std::string Str = str.substr(Pos + pattern.size());
	std::string_view StrView = Str;
	StrView.remove_prefix(std::min(StrView.find_first_not_of(" "), StrView.size()));
	return StrView.data();
}

	}
}
