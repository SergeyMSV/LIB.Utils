#include "utilsPacketNMEAType.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>
//#include <tuple>
#include <type_traits>

namespace utils
{

using namespace utils::packet::nmea::type;

template <class T>
void UnitTest_PacketNMEAType_Test(const std::string& msg, const std::string& res)
{
	T A;
	//std::cout << A.ToString() << '\n';
	utils::test::RESULT(msg, A.ToString() == res);
	//tOptional<T> Val;
	//std::cout << Val.ToString() << '\n';
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg, const std::string& msg,  const std::string& res)
{
	if constexpr (std::is_same_v<TArg, const char*>)
	{
		std::string ArgStr(arg);
		T A(ArgStr);
		//std::cout << A.ToString() << '\n';
		utils::test::RESULT(msg, A.ToString() == res);
	}
	else
	{
		T A(arg);
		//std::cout << A.ToString() << '\n';
		utils::test::RESULT(msg, A.ToString() == res);
	}

	//if constexpr (std::is_same_v<TArg, const char*>)
	//{
	//	tOptional<T> Val = T::Parse(arg);
	//	std::cout << Val.ToString() << '\n';
	//}
	//else
	//{
	//	tOptional<T> Val = T(arg);
	//	std::cout << Val.ToString() << '\n';
	//}
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg1, TArg arg2, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2);
	//std::cout << A.ToString() << '\n';
	utils::test::RESULT(msg, A.ToString() == res);

	//if constexpr (std::is_same_v<TArg, const char*>)
	//{
	//	tOptional<T> Val = T::Parse(arg1, arg2);
	//	std::cout << Val.ToString() << '\n';
	//}
	//else
	//{
	//	tOptional<T> Val(T(arg1, arg2));
	//	std::cout << Val.ToString() << '\n';
	//}
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg1, TArg arg2, TArg arg3, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2, arg3);
	//std::cout << A.ToString() << '\n';
	utils::test::RESULT(msg, A.ToString() == res);
}
/*
template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg1, TArg arg2, TArg arg3, TArg arg4)
{
	tOptional<T> Val(T(arg1, arg2, arg3, arg4));
	std::cout << Val.ToString() << '\n';
}

template <class T, class TArg1, class TArg2>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2)
{
	if constexpr (std::is_same_v<TArg2, char>)
	{
		tOptional<T> Val(T(arg1, arg2));
		std::cout << Val.ToString() << '\n';
	}
	else
	{
		tOptional<T> Val = T::Parse(arg1, arg2);
		std::cout << Val.ToString() << '\n';
	}
}*/

