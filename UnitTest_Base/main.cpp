#include <utilsTest.h>

namespace utils
{
	void UnitTest_Base();
}

int main()
{
	utils::UnitTest_Base();

	utils::test::RESULT_Total();

	return 0;
}