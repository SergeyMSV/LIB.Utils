#include <utilsTest.h>

namespace utils
{
	void UnitTest_Linux();
}

int main()
{
	utils::UnitTest_Linux();

	utils::test::RESULT_Total();

	return 0;
}