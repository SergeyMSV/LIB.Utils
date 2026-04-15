#include "utilsWindows.h"

#include "utilsTest.h"

namespace utils
{

void UnitTest_Windows_ParseGUID(std::string_view msg, const std::string& guidStr, const std::string& guidRes)
{
	std::optional<GUID> GuidOpt = utils::windows::ParseGUID(guidStr);
	char Guid_cstr[64]{};
	if (GuidOpt.has_value())
	{
		GUID Guid = *GuidOpt;
		snprintf(Guid_cstr, sizeof(Guid_cstr),
			"{%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			Guid.Data1,
			Guid.Data2,
			Guid.Data3,
			Guid.Data4[0], Guid.Data4[1],
			Guid.Data4[2], Guid.Data4[3], Guid.Data4[4], Guid.Data4[5], Guid.Data4[6], Guid.Data4[7]);
	}
	std::string GuidResStr = Guid_cstr;
	std::transform(GuidResStr.begin(), GuidResStr.end(), GuidResStr.begin(), [](unsigned char c) { return std::toupper(c); });
	utils::test::RESULT(std::string("ParseGUID ") + msg.data(), GuidResStr == guidRes);
}

void UnitTest_Windows_ParseGUID(std::string_view msg, const std::string& guidStr)
{
	return UnitTest_Windows_ParseGUID(msg, guidStr, guidStr);
}

void UnitTest_Windows()
{
	std::cout << "\n""utils::windows\n";

	UnitTest_Windows_ParseGUID("1", "{96B35CC0-69AB-4840-B8D3-CBB89A4A6232}");
	UnitTest_Windows_ParseGUID("2", "{A3BCCE2E-B1D0-4F7E-91F5-0BC5B354575D}");
	UnitTest_Windows_ParseGUID("3", "{96B35CC0-69AB-4840-B8D3-CBB89A4A6232}");
	UnitTest_Windows_ParseGUID("4", "{3A87D70F-BFE1-4C0B-9B76-FB172A4BC072}");
	UnitTest_Windows_ParseGUID("5", "{C9B04AB9-98CE-4947-B7BD-8E8A31101E18}");
	UnitTest_Windows_ParseGUID("6", "{7EC393CF-57AA-4AEF-839C-FC507FF9BDAB}");
	UnitTest_Windows_ParseGUID("7", "{}", "");
	UnitTest_Windows_ParseGUID("8", "", "");

	std::cout << std::endl;
}

}
