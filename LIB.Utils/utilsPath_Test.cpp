#include "utilsPath.h"
#include "utilsTest.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace utils
{

void UnitTest_Path()
{
	std::cout << "\n""utils::path\n";

	{
		time_t TimeNow = 1660214334;// std::time(nullptr);
		tm* DateTime1 = std::localtime(&TimeNow);

		std::string DateTimeStr = GetDateTime(*DateTime1);
		
		utils::test::RESULT("DateTime String", DateTimeStr == "2022-08-11_13-38-54");

		tm DateTime2 = GetDateTime(DateTimeStr);

		utils::test::RESULT("DateTime Parse", memcmp(DateTime1, &DateTime2, sizeof(tm)));
	}

	{
		std::filesystem::create_directories("test_root_fs/tmpram");
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_12-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_13-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_14-11.jpg" };
		std::ofstream{"test_root_fs/tmpram/temp_file.txt" };

		std::deque<std::string> List = utils::GetFilesLatest("test_root_fs/tmpram", "cam_", 2);
		utils::test::RESULT("GetFilesList", List.size() == 2 &&
			List[0] == "test_root_fs/tmpram\\cam_2022-08-02_13-11.jpg" &&
			List[1] == "test_root_fs/tmpram\\cam_2022-08-02_14-11.jpg");

		std::filesystem::remove_all("test_root_fs/tmpram");
	}

	{
		std::filesystem::create_directories("test_root_fs/tmpram");
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_12-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_13-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_14-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/temp_file.txt" };

		RemoveFilesOutdated("test_root_fs/tmpram", "cam_", 1);
		std::deque<std::string> List = utils::GetFilesLatest("test_root_fs/tmpram", "cam_", 2);
		utils::test::RESULT("RemoveFilesOutdated", List.size() == 1 &&
			List[0] == "test_root_fs/tmpram\\cam_2022-08-02_14-11.jpg");

		std::filesystem::remove_all("test_root_fs/tmpram");
	}

	std::cout << std::endl;
}

}