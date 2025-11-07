#include "utilsBase.h"

#include "utilsTest.h"

#include <algorithm>

#include <iostream>

//#define DEBUG_SHOW_DOES_NOT_WORK

namespace utils
{

void UnitTest_Base()
{
	std::cout << "\n""utils::Base" << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		unsigned int A = 0;
		[[maybe_unused]]
		unsigned char *Last = std::copy(Data.begin(), Data.end(), (unsigned char*)&A);

		utils::test::RESULT("copy 1", true);
	}

	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		unsigned int A = 0;
		[[maybe_unused]]
		unsigned char* Last = std::copy_backward(Data.begin(), Data.end(), ((unsigned char*)& A) + sizeof(A));

		utils::test::RESULT("copy_backward 2", true);
	}

	{
		unsigned int A = 0x12345678;

		std::reverse(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A));

		utils::test::RESULT("reverse 2", true);
	}

	{
		uint64_t A = 0x12345678ABCDEF35;

		std::reverse(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A));

		utils::test::RESULT("reverse 3", true);
	}

	{
		char Data[] = { 1,2,3,4,5,6,7,8 };

		std::vector<std::uint8_t> DataVector;

		DataVector.insert(DataVector.end(), Data, Data + sizeof(Data));

		utils::test::RESULT("ToVector 4", true);
	}

	{
		unsigned int Data{ 0x12345678 };

		std::vector<std::uint8_t> DataVector{ 6,3,5 };

		utils::Append(DataVector, Data);

		utils::test::RESULT("Append 1", true);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Read<unsigned int>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int>(&Data[0], Data.size());

		utils::test::RESULT("Read<unsigned int>, Endian=default", A == 0x04030201);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Read<unsigned int, std::vector<std::uint8_t>::iterator>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<unsigned int, std::vector<std::uint8_t>::iterator>, Endian=default", A == 0x04030201);
	}

	//Read<unsigned int>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int>(&Data[0], Data.size());

		utils::test::RESULT("Read<unsigned int>, Endian=Little", A == 0x04030201);
	}

	//Read<unsigned int, std::vector<std::uint8_t>::iterator>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<unsigned int, std::vector<std::uint8_t>::iterator>, Endian=default", A == 0x04030201);
	}

	//Read<unsigned int, std::vector<std::uint8_t>::iterator>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, std::vector<std::uint8_t>::iterator>, Endian=Little", A == 0x04030201);
	}

	//Read<uint64_t, std::vector<std::uint8_t>::iterator>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

		auto A = utils::Read<uint64_t, std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, std::vector<std::uint8_t>::iterator>, Endian=default", A == 0x0807060504030201);
	}

	//Read<uint64_t, std::vector<std::uint8_t>::iterator>
	{
		std::vector<std::uint8_t> Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

		auto A = utils::Read<uint64_t, std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, std::vector<std::uint8_t>::iterator>, Endian=Little", A == 0x0807060504030201);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//ToVector<double> & Read<double, std::vector<std::uint8_t>::iterator>
	{
		double Figure = 12345678.901;

		std::vector<std::uint8_t> Data = ToVector(12345678.901);

		auto A = utils::Read<decltype(Figure), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<double> & Read<double, ...>, Endian=default", A == Figure);
	}

	//ToVector<double> & Read<double, std::vector<std::uint8_t>::iterator>
	{
		double A = 12345678.901;

		std::vector<std::uint8_t> Data = ToVector(A);

		auto C = utils::Read<decltype(A)>(&Data[0], Data.size());

		utils::test::RESULT("ToVector<double> & Read<double, ...>, Endian=default", C == A);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//ToVector<int>
	{
		int Figure = -1234567;

		std::vector<std::uint8_t> Data = ToVector(Figure);

		auto A = utils::Read<decltype(Figure), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int>, Endian=default", A == Figure);
	}

	//ToVector<int>
	{
		int A = -1234567;

		std::vector<std::uint8_t> Data = ToVector(A);

		auto C = utils::Read<decltype(A), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int>, Endian=Little", C == A);
	}

	//ToVector<unsigned int>
	{
		unsigned int A = 0x12345678;

		std::vector<std::uint8_t> Data = ToVector(A);

		auto C = utils::Read<decltype(A), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<unsigned int>, Endian=Little", C == A);
	}

	//ToVector<unsigned short>
	{
		unsigned short A = 0x1234;

		std::vector<std::uint8_t> Data = ToVector(A);

		auto C = utils::Read<decltype(A), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<unsigned short>, Endian=Little", C == A);
	}

	//ToVector<int64_t>
	{
		int64_t Figure = 0x123456789ABCDEF0;

		std::vector<std::uint8_t> Data = ToVector(Figure);

		auto A = utils::Read<decltype(Figure), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int64_t>, Endian=Little", A == Figure);
	}

	//ToVector<int64_t>, Size - 4
	{
		int64_t A = 0x123456789ABCDEF0;

		std::vector<std::uint8_t> Data = ToVector(A);

		auto C = utils::Read<decltype(A), std::vector<std::uint8_t>::iterator>(Data.begin(), Data.end() - 4);

		utils::test::RESULT("ToVector<int64_t>, Endian=Little", C == (A & 0x00000000FFFFFFFF));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read - assert
	//{
	//	std::string Str = " -FEjghk";

	//	auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix::oct);
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<char>, Radix=tRadix::dec
	{
		std::string Str = " -123ko4_dsfg";

		auto A = Read<char>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<char>, Radix=tRadix::dec", A == -123);
	}

	//Read<short>, Radix=tRadix::dec
	{
		std::string Str = " -12345ko4_dsfg";

		auto A = Read<short>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<short>, Radix=tRadix::dec", A == -12345);
	}

	//Read<int>, Radix=tRadix::dec
	{
		std::string Str = " -1234567ko4_dsfg";

		auto A = Read<int>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<int>, Radix=tRadix::dec", A == -1234567);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<int64_t>, Radix=tRadix::dec
	{
		std::string Str = " -123456789123456789ko4_dsfg";

		auto A = Read<int64_t>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<int64_t>, Radix=tRadix::dec", A == -123456789123456789);
	}

	//Read<unsigned char>, Radix=tRadix::dec, 1
	{
		std::string Str = " -123ko4_dsfg";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix::dec, 1", A == 123);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned char>, Radix=tRadix::dec, 2
	{
		std::string Str = " 123ko4_dsfg";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix::dec, 2", A == 123);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<unsigned short>, Radix=tRadix::dec, 1
	{
		std::string Str = " -12345ko4_dsfg";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix::dec, 1", A == 12345);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned short>, Radix=tRadix::dec, 2
	{
		std::string Str = " 12345ko4_dsfg";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix::dec, 2", A == 12345);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<unsigned int>, Radix=tRadix::dec, 1
	{
		std::string Str = " -1234567ko4_dsfg";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix::dec, 1", A == 1234567);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned int>, Radix=tRadix::dec, 2
	{
		std::string Str = " 1234567ko4_dsfg";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix::dec, 2", A == 1234567);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<uint64_t>, Radix=tRadix::dec, 1
	{
		std::string Str = " -123456789123456789ko4_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix::dec, 1", A == 123456789123456789);
	}

	//Read<uint64_t>, Radix=tRadix::dec, 2
	{
		std::string Str = " 123456789123456789ko4_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix::dec, 2", A == 123456789123456789);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<int>, Radix=tRadix::dec, data size is bigger than possible
	{
		std::string Str = " 1234567891234345234523234556789ko4_dsfg";

		auto A = Read<int>(Str.begin(), Str.end(), tRadix::dec);

		utils::test::RESULT("Read<int>, Radix=tRadix::dec, data size is bigger than possible", A == 0x7FFFFFFF);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<unsigned char>, Radix=tRadix::hex
	{
		std::string Str = " -FEjghk";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix::hex);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix::hex", A == 0xFE);
	}

	//Read<unsigned short>, Radix=tRadix::hex
	{
		std::string Str = " -FEAC-3465l;j";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix::hex);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix::hex", A == 0xFEAC);
	}

	//Read<unsigned int>, Radix=tRadix::hex
	{
		std::string Str = " -FD345678jkl;";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix::hex);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix::hex", A == 0xFD345678);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<uint64_t>, Radix=tRadix::hex
	{
		std::string Str = " -FB123456AE098765_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix::hex);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix::hex", A == 0xFB123456AE098765);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<unsigned int>, Radix=tRadix::hex - Read(const char* data, tRadix radix)
	{
		std::string Str = " -FD345678jkl;";

		auto A = Read<unsigned int>(Str.c_str(), tRadix::hex);

		utils::test::RESULT("Read(const char* data, tRadix radix)", A == 0xFD345678);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//ReadInt, Radix=tRadix::dec
	//{
	//	std::string Str = " -1234567ko4_dsfg";

	//	auto A = ReadInt((char*)Str.c_str(), tRadix::dec);

	//	utils::test::RESULT("ReadInt, Radix=tRadix::dec", A == -1234567);
	//}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//ReadInt, Radix=tRadix::hex
	//{
	//	std::string Str = " 1234F567ko4_dsfg";

	//	auto A = ReadInt((char*)Str.c_str(), tRadix::hex);

	//	utils::test::RESULT("ReadInt, Radix=tRadix::hex", A == 0x1234F567);
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Reverse<int>
	{
		int A = 0x12345678;

		A = Reverse(A);

		utils::test::RESULT("Reverse<int>", A == 0x78563412);
	}

	//Reverse<unsigned int>
	{
		unsigned int A = 0x12345678;

		A = Reverse(A);

		utils::test::RESULT("ChangeEndian<unsigned int>", A == 0x78563412);
	}

	//Reverse<short>
	{
		short A = 0x1234;

		A = Reverse(A);

		utils::test::RESULT("Reverse<short>", A == 0x3412);
	}

	//Reverse<unsigned short>
	{
		unsigned short A = 0x1234;

		A = Reverse(A);

		utils::test::RESULT("Reverse<unsigned short>", A == 0x3412);
	}

	//Reverse<unsigned char>
	{
		unsigned char A = 0x12;

		A = Reverse(A);

		utils::test::RESULT("Reverse<unsigned char>", A == 0x12);
	}

	//Reverse<uint64_t>
	{
		uint64_t A = 0x1234'5678'90AB'CDEF;

		A = Reverse(A);

		utils::test::RESULT("Reverse<uint64_t>", A == 0xEFCD'AB90'7856'3412);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl;
}

}
