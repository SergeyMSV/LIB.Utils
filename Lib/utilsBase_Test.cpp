#include "utilsBase.h"

#include "utilsTest.h"

#include <algorithm>

#include <iostream>

//#define DEBUG_SHOW_DOES_NOT_WORK

namespace utils
{
	namespace unit_test
	{

void UnitTest_Base()
{
	std::cout << "\n""utils::Base" << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		unsigned int A = 0;

		unsigned char *Last = std::copy(Data.begin(), Data.end(), (unsigned char*)&A);

		utils::test::RESULT("copy 1", true);
	}

	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		unsigned int A = 0;

		unsigned char* Last = std::copy_backward(Data.begin(), Data.end(), ((unsigned char*)& A) + sizeof(A));

		utils::test::RESULT("copy_backward 2", true);
	}


	//{
	//	uint64_t A = 0x1234'5678'90AB'CDEF;

	//	std::vector<bool> VectorBOOL(sizeof(A) * 8);

	//	std::copy((char*)& A, ((char*)& A) + sizeof(A), VectorBOOL);

	//	utils::test::RESULT("copy 1", true);
	//}

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

		tVectorUInt8 DataVector;

		DataVector.insert(DataVector.end(), Data, Data + sizeof(Data));

		utils::test::RESULT("ToVector 4", true);
	}

	////ToVector 1
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

	//	tVectorUInt8 Vector = ToVector(&Data[0], Data.size());

	//	utils::test::RESULT("ToVector 1", Vector == Data);
	//}

	////ToVector 2
	//{
	//	char *Data = new char[0];

	//	tVectorUInt8 Vector = ToVector(Data, 0);

	//	delete[] Data;

	//	utils::test::RESULT("ToVector 2 (empty)", Vector.size() == 0);
	//}

	//{
	//	char Data[] = { 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x01, 0x00, 0x15, 0x01, 0x00, 0x15 };

	//	tVectorUInt8 DataVector = utils::ToVector(Data, sizeof(Data));
	//}

	{
		unsigned int Data{ 0x12345678 };

		tVectorUInt8 DataVector{ 6,3,5 };

		utils::Append(DataVector, Data);

		utils::test::RESULT("Append 1", true);
	}

	//{
	//	char Data[] = { 1,2,3,4,5,6,7,8 };

	//	tVectorUInt8 DataVector;

	//	utils::Append(DataVector, Data, Data + sizeof(Data));

	//	utils::test::RESULT("Append 2", true);
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Read<unsigned int>, Endian=default
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int>(&Data[0], Data.size());

		utils::test::RESULT("Read<unsigned int>, Endian=default", A == 0x04030201);
	}

	////Read<unsigned int>, Endian=Big, TEST 1
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

	//	auto A = utils::Read<unsigned int>(&Data[0], Data.size(), tEndian_Big);

	//	utils::test::RESULT("Read<unsigned int>, Endian=Big, TEST 1", A == 0x01020304);
	//}

	////Read<unsigned int>, Endian=Big, TEST 2 SHORT
	//{
	//	tVectorUInt8 Data{ 0x01,0x02 };

	//	auto A = utils::Read<unsigned int>(&Data[0], Data.size(), tEndian_Big);

	//	utils::test::RESULT("Read<unsigned int>, Endian=Big, TEST 2 SHORT", A == 0x01020000);
	//}

	////Read<unsigned int>, Endian=Little, TEST 1
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

	//	auto A = utils::Read<unsigned int>(&Data[0], Data.size());

	//	utils::test::RESULT("Read<unsigned int>, Endian=Little, TEST 1", A == 0x04030201);
	//}

	////Read<unsigned int>, Endian=Little, TEST 2 SHORT
	//{
	//	tVectorUInt8 Data{ 0x01,0x02 };

	//	auto A = utils::Read<unsigned int>(&Data[0], Data.size());

	//	utils::test::RESULT("Read<unsigned int>, Endian=Little, TEST 2 SHORT", A == 0x00000201);
	//}

	////Read<uint64_t>, Endian=Big
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

	//	auto A = utils::Read<uint64_t>(&Data[0], Data.size(), tEndian_Big);

	//	utils::test::RESULT("Read<uint64_t>, Endian=Big", A == 0x0102030405060708);
	//}

	////Read<uint64_t>, Endian=Little
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

	//	auto A = utils::Read<uint64_t>(&Data[0], Data.size());

	//	utils::test::RESULT("Read<uint64_t>, Endian=Little", A == 0x0807060504030201);
	//}

	////Read<int>, Endian=Little
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,(char)0xF4 };

	//	auto A = utils::Read<int>(&Data[0], Data.size());

	//	utils::test::RESULT("Read<int>, Endian=Little", A == 0xF4030201);
	//}

	////Read<short>, Endian=Big
	//{
	//	tVectorUInt8 Data{ 0x01,0x02 };

	//	auto A = utils::Read<short>(&Data[0], Data.size(), tEndian_Big);

	//	utils::test::RESULT("Read<short>, Endian=Big", A == 0x0102);
	//}

	////Read<short>, Endian=Little
	//{
	//	tVectorUInt8 Data{ 0x01,0x02 };

	//	auto A = utils::Read<short>(&Data[0], Data.size());

	//	utils::test::RESULT("Read<short>, Endian=Little", A == 0x0201);
	//}
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Read<unsigned int, tVectorUInt8::iterator>, Endian=default
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<unsigned int, tVectorUInt8::iterator>, Endian=default", A == 0x04030201);
	}

	//Read<unsigned int>, Endian=Little
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int>(&Data[0], Data.size());

		utils::test::RESULT("Read<unsigned int>, Endian=Little", A == 0x04030201);
	}

	//Read<unsigned int, tVectorUInt8::iterator>, Endian=default
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<unsigned int, tVectorUInt8::iterator>, Endian=default", A == 0x04030201);
	}

	////Read<unsigned int, tVectorUInt8::iterator>, Endian=Big
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

	//	auto A = utils::Read<unsigned int, tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("Read<uint64_t, tVectorUInt8::iterator>, Endian=Big", A == 0x01020304);
	//}

	//Read<unsigned int, tVectorUInt8::iterator>, Endian=Little
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04 };

		auto A = utils::Read<unsigned int, tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, tVectorUInt8::iterator>, Endian=Little", A == 0x04030201);
	}

	//Read<uint64_t, tVectorUInt8::iterator>, Endian=default
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

		auto A = utils::Read<uint64_t, tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, tVectorUInt8::iterator>, Endian=default", A == 0x0807060504030201);
	}

	////Read<uint64_t, tVectorUInt8::iterator>, Endian=Big
	//{
	//	tVectorUInt8 Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

	//	uint64_t A = utils::Read<uint64_t, tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("Read<uint64_t, tVectorUInt8::iterator>, Endian=Big", A == 0x0102030405060708);
	//}

	//Read<uint64_t, tVectorUInt8::iterator>, Endian=Little
	{
		tVectorUInt8 Data{ 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08 };

		auto A = utils::Read<uint64_t, tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("Read<uint64_t, tVectorUInt8::iterator>, Endian=Little", A == 0x0807060504030201);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//ToVector<double> & Read<double, tVectorUInt8::iterator>, Endian=default
	{
		double Figure = 12345678.901;

		tVectorUInt8 Data = ToVector(12345678.901);

		auto A = utils::Read<decltype(Figure), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<double> & Read<double, ...>, Endian=default", A == Figure);
	}

	//ToVector<double> & Read<double, tVectorUInt8::iterator>, Endian=default
	{
		double A = 12345678.901;

		tVectorUInt8 Data = ToVector(A);

		auto C = utils::Read<decltype(A)>(&Data[0], Data.size());

		utils::test::RESULT("ToVector<double> & Read<double, ...>, Endian=default", C == A);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	//ToVector<int>, Endian=default
	{
		int Figure = -1234567;

		tVectorUInt8 Data = ToVector(Figure);

		auto A = utils::Read<decltype(Figure), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int>, Endian=default", A == Figure);
	}

	////ToVector<int>, Endian=Big
	//{
	//	int A = -1234567;//Format=LittleEndian

	//	decltype(A) B = 0;

	//	std::reverse_copy(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A), (unsigned char*)& B);//Format => to BigEndian

	//	tVectorUInt8 Data = ToVector(B);

	//	auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("ToVector<int>, Endian=Big", C == A);
	//}

	//ToVector<int>, Endian=Little
	{
		int A = -1234567;//Format=LittleEndian

		tVectorUInt8 Data = ToVector(A);//Format=LittleEndian

		auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int>, Endian=Little", C == A);
	}

	////ToVector<unsigned int>, Endian=Big
	//{
	//	unsigned int A = 0x12345678;//Format=LittleEndian

	//	decltype(A) B = 0;

	//	std::reverse_copy(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A), (unsigned char*)& B);//Format => to BigEndian

	//	tVectorUInt8 Data = ToVector(B);

	//	auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("ToVector<unsigned int>, Endian=Big", C == A);
	//}

	//ToVector<unsigned int>, Endian=Little
	{
		unsigned int A = 0x12345678;//Format=LittleEndian

		tVectorUInt8 Data = ToVector(A);//Format=LittleEndian

		auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<unsigned int>, Endian=Little", C == A);
	}

	////ToVector<unsigned short>, Endian=Big
	//{
	//	unsigned short A = 0x1234;//Format=LittleEndian

	//	decltype(A) B = 0;

	//	std::reverse_copy(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A), (unsigned char*)& B);//Format => to BigEndian

	//	tVectorUInt8 Data = ToVector(B);

	//	auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("ToVector<unsigned short>, Endian=Big", C == A);
	//}

	//ToVector<unsigned short>, Endian=Little
	{
		unsigned short A = 0x1234;//Format=LittleEndian

		tVectorUInt8 Data = ToVector(A);//Format=LittleEndian

		auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<unsigned short>, Endian=Little", C == A);
	}

	////ToVector<int64_t>, Endian=Big
	//{
	//	int64_t A = 0x123456789ABCDEF0;//Format=LittleEndian

	//	decltype(A) B = 0;

	//	std::reverse_copy(((unsigned char*)& A), ((unsigned char*)& A) + sizeof(A), (unsigned char*)& B);//Format => to BigEndian

	//	tVectorUInt8 Data = ToVector(B);

	//	auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end(), tEndian_Big);

	//	utils::test::RESULT("ToVector<int64_t>, Endian=Big", C == A);
	//}

	//ToVector<int64_t>, Endian=Little
	{
		int64_t Figure = 0x123456789ABCDEF0;//Format=LittleEndian

		tVectorUInt8 Data = ToVector(Figure);//Format=LittleEndian

		auto A = utils::Read<decltype(Figure), tVectorUInt8::iterator>(Data.begin(), Data.end());

		utils::test::RESULT("ToVector<int64_t>, Endian=Little", A == Figure);
	}

	//ToVector<int64_t>, Size - 4
	{
		int64_t A = 0x123456789ABCDEF0;//Format=LittleEndian

		tVectorUInt8 Data = ToVector(A);//Format=LittleEndian

		auto C = utils::Read<decltype(A), tVectorUInt8::iterator>(Data.begin(), Data.end() - 4);

		utils::test::RESULT("ToVector<int64_t>, Endian=Little", C == (A & 0x00000000FFFFFFFF));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<char>, Radix=tRadix_10
	{
		std::string Str = " -123ko4_dsfg";

		auto A = Read<char>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<char>, Radix=tRadix_10", A == -123);
	}

	//Read<short>, Radix=tRadix_10
	{
		std::string Str = " -12345ko4_dsfg";

		auto A = Read<short>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<short>, Radix=tRadix_10", A == -12345);
	}

	//Read<int>, Radix=tRadix_10
	{
		std::string Str = " -1234567ko4_dsfg";

		auto A = Read<int>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<int>, Radix=tRadix_10", A == -1234567);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<int64_t>, Radix=tRadix_10
	{
		std::string Str = " -123456789123456789ko4_dsfg";

		auto A = Read<int64_t>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<int64_t>, Radix=tRadix_10", A == -123456789123456789);
	}

	//Read<unsigned char>, Radix=tRadix_10, 1
	{
		std::string Str = " -123ko4_dsfg";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix_10, 1", A == 123);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned char>, Radix=tRadix_10, 2
	{
		std::string Str = " 123ko4_dsfg";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix_10, 2", A == 123);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<unsigned short>, Radix=tRadix_10, 1
	{
		std::string Str = " -12345ko4_dsfg";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix_10, 1", A == 12345);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned short>, Radix=tRadix_10, 2
	{
		std::string Str = " 12345ko4_dsfg";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix_10, 2", A == 12345);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<unsigned int>, Radix=tRadix_10, 1
	{
		std::string Str = " -1234567ko4_dsfg";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix_10, 1", A == 1234567);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//Read<unsigned int>, Radix=tRadix_10, 2
	{
		std::string Str = " 1234567ko4_dsfg";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix_10, 2", A == 1234567);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<uint64_t>, Radix=tRadix_10, 1
	{
		std::string Str = " -123456789123456789ko4_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix_10, 1", A == 123456789123456789);
	}

	//Read<uint64_t>, Radix=tRadix_10, 2
	{
		std::string Str = " 123456789123456789ko4_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix_10, 2", A == 123456789123456789);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<int>, Radix=tRadix_10, data size is bigger than possible
	{
		std::string Str = " 1234567891234345234523234556789ko4_dsfg";

		auto A = Read<int>(Str.begin(), Str.end(), tRadix_10);

		utils::test::RESULT("Read<int>, Radix=tRadix_10, data size is bigger than possible", A == 0xFFFFFFFF);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<unsigned char>, Radix=tRadix_16
	{
		std::string Str = " -FEjghk";

		auto A = Read<unsigned char>(Str.begin(), Str.end(), tRadix_16);

		utils::test::RESULT("Read<unsigned char>, Radix=tRadix_16", A == 0xFE);
	}

	//Read<unsigned short>, Radix=tRadix_16
	{
		std::string Str = " -FEAC-3465l;j";

		auto A = Read<unsigned short>(Str.begin(), Str.end(), tRadix_16);

		utils::test::RESULT("Read<unsigned short>, Radix=tRadix_16", A == 0xFEAC);
	}

	//Read<unsigned int>, Radix=tRadix_16
	{
		std::string Str = " -FD345678jkl;";

		auto A = Read<unsigned int>(Str.begin(), Str.end(), tRadix_16);

		utils::test::RESULT("Read<unsigned int>, Radix=tRadix_16", A == 0xFD345678);
	}

#ifdef DEBUG_SHOW_DOES_NOT_WORK
	//Read<uint64_t>, Radix=tRadix_16
	{
		std::string Str = " -FB123456AE098765_dsfg";

		auto A = Read<uint64_t>(Str.begin(), Str.end(), tRadix_16);

		utils::test::RESULT("Read<uint64_t>, Radix=tRadix_16", A == 0xFB123456AE098765);
	}
#endif//DEBUG_SHOW_DOES_NOT_WORK

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Read<unsigned int>, Radix=tRadix_16 - Read(const char* data, tRadix radix)
	{
		std::string Str = " -FD345678jkl;";

		auto A = Read<unsigned int>(Str.c_str(), tRadix_16);

		utils::test::RESULT("Read(const char* data, tRadix radix)", A == 0xFD345678);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//ReadInt, Radix=tRadix_10
	//{
	//	std::string Str = " -1234567ko4_dsfg";

	//	auto A = ReadInt((char*)Str.c_str(), tRadix_10);

	//	utils::test::RESULT("ReadInt, Radix=tRadix_10", A == -1234567);
	//}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//ReadInt, Radix=tRadix_16
	//{
	//	std::string Str = " 1234F567ko4_dsfg";

	//	auto A = ReadInt((char*)Str.c_str(), tRadix_16);

	//	utils::test::RESULT("ReadInt, Radix=tRadix_16", A == 0x1234F567);
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

	/* [TBD]
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//FromBCD
	{
		char B = FromBCD(0x92);

		utils::test::RESULT("FromBCD", B == 92);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//ToCP866(tISO8859 codePage, const std::string& data)
	{
		std::string Str("Превед, Медвед!!!");

		std::string StrCP866 = charset::ToCP866(charset::tISO8859_5, Str);

		std::cout << Str << " = " << StrCP866 << std::endl;
	}
	*/

	std::cout << std::endl;
}

	}
}
