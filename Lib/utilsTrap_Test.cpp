#include "utilsTrap.h"
#include "utilsTest.h"

#include <iostream>

namespace utils
{
	namespace unit_test
	{

const char UnitTest_Trap_Msg[] = "\xd\xa""CLOSED\xd\xa";
const char UnitTest_Trap_Msg_R[] = "\xa\xd""DESOLC\xa\xd";

void UnitTest_Trap()
{
	std::cout<<"\n\n""utils::trap::tTrap"<<std::endl;

	using namespace trap;

	//Test: The same
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xd\xa""CLOSED\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("The same", Result);
	}

	//Test: Reverse
	{
		tTrap<UnitTest_Trap_Msg_R, sizeof(UnitTest_Trap_Msg_R) - 1, true> Trap;

		char Data[] = "\xd\xa""CLOSED\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("Reverse", Result);
	}

	//Test: 0x0D + the same
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xd\xd\xa""CLOSED\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("0x0D + the same", Result);
	}

	//Test: 0x0D + 0x0D + the same
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xd\xd\xd\xa""CLOSED\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("0x0D + 0x0D + the same", Result);
	}


	//Test: No first 0x0D
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xa""CLOSED\xd\xa";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = !Trap.Try(DataVector);

		utils::test::RESULT("no first 0x0D", Result);
	}

	//Test: 0x0D + 0x0D + the same + rubbush
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xd\xd\xd\xa""CLOSED\xd\xa""CLOX";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("0x0D + 0x0D + the same + rubbush", Result);
	}

	//Test: 0x0D + 0x0D + rubbish + the same + rubbush
	{
		tTrap<UnitTest_Trap_Msg, sizeof(UnitTest_Trap_Msg) - 1> Trap;

		char Data[] = "\xd\xd\xd\xa""CLO""\xd\xa""CLOSED\xd\xa""CLOX";

		std::vector<char> DataVector(Data, Data + sizeof(Data));

		bool Result = Trap.Try(DataVector);

		utils::test::RESULT("0x0D + 0x0D + rubbish + the same + rubbush", Result);
	}


	std::cout << std::endl;
}

	}
}