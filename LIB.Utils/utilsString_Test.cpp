#include "utilsString.h"

#include "utilsTest.h"

#include <functional>

namespace utils
{

void UnitTest_String_TrimString(std::string_view msg, std::string_view raw, std::string_view res)
{
	utils::test::RESULT(std::string("TrimString ") + msg.data(), utils::string::TrimString(raw) == res);
}

void UnitTest_String_SplitString(std::function<std::vector<std::string_view>(std::string_view str, char delimiter)> fun, std::string_view msg, std::string_view raw, std::vector<std::string_view> res)
{
	std::vector<std::string_view> Strs = fun(raw, ',');

	bool Result = Strs.size() == res.size();
	for (std::size_t i = 0; Result && i < Strs.size(); ++i)
	{
		if (Strs[i] != res[i])
			Result = false;
	}

	utils::test::RESULT(std::string("SplitString ") + msg.data(), Result);
}

void UnitTest_String()
{
	UnitTest_String_TrimString("test 1", "		  sdfsdf   4234		", "sdfsdf   4234");
	UnitTest_String_TrimString("test 2", "", "");
	UnitTest_String_TrimString("test 3", " ", "");
	UnitTest_String_SplitString(utils::string::SplitString, "test 1", "as,dfg", {"as", "dfg"});
	UnitTest_String_SplitString(utils::string::SplitString, "test 2", "as", { "as" });
	UnitTest_String_SplitString(utils::string::SplitString, "test 3", "", { });
	UnitTest_String_SplitString(utils::string::SplitString, "test 4", ",as", { "", "as" });
	UnitTest_String_SplitString(utils::string::SplitString, "test 5", ",", { "" });
	UnitTest_String_SplitString(utils::string::SplitString, "test 6", "q,w,e,t", { "q","w","e","t" });
	UnitTest_String_SplitString(utils::string::SplitTrimString, "test 7", " q		,	w  ,   e	,  t	", { "q","w","e","t" });
	utils::test::RESULT(std::string("Contains 1"), utils::string::Contains("   asd   ","asd"));
	utils::test::RESULT(std::string("Contains 2"), utils::string::Contains("   asd", "asd"));
	utils::test::RESULT(std::string("Contains 3"), utils::string::Contains("asd   ", "asd"));
	utils::test::RESULT(std::string("Contains 4"), utils::string::Contains("asd", "asd"));
}

}
