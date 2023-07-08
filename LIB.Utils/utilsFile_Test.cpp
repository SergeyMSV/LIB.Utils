#include "utilsFile.h"
#include "utilsTest.h"
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace utils
{

void UnitTest_File()
{
	std::cout << "\n""utils::file\n";

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "cam_", "pic_");
		utils::test::RESULT("Replace 1", Res.has_value() && *Res == "/tmpram/somefolder/pic_2023-07-07_21-00.jpg");
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "cam", "pic");
		utils::test::RESULT("Replace 2", Res.has_value() && *Res == "/tmpram/somefolder/pic_2023-07-07_21-00.jpg");
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "am", "pic");
		utils::test::RESULT("Replace 3", !Res.has_value());
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "some", "pic");
		utils::test::RESULT("Replace 4", !Res.has_value());
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "cam_", "picture_");
		utils::test::RESULT("Replace 5", Res.has_value() && *Res == "/tmpram/somefolder/picture_2023-07-07_21-00.jpg");
	}

	{
		std::string A = "/tmpram/somefolder/cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "cam_", "p");
		utils::test::RESULT("Replace 6", Res.has_value() && *Res == "/tmpram/somefolder/p2023-07-07_21-00.jpg");
	}

	{
		std::string A = "cam_2023-07-07_21-00.jpg";
		std::optional<std::string> Res = utils::file::ReplaceFileNamePrefix(A, "cam_", "picture_");
		utils::test::RESULT("Replace 7", Res.has_value() && *Res == "picture_2023-07-07_21-00.jpg");
	}

	{
		std::filesystem::create_directories("test_root_fs/tmpram");
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_12-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_13-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_14-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/temp_file.txt" };

		std::deque<std::string> List = utils::file::GetFilesLatest("test_root_fs/tmpram", "cam_", 2);
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

		utils::file::RemoveFilesOutdated("test_root_fs/tmpram", "cam_", 1);
		std::deque<std::string> List = utils::file::GetFilesLatest("test_root_fs/tmpram", "cam_", 2);
		utils::test::RESULT("RemoveFilesOutdated", List.size() == 1 &&
			List[0] == "test_root_fs/tmpram\\cam_2022-08-02_14-11.jpg");

		std::filesystem::remove_all("test_root_fs/tmpram");
	}

	{
		std::filesystem::create_directories("test_root_fs/tmpram");
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_12-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_13-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/cam_2022-08-02_14-11.jpg" };
		std::ofstream{ "test_root_fs/tmpram/temp_file.txt" };

		utils::file::RemoveFilesWithPrefix("test_root_fs/tmpram", "cam_");
		std::deque<std::string> List = utils::file::GetFilesLatest("test_root_fs/tmpram", "cam_", 2);
		utils::test::RESULT("RemoveFilesWithPrefix", List.empty());

		std::filesystem::remove_all("test_root_fs/tmpram");
	}

	std::cout << std::endl;
}

}