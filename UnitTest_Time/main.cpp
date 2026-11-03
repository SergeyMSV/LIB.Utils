#include <utilsTest.h>

namespace utils
{
	void UnitTest_Time();
}

int main()
{
	utils::UnitTest_Time();

	utils::test::RESULT_Total();

	return 0; 
}