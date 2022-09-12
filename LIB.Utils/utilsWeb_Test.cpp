#include "utilsTest.h"
#include "utilsWeb.h"

#include <iostream>

namespace utils
{

void UnitTest_WebHttpsReqSync()
{
	std::cout << "\n""utils::web::HttpsReqSync" << std::endl;

	{
		std::string Target = "/glonass/sostavOG/sostavOG_calc.php?lang=en&sort=point";

		std::optional<std::string> resData = web::HttpsReqSyncString("glonass-iac.ru", Target);

		test::RESULT("HttpsReqSync glonass-iac.ru: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}

	{
		std::string Target = "/uc?export=download&id=1y8pXWBZfJ9TinmAB0c-zvZr7Bo6X9IlQ";

		std::optional<std::string> resData = web::HttpsReqSyncString("drive.google.com", Target);

		test::RESULT("HttpsReqSync drive.google.com: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}

	{//Firware_mx5bull 253K zip
		std::string Target = "/uc?export=download&id=1QlVKkvL4bQr-xboYgFEknNpPciS06fgL";

		std::optional<tVectorUInt8> resData = web::HttpsReqSyncVector("drive.google.com", Target);

		test::RESULT("HttpsReqSync drive.google.com: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}

	{//Firware_mx9bull 4M zip
		std::string Target = "/uc?export=download&id=1iUwghHd3DG6x7fX-lU6tlcnEcVXjuPww";

		std::optional<tVectorUInt8> resData = web::HttpsReqSyncVector("drive.google.com", Target);

		test::RESULT("HttpsReqSync drive.google.com: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}

	{//Firware_mx10bull 4M zip encrypted
		std::string Target = "/uc?export=download&id=1oOCWlTlVYvMhDWIK41kJgDluIGkzyinS";

		std::optional<tVectorUInt8> resData = web::HttpsReqSyncVector("drive.google.com", Target);

		test::RESULT("HttpsReqSync drive.google.com: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}


	{//Firmware_mx4bull_2021-04-04 74M zip encrypted PDF
		std::string Target = "/uc?export=download&id=1GMslkxvoYfjRLIAVGSjAmj_lZSPg3jRa";

		std::optional<tVectorUInt8> resData = web::HttpsReqSyncVector("drive.google.com", Target);

		test::RESULT("HttpsReqSync drive.google.com: data received", resData.has_value());

		if (!resData.has_value())
		{
			const std::string Warning = "HttpsReqSync: check if Internet connection to the site is available";
			test::WARNING(Warning.c_str(), true);
		}
	}

	std::cout << std::endl;
}

}