void UnitTest_PacketNMEAType()
{
	std::cout << "\n""utils::packet::nmea::type\n";

	UnitTest_PacketNMEAType_Test<tGNSS>("tGNSS empty", "--");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GLONASS, "tGNSS GL", "GL");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GPS, "tGNSS GP", "GP");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GPS, "tGNSS GP", "GP");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GPS_GLONASS, "tGNSS GN", "GN");
	UnitTest_PacketNMEAType_Test<tGNSS>("", "tGNSS parse nothing", "--");
	UnitTest_PacketNMEAType_Test<tGNSS>("GL", "tGNSS parse GL", "GL");
	UnitTest_PacketNMEAType_Test<tGNSS>("GP", "tGNSS parse GP", "GP");
	UnitTest_PacketNMEAType_Test<tGNSS>("GN", "tGNSS parse GN", "GN");
	UnitTest_PacketNMEAType_Test<tGNSS>("GR", "tGNSS parse GR", "--");

	UnitTest_PacketNMEAType_Test<tValid>("tValid empty", "");
	UnitTest_PacketNMEAType_Test<tValid>(true, "tValid true", "A");
	UnitTest_PacketNMEAType_Test<tValid>(false, "tValid false", "V");
	UnitTest_PacketNMEAType_Test<tValid>("", "tValid parse empty", "");
	UnitTest_PacketNMEAType_Test<tValid>("A", "tValid parse A", "A");
	UnitTest_PacketNMEAType_Test<tValid>("V", "tValid parse V", "V");
	UnitTest_PacketNMEAType_Test<tValid>("R", "tValid parse R", "");

	UnitTest_PacketNMEAType_Test<tDate>("tDate empty", "");
	UnitTest_PacketNMEAType_Test<tDate>(1, 2, 3, "tDate 1,2,3", "030201");
	UnitTest_PacketNMEAType_Test<tDate>(99, 12, 31, "tDate 99,12,31", "311299");
	UnitTest_PacketNMEAType_Test<tDate>(100, 13, 33, "tDate 100,12,31 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("", "tDate parse empty", "");
	UnitTest_PacketNMEAType_Test<tDate>("010203", "tDate parse 010203", "010203");
	UnitTest_PacketNMEAType_Test<tDate>("311299", "tDate parse 311299", "311299");
	UnitTest_PacketNMEAType_Test<tDate>("501333", "tDate parse 501333 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("01020", "tDate parse 01020 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("0102035", "tDate parse 0102035 (wrong)", "");

	typedef tGeoDegree<2, 2, 'N', 'S', 90> tGeoDegreeLatitude2;
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("tGeoDegreeLatitude2 empty", ",");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(0, "tGeoDegreeLatitude2 0", "0000.00,N");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("1234.56", "S", "tGeoDegreeLatitude2 1234.56,S", "1234.56,S");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(31.45, "tGeoDegreeLatitude2 31.45", "3127.00,N");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(-31.45, "tGeoDegreeLatitude2 -31.45", "3127.00,S");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(34, "tGeoDegreeLatitude2 34", "3400.00,N");
	UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("1234.56", "K", "tGeoDegreeLatitude2 1234.56,K", ",");

	UnitTest_PacketNMEAType_Test<tLatitude2>("tLatitude2 empty", ",");
	UnitTest_PacketNMEAType_Test<tLatitude2>(0, "tLatitude2 0", "0000.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude2>("1234.56", "S", "tLatitude2 1234.56,S", "1234.56,S");
	UnitTest_PacketNMEAType_Test<tLatitude2>(31.45, "tLatitude2 31.45", "3127.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude2>(34, "tLatitude2 34", "3400.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude2>("1234.56", "K", "tLatitude2 1234.56,K", ",");

	UnitTest_PacketNMEAType_Test<tLatitude4>("tLatitude4 empty", ",");
	UnitTest_PacketNMEAType_Test<tLatitude4>(0, "tLatitude4 0", "0000.0000,N");
	UnitTest_PacketNMEAType_Test<tLatitude4>("1234.5678", "S", "tLatitude4 1234.5678,S", "1234.5678,S");
	UnitTest_PacketNMEAType_Test<tLatitude4>(31.45678, "tLatitude4 31.45678", "3127.4068,N");

	UnitTest_PacketNMEAType_Test<tLatitude6>("tLatitude6 empty", ",");
	UnitTest_PacketNMEAType_Test<tLatitude6>(0, "tLatitude6 0", "0000.000000,N");
	UnitTest_PacketNMEAType_Test<tLatitude6>("1234.567891", "N", "tLatitude6 1234.567891,N", "1234.567891,N");
	UnitTest_PacketNMEAType_Test<tLatitude6>(31.4567834, "tLatitude6 31.4567834", "3127.407004,N");

	UnitTest_PacketNMEAType_Test<tLongitude2>("tLongitude2 empty", ",");
	UnitTest_PacketNMEAType_Test<tLongitude2>(0, "tLongitude2 0", "00000.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude2>("01234.56", "W", "tLongitude2 0", "01234.56,W");
	UnitTest_PacketNMEAType_Test<tLongitude2>(31.45, "tLongitude2 0", "03127.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude2>(31, "tLongitude2 0", "03100.00,E");

	UnitTest_PacketNMEAType_Test<tLongitude4>("tLongitude4 empty", ",");
	UnitTest_PacketNMEAType_Test<tLongitude4>(0, "tLongitude4 00000.0000,E", "00000.0000,E");
	UnitTest_PacketNMEAType_Test<tLongitude4>("12345.6789", "W", "tLongitude4 12345.6789,W", "12345.6789,W");
	UnitTest_PacketNMEAType_Test<tLongitude4>(31.45678, "tLongitude4 31.45678", "03127.4068,E");

	UnitTest_PacketNMEAType_Test<tLongitude6>("tLongitude6 empty", ",");
	UnitTest_PacketNMEAType_Test<tLongitude6>(0, "tLongitude6 00000.000000,E", "00000.000000,E");
	UnitTest_PacketNMEAType_Test<tLongitude6>("12345.678901", "E", "tLongitude6 12345.678901,E", "12345.678901,E");
	UnitTest_PacketNMEAType_Test<tLongitude6>(123.4567834, "tLongitude6 231.4567834", "12327.407004,E");

	/*UnitTest_PacketNMEAType_Test<tFloat<2, 4>>();
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>(23);
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("34.5688");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>(52.3434);
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>(2.34);

	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>();
	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>(0);
	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>("4.56");
	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>(2.34);
	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>("1234.56");
	UnitTest_PacketNMEAType_Test<tFloat<0, 2>>(3.3418);

	UnitTest_PacketNMEAType_Test<tFloat<0, 4>>();
	UnitTest_PacketNMEAType_Test<tFloat<0, 4>>(0);
	UnitTest_PacketNMEAType_Test<tFloat<0, 4>>("4.5736");
	UnitTest_PacketNMEAType_Test<tFloat<0, 4>>(2.34);

	//UnitTest_PacketNMEAType_Test<tFloat<0, 0>>();//fract is just of max length (6 chars)
	//UnitTest_PacketNMEAType_Test<tFloat<0, 0>>(0);
	//UnitTest_PacketNMEAType_Test<tFloat<0, 0>>("4.5736");
	//UnitTest_PacketNMEAType_Test<tFloat<0, 0>>(72.34);

	//UnitTest_PacketNMEAType_Test<tFloat<2, 0>>();//doesn't exist
	//UnitTest_PacketNMEAType_Test<tFloat<2, 0>>(0);//doesn't exist
	//UnitTest_PacketNMEAType_Test<tFloat<2, 0>>("4.5736");//doesn't exist
	//UnitTest_PacketNMEAType_Test<tFloat<2, 0>>(2.34);//doesn't exist

	UnitTest_PacketNMEAType_Test<tUInt<int, 4>>();
	UnitTest_PacketNMEAType_Test<tUInt<int, 4>>(0);
	UnitTest_PacketNMEAType_Test<tUInt<int, 4>>("4356");
	UnitTest_PacketNMEAType_Test<tUInt<int, 4>>(2734);

	UnitTest_PacketNMEAType_Test<tUInt<int, 0>>();
	UnitTest_PacketNMEAType_Test<tUInt<int, 0>>(0);
	UnitTest_PacketNMEAType_Test<tUInt<int, 0>>("43586");
	UnitTest_PacketNMEAType_Test<tUInt<int, 0>>(27384);

	UnitTest_PacketNMEAType_Test<tSatellite>();
	UnitTest_PacketNMEAType_Test<tSatellite>(12, 34, 56, 78);
	UnitTest_PacketNMEAType_Test<tSatellite>("23", "38", "230", "44");

	UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>();
	UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>(0, 'G');
	UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>("54.5736", "R");
	UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>(27.384, 'S');*/

	std::cout << std::endl;
}

}
