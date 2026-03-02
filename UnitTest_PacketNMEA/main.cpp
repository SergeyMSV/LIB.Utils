#include <utilsTest.h>

namespace utils
{
	void UnitTest_PacketNMEA();
}

int main()
{
	utils::UnitTest_PacketNMEA();

	utils::test::RESULT_Total();

	return 0;
}