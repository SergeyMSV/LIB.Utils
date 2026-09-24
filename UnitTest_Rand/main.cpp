#include <utilsTest.h>

namespace utils
{
	void UnitTest_Rand();
}

int main()
{
	utils::UnitTest_Rand();

	utils::test::RESULT_Total();

	return 0;
}
