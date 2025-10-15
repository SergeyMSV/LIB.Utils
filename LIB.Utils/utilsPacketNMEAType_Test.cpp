#include "utilsPacketNMEAType.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

using namespace utils::packet::nmea::type;

//#define SHOW_RESULTS

template <class T>
void UnitTest_PacketNMEAType_Test(const std::string& msg, const std::string& res)
{
	T A;
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.ToString() == res);
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg, const std::string& msg,  const std::string& res)
{
	if constexpr (std::is_same_v<TArg, const char*>)
	{
		std::string ArgStr(arg);
		T A(ArgStr);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT(msg, A.ToString() == res);
	}
	else
	{
		T A(arg);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT(msg, A.ToString() == res);
	}
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg1, TArg arg2, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.ToString() == res);
}

template <class T, class TArg1, class TArg2, class TArg3>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2, TArg3 arg3, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2, arg3);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.ToString() == res);
}

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

	using tNumberFixed_6_2_3 = hidden::tNumberFixed<6, 2, 3>;
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("12345.678901", "tNumberFixed<6, 2, 3> 12345.678901", "12345.678901");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("02345.078901", "tNumberFixed<6, 2, 3> 02345.078901", "02345.078901");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("00000.000000", "tNumberFixed<6, 2, 3> 00000.000000", "00000.000000");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("12345", "tNumberFixed<6, 2, 3> 12345 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("12345.", "tNumberFixed<6, 2, 3> 12345. (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("12345.678", "tNumberFixed<6, 2, 3> 12345.678 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("312345.678901", "tNumberFixed<6, 2, 3> 312345.678901 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("345.678901", "tNumberFixed<6, 2, 3> 345.67890 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>(".678901", "tNumberFixed<6, 2, 3> .678901 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("12.345678901", "tNumberFixed<6, 2, 3> 12.345678901 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>("123456789.01", "tNumberFixed<6, 2, 3> 123456789.01 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tNumberFixed_6_2_3>(".12345678901", "tNumberFixed<6, 2, 3> .12345678901 (wrong)", "");

	using tNumberFixed_0_2_3 = hidden::tNumberFixed<0, 2, 3>;
	UnitTest_PacketNMEAType_Test<tNumberFixed_0_2_3>("12345", "tNumberFixed<0, 2, 3> 12345", "12345");
	UnitTest_PacketNMEAType_Test<tNumberFixed_0_2_3>("00000", "tNumberFixed<0, 2, 3> 12345", "00000");
	UnitTest_PacketNMEAType_Test<tNumberFixed_0_2_3>("1234", "tNumberFixed<0, 2, 3> 1234 (wrong)", "");

	using tNumberFixed_0_2_2_2 = hidden::tNumberFixed<0, 2, 2, 2>; // tDate
	UnitTest_PacketNMEAType_Test<tNumberFixed_0_2_2_2>("311299", "tNumberFixed<0, 2, 2, 2> 311299", "311299");
	UnitTest_PacketNMEAType_Test<tNumberFixed_0_2_2_2>("030201", "tNumberFixed<0, 2, 2, 2> 030201", "030201");

	//utils::packet::nmea::type::

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

	UnitTest_PacketNMEAType_Test<tTime0>("tTime0 empty", "");
	UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3.4567890, "tTime0 1, 2, 3.4567890", "010203");
	UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3.45, "tTime0 1, 2, 3.45", "010203");
	UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3, "tTime0 1, 2, 3", "010203");
	UnitTest_PacketNMEAType_Test<tTime0>(0, 0, 0, "tTime0 0, 0, 0", "000000");
	UnitTest_PacketNMEAType_Test<tTime0>(32, 64, 78.9999, "tTime0 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime0>("000000", "tTime0 000000", "000000");
	UnitTest_PacketNMEAType_Test<tTime0>("010203", "tTime0 010203", "010203");

	UnitTest_PacketNMEAType_Test<tTime2>("tTime2 empty", "");
	UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3.4567890, "tTime2 1, 2, 3.4567890", "010203.45");
	UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3.45, "tTime2 1, 2, 3.45", "010203.45");
	UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3, "tTime2 1, 2, 3", "010203.00");
	UnitTest_PacketNMEAType_Test<tTime2>(0, 0, 0, "tTime2 0, 0, 0", "000000.00");
	UnitTest_PacketNMEAType_Test<tTime2>(32, 64, 78.9999, "tTime2 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime2>("000000.00", "tTime2 000000.00", "000000.00");
	UnitTest_PacketNMEAType_Test<tTime2>("010203.45", "tTime2 010203.45", "010203.45");

	UnitTest_PacketNMEAType_Test<tTime3>("tTime3 empty", "");
	UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3.4567890, "tTime3 1, 2, 3.4567890", "010203.456");
	UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3.45, "tTime3 1, 2, 3.45", "010203.450");
	UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3, "tTime3 1, 2, 3", "010203.000");
	UnitTest_PacketNMEAType_Test<tTime3>(0, 0, 0, "tTime3 0, 0, 0", "000000.000");
	UnitTest_PacketNMEAType_Test<tTime3>(32, 64, 78.9999, "tTime3 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime3>("000000.000", "tTime3 000000.000", "000000.000");
	UnitTest_PacketNMEAType_Test<tTime3>("010203.456", "tTime3 010203.456", "010203.456");

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

	UnitTest_PacketNMEAType_Test<tUInt1>("tUInt1 empty", "");
	UnitTest_PacketNMEAType_Test<tUInt1>("0", "tUInt1 0", "0");
	UnitTest_PacketNMEAType_Test<tUInt1>("4", "tUInt1 4", "4");
	UnitTest_PacketNMEAType_Test<tUInt1>(0, "tUInt1 0", "0");
	UnitTest_PacketNMEAType_Test<tUInt1>(4, "tUInt1 4", "4");
	UnitTest_PacketNMEAType_Test<tUInt1>(12283, "tUInt1 12283", "12283"); // That is not a very good thing.

	UnitTest_PacketNMEAType_Test<tUInt2>("tUInt2 empty", "");
	UnitTest_PacketNMEAType_Test<tUInt2>("00", "tUInt2 00", "00");
	UnitTest_PacketNMEAType_Test<tUInt2>("45", "tUInt2 45", "45");
	UnitTest_PacketNMEAType_Test<tUInt2>(0, "tUInt2 0", "00");
	UnitTest_PacketNMEAType_Test<tUInt2>(45, "tUInt2 45", "45");

	UnitTest_PacketNMEAType_Test<tUInt3>("tUInt3 empty", "");
	UnitTest_PacketNMEAType_Test<tUInt3>("000", "tUInt3 000", "000");
	UnitTest_PacketNMEAType_Test<tUInt3>("456", "tUInt3 456", "456");
	UnitTest_PacketNMEAType_Test<tUInt3>(0, "tUInt3 0", "000");
	UnitTest_PacketNMEAType_Test<tUInt3>(456, "tUInt3 456", "456");
	
	UnitTest_PacketNMEAType_Test<tUInt4>("tUInt4 empty", "");
	UnitTest_PacketNMEAType_Test<tUInt4>("0000", "tUInt4 0000", "0000");
	UnitTest_PacketNMEAType_Test<tUInt4>("4567", "tUInt4 4567", "4567");
	UnitTest_PacketNMEAType_Test<tUInt4>(0, "tUInt4 0", "0000");
	UnitTest_PacketNMEAType_Test<tUInt4>(4567, "tUInt4 4567", "4567");

	UnitTest_PacketNMEAType_Test<tFloat2x4>("tFloat2x4", "");
	UnitTest_PacketNMEAType_Test<tFloat2x4>("00.0000", "tFloat2x4 00.0000", "00.0000");
	UnitTest_PacketNMEAType_Test<tFloat2x4>("12.3456", "tFloat2x4 12.3456", "12.3456");
	UnitTest_PacketNMEAType_Test<tFloat2x4>(0, "tFloat2x4 0", "00.0000");
	UnitTest_PacketNMEAType_Test<tFloat2x4>(12.3456, "tFloat2x4 12.3456", "12.3456");

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
