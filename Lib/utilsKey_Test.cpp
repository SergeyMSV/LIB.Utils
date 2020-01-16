#include "utilsKey.h"

#include "utilsTest.h"

#include <algorithm>

#include <iostream>

//#define DEBUG_SHOW_DOES_NOT_WORK

namespace utils
{

void UnitTest_Key()
{
	std::cout << "\n""utils::Key" << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////

	{
		tKey256 Key_1;
		tKey256 Key_2;

		type::tKey2<8> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey256 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey256 !=", Key_1 != Key_2);
	}

	{
		tKey192 Key_1;
		tKey192 Key_2;

		type::tKey2<6> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey192 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey192 !=", Key_1 != Key_2);
	}

	{
		tKey128 Key_1;
		tKey128 Key_2;

		type::tKey2<4> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey128 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey128 !=", Key_1 != Key_2);
	}

	{
		tKey96 Key_1;
		tKey96 Key_2;

		type::tKey2<3> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey96 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey96 !=", Key_1 != Key_2);
	}

	{
		tKey64 Key_1;
		tKey64 Key_2;

		type::tKey2<2> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey64 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey64 !=", Key_1 != Key_2);
	}

	{
		tKey32 Key_1;
		tKey32 Key_2;

		type::tKey2<1> Key2;

		Key_1.Value = Key2;
		Key_2.Value = Key2;

		utils::test::RESULT("tKey32 ==", Key_1 == Key_2);

		Key_1.Field.A = 4;

		utils::test::RESULT("tKey32 !=", Key_1 != Key_2);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	{
		tKey256 Key;
		Key.Field.A = 0x01;
		Key.Field.B = 0x02;
		Key.Field.C = 0x03;
		Key.Field.D = 0x04;
		Key.Field.E = 0x05;
		Key.Field.F = 0x06;
		Key.Field.G = 0x07;
		Key.Field.H = 0x08;

		std::string Str = ToString(Key);

		utils::test::RESULT("ToString() tKey256 !=", Str == "00000008-00000007-00000006-00000005-00000004-00000003-00000002-00000001");
	}

	{
		tKey128 Key;
		Key.Field.A = 0x01;
		Key.Field.B = 0x02;
		Key.Field.C = 0x03;
		Key.Field.D = 0x04;

		std::string Str = ToString(Key);

		utils::test::RESULT("ToString() tKey128 !=", Str == "00000004-00000003-00000002-00000001");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	{
		tKey256 Key;
		Key.Field.A = 0x01;
		Key.Field.B = 0x02;
		Key.Field.C = 0x03;
		Key.Field.D = 0x04;
		Key.Field.E = 0x05;
		Key.Field.F = 0x06;
		Key.Field.G = 0x07;
		Key.Field.H = 0x08;

		tVectorUInt8 Data = ToVector(Key);

		utils::test::RESULT("ToVector() tKey256 !=", Data == tVectorUInt8{ 1,0,0,0,2,0,0,0,3,0,0,0,4,0,0,0,5,0,0,0,6,0,0,0,7,0,0,0,8,0,0,0 });
	}

	{
		tKey128 Key;
		Key.Field.A = 0x01;
		Key.Field.B = 0x02;
		Key.Field.C = 0x03;
		Key.Field.D = 0x04;

		tVectorUInt8 Data = utils::ToVector(Key);

		utils::test::RESULT("ToVector() tKey128 !=", Data == tVectorUInt8{ 1,0,0,0,2,0,0,0,3,0,0,0,4,0,0,0 });
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////


	std::cout << std::endl;
}

}
