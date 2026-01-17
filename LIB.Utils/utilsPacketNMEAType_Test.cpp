#include "utilsPacketNMEAType.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

using namespace utils::packet::nmea::type;

#define SHOW_RESULTS

template <class T>
void UnitTest_PacketNMEAType_Test(const std::string& msg, const std::string& res)
{
	T A;
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
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
		std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT(msg, A.ToString() == res);
	}
	else
	{
		T A(arg);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
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

template <class T, class TArg1, class TArg2>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.ToString() == res);
}

template <class T, class TArg1, class TArg2, class TArg3>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2, TArg3 arg3, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2, arg3);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
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
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GlobalNavigation, "tGNSS GN", "GN");
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

	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(       "tUIntFixed<1> empty", "");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>("0",   "tUIntFixed<1> 0",     "0");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>("4",   "tUIntFixed<1> 4",     "4");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(0,     "tUIntFixed<1> 0",     "0");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(4,     "tUIntFixed<1> 4",     "4");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(12283, "tUIntFixed<1> 12283", ""); // ERR

	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>(      "tUIntFixed<2> empty", "");
	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>("00", "tUIntFixed<2> 00",    "00");
	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>("45", "tUIntFixed<2> 45",    "45");
	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>(0,    "tUIntFixed<2> 0",     "00");
	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>(45,   "tUIntFixed<2> 45",    "45");
	UnitTest_PacketNMEAType_Test<tUIntFixed<2>>(453,  "tUIntFixed<2> 453",   ""); // ERR

	UnitTest_PacketNMEAType_Test<tUIntFixed<3>>(       "tUIntFixed<3> empty", "");
	UnitTest_PacketNMEAType_Test<tUIntFixed<3>>("000", "tUIntFixed<3> 000",   "000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<3>>("456", "tUIntFixed<3> 456",   "456");
	UnitTest_PacketNMEAType_Test<tUIntFixed<3>>(0,     "tUIntFixed<3> 0",     "000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<3>>(456,   "tUIntFixed<3> 456",   "456");
	
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(        "tUIntFixed<4> empty", "");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>("0000", "tUIntFixed<4> 0000", "0000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>("4567", "tUIntFixed<4> 4567", "4567");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(0,      "tUIntFixed<4> 0",    "0000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(4567,   "tUIntFixed<4> 4567", "4567");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(45678,  "tUIntFixed<4> 45678", ""); // ERR

	UnitTest_PacketNMEAType_Test<tIntFixed<1>>(       "tIntFixed<1> empty", "");
	UnitTest_PacketNMEAType_Test<tIntFixed<1>>("0",   "tIntFixed<1> 0", "0");
	UnitTest_PacketNMEAType_Test<tIntFixed<1>>("4",   "tIntFixed<1> 4", "4");
	UnitTest_PacketNMEAType_Test<tIntFixed<1>>(0,     "tIntFixed<1> 0", "0");
	UnitTest_PacketNMEAType_Test<tIntFixed<1>>(4,     "tIntFixed<1> 4", "4");
	UnitTest_PacketNMEAType_Test<tIntFixed<1>>(12283, "tIntFixed<1> 12283", ""); // ERR

	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(       "tIntFixed<2> empty", "");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("00",  "tIntFixed<2> 00",    "00");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("-00", "tIntFixed<2> -00",   "00"); // !
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("-45", "tIntFixed<2> -45",   "-45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("45",  "tIntFixed<2> 45",    "45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(0,     "tIntFixed<2> 0",     "00");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(-45,   "tIntFixed<2> -45",   "-45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(45,    "tIntFixed<2> 45",    "45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(-453,  "tIntFixed<2> -453",  ""); // ERR
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(-4,    "tIntFixed<2> -4",    "-04");

	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>(              "tUFloatFixed<2,4>",          "");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>("00.0000",    "tUFloatFixed<2,4> 00.0000",  "00.0000");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>("12.3456",    "tUFloatFixed<2,4> 12.3456",  "12.3456");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>("127.3456",   "tUFloatFixed<2,4> 127.3456", ""); // ERR
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>("7.3456",     "tUFloatFixed<2,4> 7.3456",   ""); // ERR
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>(0,            "tUFloatFixed<2,4> 0",        "00.0000");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>(12.3456,      "tUFloatFixed<2,4> 12.3456",  "12.3456");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2,4>>(127.3456,     "tUFloatFixed<2,4> 127.3456", ""); // ERR

	UnitTest_PacketNMEAType_Test<tFloatFixed<2,4>>("-00.0000",  "tFloatFixed<2,4> -00.0000", "00.0000"); // !
	UnitTest_PacketNMEAType_Test<tFloatFixed<2,4>>("-12.3456",  "tFloatFixed<2,4> -12.3456", "-12.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2,4>>("-7.3456",   "tFloatFixed<2,4> -7.3456", ""); // ERR
	UnitTest_PacketNMEAType_Test<tFloatFixed<2,4>>(-12.3456,    "tFloatFixed<2,4> -12.3456", "-12.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2,4>>(-1.3456,     "tFloatFixed<2,4> -1.3456", "-01.3456");

	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>(            "tFloatFixed<5,1>",           "");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>("00000.0",  "tFloatFixed<5,1> 00000.0",   "00000.0");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>("12345.6",  "tFloatFixed<5,1> 12345.678", "12345.6");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>("00001.6",  "tFloatFixed<5,1> 00001.6",   "00001.6");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>(0,          "tFloatFixed<5,1> 0",         "00000.0");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>(12345.6111, "tFloatFixed<5,1> 12345.6",   "12345.6");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>(4.6111,     "tFloatFixed<5,1> 4.6111",    "00004.6");
	UnitTest_PacketNMEAType_Test<tFloatFixed<5,1>>(-4.6111,    "tFloatFixed<5,1> -4.6111",   "-00004.6");

	UnitTest_PacketNMEAType_Test<tDate>("tDate empty", "");
	UnitTest_PacketNMEAType_Test<tDate>(3, 2, 1, "tDate 3,2,1", "010203");
	UnitTest_PacketNMEAType_Test<tDate>(99, 12, 31, "tDate 99,12,31", "311299");
	UnitTest_PacketNMEAType_Test<tDate>(100, 13, 33, "tDate 100,12,31 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("", "tDate parse empty", "");
	UnitTest_PacketNMEAType_Test<tDate>("010203", "tDate parse 010203", "010203");
	UnitTest_PacketNMEAType_Test<tDate>("311299", "tDate parse 311299", "311299");
	UnitTest_PacketNMEAType_Test<tDate>("501333", "tDate parse 501333 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("01020", "tDate parse 01020 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>("0102035", "tDate parse 0102035 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tDate>(1044046800, "tDate 3,2,1", "010203");
	UnitTest_PacketNMEAType_Test<tDate>(946587600, "tDate 99,12,31", "311299");
	
	UnitTest_PacketNMEAType_Test<tTime<0>>(                 "tTime<0> empty",                   "");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3.4567890, "tTime<0> 1, 2, 3.4567890",         "010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3.45,      "tTime<0> 1, 2, 3.45",              "010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3,         "tTime<0> 1, 2, 3",                 "010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(0, 0, 0,         "tTime<0> 0, 0, 0",                 "000000");
	UnitTest_PacketNMEAType_Test<tTime<0>>(32, 64, 78.9999, "tTime<0> 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime<0>>("000000",        "tTime<0> 000000",                  "000000");
	UnitTest_PacketNMEAType_Test<tTime<0>>("010203",        "tTime<0> 010203",                  "010203");

	UnitTest_PacketNMEAType_Test<tTime<2>>(                 "tTime<2> empty",                   "");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3.4567890, "tTime<2> 1, 2, 3.4567890",         "010203.46");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3.45,      "tTime<2> 1, 2, 3.45",              "010203.45");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3,         "tTime<2> 1, 2, 3",                 "010203.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>(0, 0, 0,         "tTime<2> 0, 0, 0",                 "000000.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>(32, 64, 78.9999, "tTime<2> 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime<2>>("000000.00",     "tTime<2> 000000.00",               "000000.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>("010203.45",     "tTime<2> 010203.45",               "010203.45");

	UnitTest_PacketNMEAType_Test<tTime<3>>(                 "tTime<3> empty",                   "");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3.4567890, "tTime<3> 1, 2, 3.4567890",         "010203.457");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3.45,      "tTime<3> 1, 2, 3.45",              "010203.450");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3,         "tTime<3> 1, 2, 3",                 "010203.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>(0, 0, 0,         "tTime<3> 0, 0, 0",                 "000000.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>(32, 64, 78.9999, "tTime<3> 32, 64, 78.9999 (wrong)", "");
	UnitTest_PacketNMEAType_Test<tTime<3>>("000000.000",    "tTime<3> 000000.000",              "000000.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>("010203.456",    "tTime<3> 010203.456",              "010203.456");

	UnitTest_PacketNMEAType_Test<tLatitude<2>>(                "tLatitude<2> empty",     ",");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(0,              "tLatitude<2> 0",         "0000.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>("1234.56", "S", "tLatitude<2> 1234.56,S", "1234.56,S");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(31.45,          "tLatitude<2> 31.45",     "3127.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(-31.45,         "tLatitude<2> -31.45",    "3127.00,S");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(34,             "tLatitude<2> 34",        "3400.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>("1234.56", "K", "tLatitude<2> 1234.56,K", ",");
	
	UnitTest_PacketNMEAType_Test<tLatitude<4>>(                  "tLatitude<4> empty",       ",");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>(0,                "tLatitude<4> 0",           "0000.0000,N");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>("1234.5678", "S", "tLatitude<4> 1234.5678,S", "1234.5678,S");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>(31.45678,         "tLatitude<4> 31.45678",    "3127.4068,N");
	
	UnitTest_PacketNMEAType_Test<tLatitude<6>>(                    "tLatitude<6> empty",         ",");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>(0,                  "tLatitude<6> 0",             "0000.000000,N");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>("1234.567891", "N", "tLatitude<6> 1234.567891,N", "1234.567891,N");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>(31.4567834,         "tLatitude<6> 31.4567834",    "3127.407004,N");

	UnitTest_PacketNMEAType_Test<tLongitude<2>>(                 "tLongitude<2> empty", ",");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(0,               "tLongitude<2> 0",     "00000.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>("01234.56", "W", "tLongitude<2> 0",     "01234.56,W");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(31.45,           "tLongitude<2> 0",     "03127.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(31,              "tLongitude<2> 0",     "03100.00,E");

	UnitTest_PacketNMEAType_Test<tLongitude<4>>(                   "tLongitude4 empty",        ",");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>(0,                 "tLongitude4 00000.0000,E", "00000.0000,E");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>("12345.6789", "W", "tLongitude4 12345.6789,W", "12345.6789,W");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>(31.45678,          "tLongitude4 31.45678",     "03127.4068,E");

	UnitTest_PacketNMEAType_Test<tLongitude<6>>(                     "tLongitude6 empty",          ",");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>(0,                   "tLongitude6 00000.000000,E", "00000.000000,E");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>("12345.678901", "E", "tLongitude6 12345.678901,E", "12345.678901,E");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>(123.4567834,         "tLongitude6 231.4567834",    "12327.407004,E");

	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>(             "tFloatPrecisionFixed<2> empty",     "");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>(0,           "tFloatPrecisionFixed<2> 0.00",      "0.00");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>(-10.3456,    "tFloatPrecisionFixed<2> -10.35",    "-10.35");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>("12345.67",  "tFloatPrecisionFixed<2> 12345.67",  "12345.67");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>("-12345.67", "tFloatPrecisionFixed<2> -12345.67", "-12345.67");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<2>>(123.4567834, "tFloatPrecisionFixed<2> 123.46",    "123.46");

	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>(             "tFloatPrecisionFixed<2> empty",     "");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>(0,           "tFloatPrecisionFixed<2> 0.00",      "0.00");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>(-10.3456,    "tFloatPrecisionFixed<2> 0.00",      "");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>("12345.67",  "tFloatPrecisionFixed<2> 12345.67",  "12345.67");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>("-12345.67", "tFloatPrecisionFixed<2> 12345.67",  "");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<2>>(123.4567834, "tFloatPrecisionFixed<2> 231.46",    "123.46");

	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<4>>("12.3456", "R", "tFloatPrecisionFixedUnit<4> 12.3456,R",  "12.3456,R");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<4>>(12.3456, 'W',   "tFloatPrecisionFixedUnit<4> 12.3456,W",  "12.3456,W");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<4>>(-12.3456, 'W',  "tFloatPrecisionFixedUnit<4> -12.3456,W", "-12.3456,W");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<4>>(1.3, 'W',       "tFloatPrecisionFixedUnit<4> 1.3000,W",   "1.3000,W");

	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<4>>("12.3456", "R", "tFloatPrecisionFixedUnit<4> 12.3456,R", "12.3456,R");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<4>>(12.3456, 'W',   "tFloatPrecisionFixedUnit<4> 12.3456,W", "12.3456,W");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<4>>(-12.3456, 'W',  "tFloatPrecisionFixedUnit<4> -12.3456,W", ",W");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<4>>(-12.3456, 0,    "tFloatPrecisionFixedUnit<4> -12.3456,W", ",");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<4>>(1.3, 'W',       "tFloatPrecisionFixedUnit<4> 1.3000,W",   "1.3000,W");
	/*
	//UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>();
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(0, 'G');
	//
	UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("12.3456", "R", "tFloatUnit2x4 12.3456,R", "12.3456,R");
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("312.3456", "R", "tFloatUnit2x4 12.3456,R", "312.3456,R"); // Parse ERR
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("2.3456", "R", "tFloatUnit2x4 12.3456,R", "2.3456,R"); // Parse ERR
	//
	UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(12.3456, 'W', "tFloatUnit2x4 12.3456,W", "12.3456,W");
	UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(1.3,     'W', "tFloatUnit2x4 01.3000,W", "01.3000,W");
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 0>>(1.3, 'W', "tFloatUnit0x4 1.3000,W", "1.3000,W"); // ?
	//
	//UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>(27.384, 'S');
	*/

/*
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

	//UnitTest_PacketNMEAType_Test<tDate>("tDate empty", "");
	//UnitTest_PacketNMEAType_Test<tDate>(1, 2, 3, "tDate 1,2,3", "030201");
	//UnitTest_PacketNMEAType_Test<tDate>(99, 12, 31, "tDate 99,12,31", "311299");
	//UnitTest_PacketNMEAType_Test<tDate>(100, 13, 33, "tDate 100,12,31 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tDate>("", "tDate parse empty", "");
	//UnitTest_PacketNMEAType_Test<tDate>("010203", "tDate parse 010203", "010203");
	//UnitTest_PacketNMEAType_Test<tDate>("311299", "tDate parse 311299", "311299");
	//UnitTest_PacketNMEAType_Test<tDate>("501333", "tDate parse 501333 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tDate>("01020", "tDate parse 01020 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tDate>("0102035", "tDate parse 0102035 (wrong)", "");

	//UnitTest_PacketNMEAType_Test<tTime0>("tTime0 empty", "");
	//UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3.4567890, "tTime0 1, 2, 3.4567890", "010203");
	//UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3.45, "tTime0 1, 2, 3.45", "010203");
	//UnitTest_PacketNMEAType_Test<tTime0>(1, 2, 3, "tTime0 1, 2, 3", "010203");
	//UnitTest_PacketNMEAType_Test<tTime0>(0, 0, 0, "tTime0 0, 0, 0", "000000");
	//UnitTest_PacketNMEAType_Test<tTime0>(32, 64, 78.9999, "tTime0 32, 64, 78.9999 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tTime0>("000000", "tTime0 000000", "000000");
	//UnitTest_PacketNMEAType_Test<tTime0>("010203", "tTime0 010203", "010203");

	//UnitTest_PacketNMEAType_Test<tTime2>("tTime2 empty", "");
	//UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3.4567890, "tTime2 1, 2, 3.4567890", "010203.45");
	//UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3.45, "tTime2 1, 2, 3.45", "010203.45");
	//UnitTest_PacketNMEAType_Test<tTime2>(1, 2, 3, "tTime2 1, 2, 3", "010203.00");
	//UnitTest_PacketNMEAType_Test<tTime2>(0, 0, 0, "tTime2 0, 0, 0", "000000.00");
	//UnitTest_PacketNMEAType_Test<tTime2>(32, 64, 78.9999, "tTime2 32, 64, 78.9999 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tTime2>("000000.00", "tTime2 000000.00", "000000.00");
	//UnitTest_PacketNMEAType_Test<tTime2>("010203.45", "tTime2 010203.45", "010203.45");

	//UnitTest_PacketNMEAType_Test<tTime3>("tTime3 empty", "");
	//UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3.4567890, "tTime3 1, 2, 3.4567890", "010203.456");
	//UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3.45, "tTime3 1, 2, 3.45", "010203.450");
	//UnitTest_PacketNMEAType_Test<tTime3>(1, 2, 3, "tTime3 1, 2, 3", "010203.000");
	//UnitTest_PacketNMEAType_Test<tTime3>(0, 0, 0, "tTime3 0, 0, 0", "000000.000");
	//UnitTest_PacketNMEAType_Test<tTime3>(32, 64, 78.9999, "tTime3 32, 64, 78.9999 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tTime3>("000000.000", "tTime3 000000.000", "000000.000");
	//UnitTest_PacketNMEAType_Test<tTime3>("010203.456", "tTime3 010203.456", "010203.456");

	//typedef tGeoDegree<2, 2, 'N', 'S', 90> tGeoDegreeLatitude2;
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("tGeoDegreeLatitude2 empty", ",");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(0, "tGeoDegreeLatitude2 0", "0000.00,N");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("1234.56", "S", "tGeoDegreeLatitude2 1234.56,S", "1234.56,S");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(31.45, "tGeoDegreeLatitude2 31.45", "3127.00,N");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(-31.45, "tGeoDegreeLatitude2 -31.45", "3127.00,S");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>(34, "tGeoDegreeLatitude2 34", "3400.00,N");
	//UnitTest_PacketNMEAType_Test<tGeoDegreeLatitude2>("1234.56", "K", "tGeoDegreeLatitude2 1234.56,K", ",");

	//UnitTest_PacketNMEAType_Test<tLatitude2>("tLatitude2 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLatitude2>(0, "tLatitude2 0", "0000.00,N");
	//UnitTest_PacketNMEAType_Test<tLatitude2>("1234.56", "S", "tLatitude2 1234.56,S", "1234.56,S");
	//UnitTest_PacketNMEAType_Test<tLatitude2>(31.45, "tLatitude2 31.45", "3127.00,N");
	//UnitTest_PacketNMEAType_Test<tLatitude2>(34, "tLatitude2 34", "3400.00,N");
	//UnitTest_PacketNMEAType_Test<tLatitude2>("1234.56", "K", "tLatitude2 1234.56,K", ",");

	//UnitTest_PacketNMEAType_Test<tLatitude4>("tLatitude4 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLatitude4>(0, "tLatitude4 0", "0000.0000,N");
	//UnitTest_PacketNMEAType_Test<tLatitude4>("1234.5678", "S", "tLatitude4 1234.5678,S", "1234.5678,S");
	//UnitTest_PacketNMEAType_Test<tLatitude4>(31.45678, "tLatitude4 31.45678", "3127.4068,N");

	//UnitTest_PacketNMEAType_Test<tLatitude6>("tLatitude6 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLatitude6>(0, "tLatitude6 0", "0000.000000,N");
	//UnitTest_PacketNMEAType_Test<tLatitude6>("1234.567891", "N", "tLatitude6 1234.567891,N", "1234.567891,N");
	//UnitTest_PacketNMEAType_Test<tLatitude6>(31.4567834, "tLatitude6 31.4567834", "3127.407004,N");

	//UnitTest_PacketNMEAType_Test<tLongitude2>("tLongitude2 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLongitude2>(0, "tLongitude2 0", "00000.00,E");
	//UnitTest_PacketNMEAType_Test<tLongitude2>("01234.56", "W", "tLongitude2 0", "01234.56,W");
	//UnitTest_PacketNMEAType_Test<tLongitude2>(31.45, "tLongitude2 0", "03127.00,E");
	//UnitTest_PacketNMEAType_Test<tLongitude2>(31, "tLongitude2 0", "03100.00,E");

	//UnitTest_PacketNMEAType_Test<tLongitude4>("tLongitude4 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLongitude4>(0, "tLongitude4 00000.0000,E", "00000.0000,E");
	//UnitTest_PacketNMEAType_Test<tLongitude4>("12345.6789", "W", "tLongitude4 12345.6789,W", "12345.6789,W");
	//UnitTest_PacketNMEAType_Test<tLongitude4>(31.45678, "tLongitude4 31.45678", "03127.4068,E");

	//UnitTest_PacketNMEAType_Test<tLongitude6>("tLongitude6 empty", ",");
	//UnitTest_PacketNMEAType_Test<tLongitude6>(0, "tLongitude6 00000.000000,E", "00000.000000,E");
	//UnitTest_PacketNMEAType_Test<tLongitude6>("12345.678901", "E", "tLongitude6 12345.678901,E", "12345.678901,E");
	//UnitTest_PacketNMEAType_Test<tLongitude6>(123.4567834, "tLongitude6 231.4567834", "12327.407004,E");

	//UnitTest_PacketNMEAType_Test<tUIntFixed1>(       "tUIntFixed1 empty", "");
	//UnitTest_PacketNMEAType_Test<tUIntFixed1>("0",   "tUIntFixed1 0", "0");
	//UnitTest_PacketNMEAType_Test<tUIntFixed1>("4",   "tUIntFixed1 4", "4");
	//UnitTest_PacketNMEAType_Test<tUIntFixed1>(0,     "tUIntFixed1 0", "0");
	//UnitTest_PacketNMEAType_Test<tUIntFixed1>(4,     "tUIntFixed1 4", "4");
	//UnitTest_PacketNMEAType_Test<tUIntFixed1>(12283, "tUIntFixed1 12283", ""); // ERR

	//UnitTest_PacketNMEAType_Test<tUIntFixed2>(       "tUIntFixed2 empty", "");
	//UnitTest_PacketNMEAType_Test<tUIntFixed2>("00",  "tUIntFixed2 00", "00");
	//UnitTest_PacketNMEAType_Test<tUIntFixed2>("45",  "tUIntFixed2 45", "45");
	//UnitTest_PacketNMEAType_Test<tUIntFixed2>(0,     "tUIntFixed2 0", "00");
	//UnitTest_PacketNMEAType_Test<tUIntFixed2>(45,    "tUIntFixed2 45", "45");

	//UnitTest_PacketNMEAType_Test<tUIntFixed3>(       "tUIntFixed3 empty", "");
	//UnitTest_PacketNMEAType_Test<tUIntFixed3>("000", "tUIntFixed3 000", "000");
	//UnitTest_PacketNMEAType_Test<tUIntFixed3>("456", "tUIntFixed3 456", "456");
	//UnitTest_PacketNMEAType_Test<tUIntFixed3>(0,     "tUIntFixed3 0", "000");
	//UnitTest_PacketNMEAType_Test<tUIntFixed3>(456,   "tUIntFixed3 456", "456");
	//
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>(        "tUIntFixed4 empty", "");
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>("0000", "tUIntFixed4 0000", "0000");
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>("4567", "tUIntFixed4 4567", "4567");
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>(0,      "tUIntFixed4 0", "0000");
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>(4567,   "tUIntFixed4 4567", "4567");
	//UnitTest_PacketNMEAType_Test<tUIntFixed4>(45678,  "tUIntFixed4 45678", ""); // ERR

	UnitTest_PacketNMEAType_Test<tUInt1>(      "tUInt1 empty", "");
	UnitTest_PacketNMEAType_Test<tUInt1>("0",  "tUInt1 0", "0");
	UnitTest_PacketNMEAType_Test<tUInt1>("5",  "tUInt1 5", "5");
	UnitTest_PacketNMEAType_Test<tUInt1>("10", "tUInt1 10", ""); // ERR
	UnitTest_PacketNMEAType_Test<tUInt1>(0,    "tUInt1 0", "0");
	UnitTest_PacketNMEAType_Test<tUInt1>(4567, "tUInt1 4567", ""); // ERR

	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(           "tFloatFixed2", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(0,         "tFloatFixed2 0", "0.00");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("4.56",    "tFloatFixed2 4.56", "4.56");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(2.34,      "tFloatFixed2 2.34", "2.34");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("1234.56", "tFloatFixed2 1234.56", "1234.56");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("34.5678", "tFloatFixed2 34.5678 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(3.3418,    "tFloatFixed2 3.3418", "3.34");

	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("-4.56",    "tFloatFixed2 -4.56", "-4.56");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(-2.34,      "tFloatFixed2 -2.34", "-2.34");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("-1234.56", "tFloatFixed2 -1234.56", "-1234.56");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>("-34.5678", "tFloatFixed2 -34.5678 (wrong)", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2>(-3.3418,    "tFloatFixed2 -3.3418", "-3.34");

	//UnitTest_PacketNMEAType_Test<tFloatFixed4>(          "tFloat0x4", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed4>(0,        "tFloat0x4 0", "0.0000");
	//UnitTest_PacketNMEAType_Test<tFloatFixed4>("4.5736", "tFloat0x4 4.5736", "4.5736");
	//UnitTest_PacketNMEAType_Test<tFloatFixed4>(2.34,     "tFloat0x4 2.34", "2.3400");
	//UnitTest_PacketNMEAType_Test<tFloatFixed4>(0.34,     "tFloat0x4 0.34", "0.3400");

	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(              "tFloatFixed2x4", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("00.0000",    "tFloatFixed2x4 00.0000", "00.0000");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("12.3456",    "tFloatFixed2x4 12.3456", "12.3456");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("127.3456",   "tFloatFixed2x4 127.3456", ""); // ERR
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("7.3456",     "tFloatFixed2x4 7.3456", ""); // ERR
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("127.345",    "tFloatFixed2x4 127.345", ""); // ERR
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(0,            "tFloatFixed2x4 0", "00.0000");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(12.3456,      "tFloatFixed2x4 12.3456", "12.3456");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(127.3456,     "tFloatFixed2x4 127.3456", ""); // ERR
	//
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("-00.0000",  "tFloatFixed2x4 -00.0000", "00.0000");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("-12.3456",  "tFloatFixed2x4 -12.3456", "-12.3456");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>("-7.3456",   "tFloatFixed2x4 -7.3456", ""); // ERR
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(-12.3456,    "tFloatFixed2x4 -12.3456", "-12.3456");
	//UnitTest_PacketNMEAType_Test<tFloatFixed2x4>(-1.3456,     "tFloatFixed2x4 -1.3456", "-1.3456");

	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>(            "tFloatFixed5x1", "");
	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>("00000.0",  "tFloatFixed5x1 00000.0", "00000.0");
	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>("12345.6",  "tFloatFixed5x1 12345.678", "12345.6");
	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>("00001.6",  "tFloatFixed5x1 00001.6", "00001.6");
	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>(0,          "tFloatFixed5x1 0", "00000.0");
	//UnitTest_PacketNMEAType_Test<tFloatFixed5x1>(12345.6111, "tFloatFixed5x1 12345.6", "12345.6");
*/
	/*UnitTest_PacketNMEAType_Test<tSatellite>();
	UnitTest_PacketNMEAType_Test<tSatellite>(12, 34, 56, 78);
	UnitTest_PacketNMEAType_Test<tSatellite>("23", "38", "230", "44");*/
/*
	////UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>();
	////UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(0, 'G');
	//// 
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("12.3456", "R", "tFloatUnit2x4 12.3456,R", "12.3456,R");
	////UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("312.3456", "R", "tFloatUnit2x4 12.3456,R", "312.3456,R"); // Parse ERR
	////UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>("2.3456", "R", "tFloatUnit2x4 12.3456,R", "2.3456,R"); // Parse ERR
	//// 
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(12.3456, 'W', "tFloatUnit2x4 12.3456,W", "12.3456,W");
	//UnitTest_PacketNMEAType_Test<tFloatUnit<4, 2>>(1.3,     'W', "tFloatUnit2x4 01.3000,W", "01.3000,W");
	////UnitTest_PacketNMEAType_Test<tFloatUnit<4, 0>>(1.3, 'W', "tFloatUnit0x4 1.3000,W", "1.3000,W"); // ?
	//// 
	////UnitTest_PacketNMEAType_Test<tFloatUnit<2, 4>>(27.384, 'S');
*/
	std::cout << std::endl;
}

}
