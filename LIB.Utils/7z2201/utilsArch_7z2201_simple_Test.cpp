#include "utilsArch_7z2201_simple.h"
#include <utilsPath.h>
#include <utilsTest.h>
#include <iostream>
#include <filesystem>

namespace utils
{

void UnitTest_Arch_7z2201()
{
	std::cout << "\n""utils::Arch_7z2201_simple\n";

	{
		bool Res = true;
		std::string Path = "test_root_fs/test.7z";
		try
		{
			utils::arch_7z2201_simple::Decompress(Path);
		}
		catch (const std::exception& e)
		{
			Res = false;
			std::cout << e.what() << '\n';
		}

		std::filesystem::remove_all("test_root_fs/test");

		utils::test::RESULT("Decompress test.7z", Res);
	}

	//{
	//	bool Res = true;
	//	std::string Path = "test_root_fs/test3.7z";
	//	try
	//	{
	//		utils::arch_7z2201_simple::Decompress(Path);
	//	}
	//	catch (const std::exception& e)
	//	{
	//		Res = false;
	//		std::cout << e.what() << '\n';
	//	}

	//	utils::test::RESULT("Decompress test3.7z", Res);
	//}

	std::cout << std::endl;
}

}