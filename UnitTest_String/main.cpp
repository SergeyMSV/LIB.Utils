#include <utilsTest.h>

namespace utils
{
	void UnitTest_String();
}

int main()
{
	utils::UnitTest_String();

	utils::test::RESULT_Total();

	return 0;
}