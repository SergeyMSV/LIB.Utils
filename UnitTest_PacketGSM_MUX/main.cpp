#include <utilsTest.h>

namespace utils
{
	void UnitTest_PacketMQTTv3_1_1();
}

int main()
{
	utils::UnitTest_PacketMQTTv3_1_1();

	utils::test::RESULT_Total();

	return 0;
}