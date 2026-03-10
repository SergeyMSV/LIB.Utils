#include <utilsTest.h>

namespace utils
{
	void UnitTest_PacketNMEAPayload();
}

int main()
{
	utils::UnitTest_PacketNMEAPayload();

	utils::test::RESULT_Total();

	return 0;
}