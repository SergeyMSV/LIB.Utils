#include <utilsTest.h>

namespace utils
{
	void UnitTest_PacketNMEAType();
}

int main()
{
	utils::UnitTest_PacketNMEAType();

	utils::test::RESULT_Total();

	return 0;
}