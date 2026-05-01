#include "utilsPacketNMEAType.h"

#include "utilsTest.h"

#include <iostream>
#include <iomanip>

namespace utils
{

//#define SHOW_RESULTS

template <class T>
void UnitTest_PacketNMEAType_Test(const std::string& msg, const std::string& res)
{
	T A;
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
}

template <class T>
void UnitTest_PacketNMEAType_TestERR(const std::string& msg)
{
	T A;
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + msg, !A.IsVerified());
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
		utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
	}
	else
	{
		T A(arg);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
	}
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_TestERR(TArg arg, const std::string& msg)
{
	if constexpr (std::is_same_v<TArg, const char*>)
	{
		std::string ArgStr(arg);
		T A(ArgStr);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT("ERR " + msg, !A.IsVerified());
	}
	else
	{
		T A(arg);
#ifdef SHOW_RESULTS
		std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
		utils::test::RESULT("ERR " + msg, !A.IsVerified());
	}
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_Test(TArg arg1, TArg arg2, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
}

template <class T, class TArg>
void UnitTest_PacketNMEAType_TestERR(TArg arg1, TArg arg2, const std::string& msg)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + msg, !A.IsVerified());
}

template <class T, class TArg1, class TArg2>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
}

template <class T, class TArg1, class TArg2>
void UnitTest_PacketNMEAType_TestERR(TArg1 arg1, TArg2 arg2, const std::string& msg)
{
	T A(arg1, arg2);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + msg, !A.IsVerified());
}

template <class T, class TArg1, class TArg2, class TArg3>
void UnitTest_PacketNMEAType_Test(TArg1 arg1, TArg2 arg2, TArg3 arg3, const std::string& msg, const std::string& res)
{
	T A(arg1, arg2, arg3);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT(msg, A.IsVerified() && A.ToString() == res);
}

template <class T, class TArg1, class TArg2, class TArg3>
void UnitTest_PacketNMEAType_TestERR(TArg1 arg1, TArg2 arg2, TArg3 arg3, const std::string& msg)
{
	T A(arg1, arg2, arg3);
#ifdef SHOW_RESULTS
	std::cout << A.ToString() << " --> " << A.GetValue() << '\n';
#endif // SHOW_RESULTS
	utils::test::RESULT("ERR " + msg, !A.IsVerified());
}

void UnitTest_PacketNMEAType()
{
	std::cout << "\n""utils::packet::nmea::type\n";

	using namespace utils::packet::nmea::type;

	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GLONASS,			"tGNSS GL",					"GL");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GPS,				"tGNSS GP",					"GP");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GPS,				"tGNSS GP",					"GP");
	UnitTest_PacketNMEAType_Test<tGNSS>(tGNSS_State::GlobalNavigation,	"tGNSS GN",					"GN");
	UnitTest_PacketNMEAType_Test<tGNSS>("GL",							"tGNSS parse GL",			"GL");
	UnitTest_PacketNMEAType_Test<tGNSS>("GP",							"tGNSS parse GP",			"GP");
	UnitTest_PacketNMEAType_Test<tGNSS>("GN",							"tGNSS parse GN",			"GN");
	UnitTest_PacketNMEAType_TestERR<tGNSS>(								"tGNSS empty");
	UnitTest_PacketNMEAType_TestERR<tGNSS>("",							"tGNSS parse nothing");
	UnitTest_PacketNMEAType_TestERR<tGNSS>("GR",						"tGNSS parse GR");

	UnitTest_PacketNMEAType_Test<tStatus>(true,			"tValid true",			"A");
	UnitTest_PacketNMEAType_Test<tStatus>(false,		"tValid false",			"V");
	UnitTest_PacketNMEAType_Test<tStatus>("A",			"tValid parse A",		"A");
	UnitTest_PacketNMEAType_Test<tStatus>("V",			"tValid parse V",		"V");
	UnitTest_PacketNMEAType_TestERR<tStatus>(			"tValid empty");
	UnitTest_PacketNMEAType_TestERR<tStatus>("",		"tValid parse empty");
	UnitTest_PacketNMEAType_TestERR<tStatus>("R",		"tValid parse R");

	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(							"tUIntFixed<1> empty",			"");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(0,							"tUIntFixed<1> 0",				"0");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>(4,							"tUIntFixed<1> 4",				"4");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>("0",						"tUIntFixed<1> 0",				"0");
	UnitTest_PacketNMEAType_Test<tUIntFixed<1>>("4",						"tUIntFixed<1> 4",				"4");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(							"tUIntFixed<4> empty",			"");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(0,							"tUIntFixed<4> 0",				"0000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>(4567,						"tUIntFixed<4> 4567",			"4567");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>("0000",						"tUIntFixed<4> 0000",			"0000");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>("4567",						"tUIntFixed<4> 4567",			"4567");
	UnitTest_PacketNMEAType_Test<tUIntFixed<4>>("0567",						"tUIntFixed<4> 0567",			"0567");
	UnitTest_PacketNMEAType_TestERR<tUIntFixed<4>>(-567,					"tUIntFixed<4> -567");
	UnitTest_PacketNMEAType_TestERR<tUIntFixed<4>>("-567",					"tUIntFixed<4> -567");
	UnitTest_PacketNMEAType_TestERR<tUIntFixed<4>>(45678,					"tUIntFixed<4> 45678");
	UnitTest_PacketNMEAType_Test<tUIntFixedNoNull<4>>("0000",				"tUIntFixedNoNull<4> 0000",		"0000");
	UnitTest_PacketNMEAType_TestERR<tUIntFixedNoNull<4>>("",				"tUIntFixedNoNull<4> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUIntFixedNoNull<4>>(					"tUIntFixedNoNull<4> empty 2");
	
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>(						"tIntFixedStrict<2> empty 1",			"");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>(0,						"tIntFixedStrict<2>  0",				"00");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>(45,					"tIntFixedStrict<2> 45",				"45");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>(-4,					"tIntFixedStrict<2> -4",				"-4");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>("",					"tIntFixedStrict<2> empty 2",			"");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>("00",					"tIntFixedStrict<2> 00",				"00");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>("-0",					"tIntFixedStrict<2> -0",				"00");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>("-4",					"tIntFixedStrict<2> -4",				"-4");
	UnitTest_PacketNMEAType_Test<tIntFixedStrict<2>>("45",					"tIntFixedStrict<2> 45",				"45");
	UnitTest_PacketNMEAType_TestERR<tIntFixedStrict<2>>("-45",				"tIntFixedStrict<2> -45");
	UnitTest_PacketNMEAType_TestERR<tIntFixedStrict<2>>(-45,				"tIntFixedStrict<2> -45");
	UnitTest_PacketNMEAType_Test<tIntFixedStrictNoNull<2>>("00",			"tIntFixedStrictNoNull<2> 00",			"00");
	UnitTest_PacketNMEAType_TestERR<tIntFixedStrictNoNull<2>>("",			"tIntFixedStrictNoNull<2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tIntFixedStrictNoNull<2>>(				"tIntFixedStrictNoNull<2> empty 2");
	
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(								"tIntFixed<2> empty 1",				"");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(0,							"tIntFixed<2>  0",						"00");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(4,							"tIntFixed<2>  4",						"04");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(-4,							"tIntFixed<2> -4",						"-4");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(45,							"tIntFixed<2> 45",						"45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>(-45,							"tIntFixed<2> -45",					"-45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("",							"tIntFixed<2> empty 2",				"");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("00",						"tIntFixed<2> 00",						"00");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("-0",						"tIntFixed<2> -0",						"00");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("-4",						"tIntFixed<2> -4",						"-4");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("45",						"tIntFixed<2> 45",						"45");
	UnitTest_PacketNMEAType_Test<tIntFixed<2>>("-45",						"tIntFixed<2> -45",					"-45");
	UnitTest_PacketNMEAType_Test<tIntFixedNoNull<2>>("00",					"tIntFixedNoNull<2> 00",				"00");
	UnitTest_PacketNMEAType_TestERR<tIntFixedNoNull<2>>("",					"tIntFixedNoNull<2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tIntFixedNoNull<2>>(					"tIntFixedNoNull<2> empty 2");

	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>(					"tFloatFixedStrict<2,4> empty 1",				"");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>("",				"tFloatFixedStrict<2,4> empty 2",				"");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>("-0.0000",		"tFloatFixedStrict<2,4> -0.0000",				"00.0000");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>("-2.3456",		"tFloatFixedStrict<2,4> -2.3456",				"-2.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>(-2.3456,			"tFloatFixedStrict<2,4> -2.3456",				"-2.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>("02.3456",		"tFloatFixedStrict<2,4> 02.3456",				"02.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrict<2, 4>>(2.3456,			"tFloatFixedStrict<2,4>  2.3456",				"02.3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>("-12.3456",	"tFloatFixedStrict<2,4> -12.3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>("-02.3456",	"tFloatFixedStrict<2,4> -02.3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>(".3456",		"tFloatFixedStrict<2,4> .3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>(".",			"tFloatFixedStrict<2,4> .");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>(".3",			"tFloatFixedStrict<2,4> .3");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>("2.6",			"tFloatFixedStrict<2,4> 2.6");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrict<2, 4>>("12.",			"tFloatFixedStrict<2,4> 12. ");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictNoNull<2, 4>>("",		"tFloatFixedStrictNoNull<2,4> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictNoNull<2, 4>>(			"tFloatFixedStrictNoNull<2,4> empty 2");
	
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>(						"tFloatFixedStrict<2,4> empty 1",				"");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>("",						"tFloatFixedStrict<2,4> empty 2",				"");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>("-0.0000",				"tFloatFixedStrict<2,4> -0.0000",				"00.0000");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>("-2.3456",				"tFloatFixedStrict<2,4> -2.3456",				"-2.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>(-2.3456,				"tFloatFixedStrict<2,4> -2.3456",				"-2.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>("02.3456",				"tFloatFixedStrict<2,4> 02.3456",				"02.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>(2.3456,					"tFloatFixedStrict<2,4>  2.3456",				"02.3456");
	UnitTest_PacketNMEAType_Test<tFloatFixed<2, 4>>("-12.3456",				"tFloatFixedStrict<2,4> -12.3456",				"-12.3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>("-02.3456",			"tFloatFixedStrict<2,4> -02.3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>(".3456",				"tFloatFixedStrict<2,4> .3456");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>(".",					"tFloatFixedStrict<2,4> .");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>(".3",				"tFloatFixedStrict<2,4> .3");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>("2.6",				"tFloatFixedStrict<2,4> 2.6");
	UnitTest_PacketNMEAType_TestERR<tFloatFixed<2, 4>>("12.",				"tFloatFixedStrict<2,4> 12. ");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNull<2, 4>>("",			"tFloatFixedStrictNoNull<2,4> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNull<2, 4>>(				"tFloatFixedStrictNoNull<2,4> empty 2");

	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>(						"tUFloatFixed<2,4> empty 1",					"");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>(0,						"tUFloatFixed<2,4> 0",							"00.0000");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>("",					"tUFloatFixed<2,4> empty 2",					"");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>("00.0000",				"tUFloatFixed<2,4> 00.0000",					"00.0000");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>("12.3456",				"tUFloatFixed<2,4> 12.3456",					"12.3456");
	UnitTest_PacketNMEAType_Test<tUFloatFixed<2, 4>>(12.3456,				"tUFloatFixed<2,4> 12.3456",					"12.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixed<2, 4>>("127.3456",			"tUFloatFixed<2,4> 127.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixed<2, 4>>("7.3456",			"tUFloatFixed<2,4> 7.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixed<2, 4>>(127.3456,			"tUFloatFixed<2,4> 127.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNull<2, 4>>(				"tTypeNoNull<tUFloatFixed<2,4>> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNull<2, 4>>("",			"tTypeNoNull<tUFloatFixed<2,4>> empty 2");

	UnitTest_PacketNMEAType_Test<tInt<2>>(					"tInt<2> empty 1",					"");
	UnitTest_PacketNMEAType_Test<tInt<2>>(0,				"tInt<2> 0",						"0");
	UnitTest_PacketNMEAType_Test<tInt<2>>(5,				"tInt<2> 5",						"5");
	UnitTest_PacketNMEAType_Test<tInt<2>>(-5,				"tInt<2> -5",						"-5");
	UnitTest_PacketNMEAType_Test<tInt<2>>("",				"tInt<2> empty 2",					"");
	UnitTest_PacketNMEAType_Test<tInt<2>>("0",				"tInt<2> 0",						"0");
	UnitTest_PacketNMEAType_Test<tInt<2>>("-0",				"tInt<2> -0",						"0");
	UnitTest_PacketNMEAType_Test<tInt<2>>("5",				"tInt<2> 5",						"5");
	UnitTest_PacketNMEAType_Test<tInt<2>>("-5",				"tInt<2> -5",						"-5");
	UnitTest_PacketNMEAType_TestERR<tInt<2>>("-10",			"tInt<2> -10");
	UnitTest_PacketNMEAType_TestERR<tInt<2>>("-05",			"tInt<2> -05");
	UnitTest_PacketNMEAType_TestERR<tInt<2>>("103",			"tInt<2> 103");
	UnitTest_PacketNMEAType_TestERR<tInt<2>>(-10,			"tInt<2> -10");
	UnitTest_PacketNMEAType_TestERR<tInt<2>>(103,			"tInt<2> 103");
	UnitTest_PacketNMEAType_TestERR<tIntNoNull<2>>(			"tIntNoNull<2> empty 2");
	UnitTest_PacketNMEAType_TestERR<tIntNoNull<2>>("",		"tIntNoNull<2> empty 1");

	UnitTest_PacketNMEAType_Test<tUInt<1>>(					"tUInt<1> empty 1",					"");
	UnitTest_PacketNMEAType_Test<tUInt<1>>(0,				"tUInt<1> 0",						"0");
	UnitTest_PacketNMEAType_Test<tUInt<1>>(5,				"tUInt<1> 5",						"5");
	UnitTest_PacketNMEAType_Test<tUInt<1>>("",				"tUInt<1> empty 2",					"");
	UnitTest_PacketNMEAType_Test<tUInt<1>>("0",				"tUInt<1> 0",						"0");
	UnitTest_PacketNMEAType_Test<tUInt<1>>("5",				"tUInt<1> 5",						"5");
	UnitTest_PacketNMEAType_TestERR<tUInt<1>>(-5,			"tUInt<1> -5");
	UnitTest_PacketNMEAType_TestERR<tUInt<1>>(4567,			"tUInt<1> 4567");
	UnitTest_PacketNMEAType_TestERR<tUInt<1>>("-0",			"tUInt<1> -0");
	UnitTest_PacketNMEAType_TestERR<tUInt<1>>("-5",			"tUInt<1> -5");
	UnitTest_PacketNMEAType_TestERR<tUInt<1>>("10",			"tUInt<1> 10");
	UnitTest_PacketNMEAType_TestERR<tUIntNoNull<1>>(		"tUIntNoNull<1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUIntNoNull<1>>("",		"tUIntNoNull<1> empty 2");
	
	UnitTest_PacketNMEAType_Test<tUInt<2>>(					"tUInt<2> empty",					"");
	UnitTest_PacketNMEAType_Test<tUInt<2>>(0,				"tUInt<2> 0",						"0");
	UnitTest_PacketNMEAType_Test<tUInt<2>>(5,				"tUInt<2> 5",						"5");
	UnitTest_PacketNMEAType_Test<tUInt<2>>(10,				"tUInt<2> 10",						"10");
	UnitTest_PacketNMEAType_Test<tUInt<2>>("0",				"tUInt<2> 0",						"0");
	UnitTest_PacketNMEAType_Test<tUInt<2>>("5",				"tUInt<2> 5",						"5");
	UnitTest_PacketNMEAType_Test<tUInt<2>>("10",			"tUInt<2> 10",						"10");
	UnitTest_PacketNMEAType_TestERR<tUInt<2>>(-5,			"tUInt<2> -5");
	UnitTest_PacketNMEAType_TestERR<tUInt<2>>(-10,			"tUInt<2> -10");
	UnitTest_PacketNMEAType_TestERR<tUInt<2>>(4567,			"tUInt<2> 4567");
	UnitTest_PacketNMEAType_TestERR<tUInt<2>>("-5",			"tUInt<2> -5");
	UnitTest_PacketNMEAType_TestERR<tUInt<2>>("-10",		"tUInt<2> -10");
	
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>(					"tFloat<2,4> empty 1",			"");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>(-1.3456,			"tFloat<2,4> -1.3456",			"-1.3456");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("",				"tFloat<2,4> empty 2",			"");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("-0.0000",		"tFloat<2,4> -0.0000",			"0.0");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("-2.3",			"tFloat<2,4> -2.3",				"-2.3");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("2.6",			"tFloat<2,4> 2.6",				"2.6");
	UnitTest_PacketNMEAType_Test<tFloat<2, 4>>("12.3456",		"tFloat<2,4> 12.3456",			"12.3456");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>(-12.3456,		"tFloat<2,4> -12.3456");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>("-12.3",		"tFloat<2,4> -12.3");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>("127.3456",	"tFloat<2,4> 123.3456");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>("12.34567",	"tFloat<2,4> 123.34567");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>(".3456",		"tFloat<2,4> .3456");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>(".",			"tFloat<2,4> .");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>(".3",			"tFloat<2,4> .3");
	UnitTest_PacketNMEAType_TestERR<tFloat<2, 4>>("12.",		"tFloat<2,4> 12.");
	UnitTest_PacketNMEAType_TestERR<tFloatNoNull<2, 4>>(		"tFloatNoNull<2,4> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatNoNull<2, 4>>("",		"tFloatNoNull<2,4> empty 2");

	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>(				"tUFloat<2,4> empty 1",			"");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>(12.3456,		"tUFloat<2,4> 12.3456",			"12.3456");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>(1.3456,			"tUFloat<2,4> 1.3456",			"1.3456");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>("",				"tUFloat<2,4> empty 2",			"");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>("00.0000",		"tUFloat<2,4> 00.0000",			"0.0");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>("12.3",			"tUFloat<2,4> 12.3",			"12.3");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>("2.3",			"tUFloat<2,4> 2.3",				"2.3");
	UnitTest_PacketNMEAType_Test<tUFloat<2, 4>>("12.3456",		"tUFloat<2,4> 12.3456",			"12.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloat<2, 4>>(-1.3456,		"tUFloat<2,4> -1.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloat<2, 4>>("127.3456",	"tUFloat<2,4> 123.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloat<2, 4>>("-7.3456",	"tUFloat<2,4> -7.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatNoNull<2, 4>>(		"tFloatNoNull<2,4> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatNoNull<2, 4>>("",	"tFloatNoNull<2,4> empty 2");

	// Precision Fixed
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>(					"tFloatPrecisionFixed<5,2> empty 1",		"");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>(0,					"tFloatPrecisionFixed<5,2> 0.00",			"0.00");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>(-10.3456,			"tFloatPrecisionFixed<5,2> -10.35",			"-10.35");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>("",				"tFloatPrecisionFixed<5,2> empty 2",		"");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>("12345.67",		"tFloatPrecisionFixed<5,2> 12345.67",		"12345.67");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>("-2345.67",		"tFloatPrecisionFixed<5,2> -2345.67",		"-2345.67");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixed<5, 2>>(123.4567834,		"tFloatPrecisionFixed<5,2> 123.46",			"123.46");
	UnitTest_PacketNMEAType_TestERR<tFloatPrecisionFixed<5, 2>>("1234599.67",	"tFloatPrecisionFixed<5,2> 1234599.67");
	UnitTest_PacketNMEAType_TestERR<tFloatPrecisionFixedNoNull<5, 2>>(			"tTypeNoNull<tFloatPrecisionFixed<5, 2>> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatPrecisionFixedNoNull<5, 2>>("",		"tTypeNoNull<tFloatPrecisionFixed<5, 2>> empty 2");

	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<5, 2>>(					"tUFloatPrecisionFixed<5,2> empty 1",		"");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<5, 2>>("",				"tUFloatPrecisionFixed<5,2> empty 2",		"");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<5, 2>>(0,				"tUFloatPrecisionFixed<5,2> 0.00",			"0.00");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<5, 2>>("12345.67",		"tUFloatPrecisionFixed<5,2> 12345.67",		"12345.67");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixed<5, 2>>(123.4567834,		"tUFloatPrecisionFixed<5,2> 231.46",		"123.46");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixed<5, 2>>(-10.3456,		"tUFloatPrecisionFixed<5,2> -10.3456");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixed<5, 2>>("-12345.67",	"tUFloatPrecisionFixed<5,2> 12345.67");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedNoNull<5, 2>>(			"tUFloatPrecisionFixedNoNull<5, 2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedNoNull<5, 2>>("",		"tUFloatPrecisionFixedNoNull<5, 2> empty 2");

	// Unit
	UnitTest_PacketNMEAType_Test<tFloatFixedStrictUnit<5, 1>>(								"tFloatFixedStrictUnit<5,1> empty 1",		",");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrictUnit<5, 1>>("",					"",		"tFloatFixedStrictUnit<5,1> empty 2",		",");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrictUnit<5, 1>>("",					"R",	"tFloatFixedStrictUnit<5,1> ,R",			",R");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrictUnit<5, 1>>("00000.0",			"",		"tFloatFixedStrictUnit<5,1> 00000.0,",		"00000.0,");
	UnitTest_PacketNMEAType_Test<tFloatFixedStrictUnit<5, 1>>("-0001.2",			"",		"tFloatFixedStrictUnit<5,1> -0001.2,",		"-0001.2,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictUnit<5, 1>>("-12341.2",		"",		"tFloatFixedStrictUnit<5,1> -12341.2,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictUnitNoNull<5, 1>>(						"tFloatFixedStrictUnitNoNull<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictUnitNoNull<5, 1>>("",			"",		"tFloatFixedStrictUnitNoNull<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedStrictUnitNoNull<5, 1>>("00000.0",	"",		"tFloatFixedStrictUnitNoNull<5,1> 00000.0,");

	UnitTest_PacketNMEAType_Test<tFloatFixedUnit<5, 1>>(									"tFloatFixedUnit<5,1> empty 1",			",");
	UnitTest_PacketNMEAType_Test<tFloatFixedUnit<5, 1>>("",							"",		"tFloatFixedUnit<5,1> empty 2",			",");
	UnitTest_PacketNMEAType_Test<tFloatFixedUnit<5, 1>>("",							"R",	"tFloatFixedUnit<5,1> ,R",				",R");
	UnitTest_PacketNMEAType_Test<tFloatFixedUnit<5, 1>>("00000.0",					"",		"tFloatFixedUnit<5,1> 00000.0,",		"00000.0,");
	UnitTest_PacketNMEAType_Test<tFloatFixedUnit<5, 1>>("-12341.2",					"",		"tFloatFixedUnit<5,1> -12341.2,",		"-12341.2,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedUnitNoNull<5, 1>>(							"tFloatFixedUnitNoNull<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedUnitNoNull<5, 1>>("",				"",		"tFloatFixedUnitNoNull<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedUnitNoNull<5, 1>>("00000.0",			"",		"tFloatFixedUnitNoNull<5,1> 00000.0,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnit<5, 1>>(							"tFloatFixedNoNullUnit<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnit<5, 1>>("",				"",		"tFloatFixedNoNullUnit<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnit<5, 1>>("",				"R",	"tFloatFixedNoNullUnit<5,1> ,R,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnitNoNull<5, 1>>(						"tFloatFixedNoNullUnitNoNull<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnitNoNull<5, 1>>("",			"",		"tFloatFixedNoNullUnitNoNull<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnitNoNull<5, 1>>("00000.0",	"",		"tFloatFixedNoNullUnitNoNull<5,1> 00000.0,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnitNoNull<5, 1>>("-0014.2",	"",		"tFloatFixedNoNullUnitNoNull<5,1> -0014.2,");
	UnitTest_PacketNMEAType_TestERR<tFloatFixedNoNullUnitNoNull<5, 1>>("",			"R",	"tFloatFixedNoNullUnitNoNull<5,1> ,R,");

	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnit<5, 1>>(							"tUFloatFixedNoNullUnit<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnit<5, 1>>("",				"",		"tUFloatFixedNoNullUnit<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnit<5, 1>>("",				"R",	"tUFloatFixedNoNullUnit<5,1> ,R,");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnitNoNull<5, 1>>(					"tUFloatFixedNoNullUnitNoNull<5,1> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnitNoNull<5, 1>>("",			"",		"tUFloatFixedNoNullUnitNoNull<5,1> empty 2");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnitNoNull<5, 1>>("00000.0",	"",		"tUFloatFixedNoNullUnitNoNull<5,1> 00000.0,");
	UnitTest_PacketNMEAType_TestERR<tUFloatFixedNoNullUnitNoNull<5, 1>>("",			"R",	"tUFloatFixedNoNullUnitNoNull<5,1> ,R,");

	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>(						"tFloatPrecisionFixedUnit<5,4> empty 1",		",");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>(12.3456,		'W',	"tFloatPrecisionFixedUnit<5,4> 12.3456,W",		"12.3456,W");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>(-12.3456,		'W',	"tFloatPrecisionFixedUnit<5,4> -12.3456,W",		"-12.3456,W");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>(1.3,			'W',	"tFloatPrecisionFixedUnit<5,4> 1.3000,W",		"1.3000,W");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>("",			"",		"tFloatPrecisionFixedUnit<5,4> empty 2",		",");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>("",			"R",	"tFloatPrecisionFixedUnit<5,4> ,R",				",R");
	UnitTest_PacketNMEAType_Test<tFloatPrecisionFixedUnit<5, 4>>("12.3456",		"R",	"tFloatPrecisionFixedUnit<5,4> 12.3456,R",		"12.3456,R");
	UnitTest_PacketNMEAType_TestERR<tFloatPrecisionFixedUnitNoNull<5, 2>>(				"tFloatPrecisionFixedUnitNoNull<5, 2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tFloatPrecisionFixedUnitNoNull<5, 2>>("",	"",		"tFloatPrecisionFixedUnitNoNull<5, 2> empty 2");
	
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>(						"tUFloatPrecisionFixedUnit<5,4> empty 1",		",");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>(12.3456,		'W',	"tUFloatPrecisionFixedUnit<5,4> 12.3456,W",		"12.3456,W");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>(1.3,			'W',	"tUFloatPrecisionFixedUnit<5,4> 1.3000,W",		"1.3000,W");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>("",			"",		"tUFloatPrecisionFixedUnit<5,4> empty 2",		",");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>("",			"R",	"tUFloatPrecisionFixedUnit<5,4> ,R", ",R");
	UnitTest_PacketNMEAType_Test<tUFloatPrecisionFixedUnit<5, 4>>("12.3456",	"R",	"tUFloatPrecisionFixedUnit<5,4> 12.3456,R",		"12.3456,R");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedUnit<5, 4>>(-12.3456,	'W',	"tUFloatPrecisionFixedUnit<5,4> -12.3456,W");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedUnit<5, 4>>(-12.3456,	0,		"tUFloatPrecisionFixedUnit<5,4> -12.3456,");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedUnitNoNull<5, 2>>(				"tFloatPrecisionFixedUnitNoNull<5, 2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tUFloatPrecisionFixedUnitNoNull<5, 2>>("",	"",		"tFloatPrecisionFixedUnitNoNull<5, 2> empty 2");

	// GNSS
	UnitTest_PacketNMEAType_Test<tDate>(					"tDate empty 1",					"");
	UnitTest_PacketNMEAType_Test<tDate>("",					"tDate empty 2",					"");
	UnitTest_PacketNMEAType_Test<tDate>(3, 2, 1,			"tDate 3,2,1",						"010203");
	UnitTest_PacketNMEAType_Test<tDate>(99, 12, 31,			"tDate 99,12,31",					"311299");
	UnitTest_PacketNMEAType_Test<tDate>("010203",			"tDate parse 010203",				"010203");
	UnitTest_PacketNMEAType_Test<tDate>("311299",			"tDate parse 311299",				"311299");
	UnitTest_PacketNMEAType_Test<tDate>(1044057600,			"tDate 3,2,1",						"010203");
	UnitTest_PacketNMEAType_Test<tDate>(946598400,			"tDate 99,12,31",					"311299");
	UnitTest_PacketNMEAType_TestERR<tDate>(100, 13, 33,		"tDate 100,12,31 (wrong)");
	//UnitTest_PacketNMEAType_TestERR<tDate>("", "tDate parse empty");
	UnitTest_PacketNMEAType_TestERR<tDate>("501333",		"tDate parse 501333 (wrong)");
	UnitTest_PacketNMEAType_TestERR<tDate>("01020",			"tDate parse 01020 (wrong)");
	UnitTest_PacketNMEAType_TestERR<tDate>("0102035",		"tDate parse 0102035 (wrong)");
	UnitTest_PacketNMEAType_TestERR<tDateNoNull>(			"tTypeNoNull<tDate> empty 1");
	UnitTest_PacketNMEAType_TestERR<tDateNoNull>("",		"tTypeNoNull<tDate> empty 2");
	
	UnitTest_PacketNMEAType_Test<tTime<0>>(						"tTime<0> empty 1",					"");
	UnitTest_PacketNMEAType_Test<tTime<0>>("",					"tTime<0> empty 2",					"");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3.4567890,		"tTime<0> 1, 2, 3.4567890",			"010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3.45,			"tTime<0> 1, 2, 3.45",				"010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(1, 2, 3,				"tTime<0> 1, 2, 3",					"010203");
	UnitTest_PacketNMEAType_Test<tTime<0>>(0, 0, 0,				"tTime<0> 0, 0, 0",					"000000");
	UnitTest_PacketNMEAType_Test<tTime<0>>("000000",			"tTime<0> 000000",					"000000");
	UnitTest_PacketNMEAType_Test<tTime<0>>("010203",			"tTime<0> 010203",					"010203");
	UnitTest_PacketNMEAType_TestERR<tTime<0>>(32, 64, 78.9999,	"tTime<0> 32, 64, 78.9999 (wrong)");
	UnitTest_PacketNMEAType_TestERR<tTimeNoNull<0>>(			"tTypeNoNull<tTime<0>> empty 1");
	UnitTest_PacketNMEAType_TestERR<tTimeNoNull<0>>("",			"tTypeNoNull<ttTime<0>Date> empty 2");

	UnitTest_PacketNMEAType_Test<tTime<2>>(						"tTime<2> empty 1",					"");
	UnitTest_PacketNMEAType_Test<tTime<2>>("",					"tTime<2> empty 2",					"");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3.4567890,		"tTime<2> 1, 2, 3.4567890",			"010203.46");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3.45,			"tTime<2> 1, 2, 3.45",				"010203.45");
	UnitTest_PacketNMEAType_Test<tTime<2>>(1, 2, 3,				"tTime<2> 1, 2, 3",					"010203.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>(0, 0, 0,				"tTime<2> 0, 0, 0",					"000000.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>("000000.00",			"tTime<2> 000000.00",				"000000.00");
	UnitTest_PacketNMEAType_Test<tTime<2>>("010203.45",			"tTime<2> 010203.45",				"010203.45");
	UnitTest_PacketNMEAType_TestERR<tTime<2>>(32, 64, 78.9999,	"tTime<2> 32, 64, 78.9999 (wrong)");
	UnitTest_PacketNMEAType_TestERR<tTimeNoNull<2>>(			"tTypeNoNull<tTime<2>> empty 1");
	UnitTest_PacketNMEAType_TestERR<tTimeNoNull<2>>("",			"tTypeNoNull<tTime<2>> empty 2");

	UnitTest_PacketNMEAType_Test<tTime<3>>(						"tTime<3> empty",					"");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3.4567890,		"tTime<3> 1, 2, 3.4567890",			"010203.457");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3.45,			"tTime<3> 1, 2, 3.45",				"010203.450");
	UnitTest_PacketNMEAType_Test<tTime<3>>(1, 2, 3,				"tTime<3> 1, 2, 3",					"010203.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>(0, 0, 0,				"tTime<3> 0, 0, 0",					"000000.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>("000000.000",		"tTime<3> 000000.000",				"000000.000");
	UnitTest_PacketNMEAType_Test<tTime<3>>("010203.456",		"tTime<3> 010203.456",				"010203.456");
	UnitTest_PacketNMEAType_TestERR<tTime<3>>(32, 64, 78.9999, "tTime<3> 32, 64, 78.9999 (wrong)");
	
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(							"tLatitude<2> empty 1",		",");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>("",				"",		"tLatitude<2> empty 2",		",");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(0,						"tLatitude<2> 0",			"0000.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>("1234.56",		"S",	"tLatitude<2> 1234.56,S",	"1234.56,S");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(31.45,					"tLatitude<2> 31.45",		"3127.00,N");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(-31.45,					"tLatitude<2> -31.45",		"3127.00,S");
	UnitTest_PacketNMEAType_Test<tLatitude<2>>(34,						"tLatitude<2> 34",			"3400.00,N");
	UnitTest_PacketNMEAType_TestERR<tLatitude<2>>("1234.56",	"K",	"tLatitude<2> 1234.56,K");
	UnitTest_PacketNMEAType_TestERR<tLatitudeNoNull<2>>(				"tLatitudeNoNull<2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tLatitudeNoNull<2>>("",		"",		"tLatitudeNoNull<2> empty 2");

	UnitTest_PacketNMEAType_Test<tLatitude<4>>(							"tLatitude<4> empty",		",");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>(0,						"tLatitude<4> 0",			"0000.0000,N");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>("1234.5678",		"S",	"tLatitude<4> 1234.5678,S",	"1234.5678,S");
	UnitTest_PacketNMEAType_Test<tLatitude<4>>(31.45678,				"tLatitude<4> 31.45678",	"3127.4068,N");

	UnitTest_PacketNMEAType_Test<tLatitude<6>>(							"tLatitude<6> empty",			",");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>(0,						"tLatitude<6> 0",				"0000.000000,N");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>("1234.567891",	"N",	"tLatitude<6> 1234.567891,N",	"1234.567891,N");
	UnitTest_PacketNMEAType_Test<tLatitude<6>>(31.4567834,				"tLatitude<6> 31.4567834",		"3127.407004,N");

	UnitTest_PacketNMEAType_Test<tLongitude<2>>(						"tLongitude<2> empty 1",		",");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>("",				"",		"tLongitude<2> empty 2",		",");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(0,						"tLongitude<2> 0",				"00000.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>("01234.56",		"W",	"tLongitude<2> 0",				"01234.56,W");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(31.45,					"tLongitude<2> 0",				"03127.00,E");
	UnitTest_PacketNMEAType_Test<tLongitude<2>>(31,						"tLongitude<2> 0",				"03100.00,E");
	UnitTest_PacketNMEAType_TestERR<tLongitudeNoNull<2>>(				"tLongitudeNoNull<2> empty 1");
	UnitTest_PacketNMEAType_TestERR<tLongitudeNoNull<2>>("",	"",		"tLongitudeNoNull<2> empty 2");

	UnitTest_PacketNMEAType_Test<tLongitude<4>>(						"tLongitude4 empty 1",			",");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>("",				"",		"tLongitude4 empty 2",			",");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>(0,						"tLongitude4 00000.0000,E",		"00000.0000,E");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>("12345.6789",	"W",	"tLongitude4 12345.6789,W",		"12345.6789,W");
	UnitTest_PacketNMEAType_Test<tLongitude<4>>(31.45678,				"tLongitude4 31.45678",			"03127.4068,E");

	UnitTest_PacketNMEAType_Test<tLongitude<6>>(						"tLongitude6 empty",			",");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>(0,						"tLongitude6 00000.000000,E",	"00000.000000,E");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>("12345.678901",	"E",	"tLongitude6 12345.678901,E",	"12345.678901,E");
	UnitTest_PacketNMEAType_Test<tLongitude<6>>(123.4567834,			"tLongitude6 231.4567834",		"12327.407004,E");

	UnitTest_PacketNMEAType_Test<tMode>(				"tMode empty 1",	"");
	UnitTest_PacketNMEAType_Test<tMode>("",				"tMode empty 2",	"");
	UnitTest_PacketNMEAType_Test<tMode>("A",			"tMode 1",			"A");
	UnitTest_PacketNMEAType_TestERR<tMode>("AS",		"tMode 3");
	UnitTest_PacketNMEAType_Test<tModeNoNull>("A",		"tModeNoNull 1",	"A");
	UnitTest_PacketNMEAType_TestERR<tModeNoNull>("",	"tModeNoNull 2");

	UnitTest_PacketNMEAType_Test<tQuality>(					"tQuality empty 1",		"");
	UnitTest_PacketNMEAType_Test<tQuality>("",				"tQuality empty 2",		"");
	UnitTest_PacketNMEAType_Test<tQuality>("1",				"tQuality 1",			"1");
	UnitTest_PacketNMEAType_Test<tQuality>("8",				"tQuality 2",			"8");
	UnitTest_PacketNMEAType_TestERR<tQuality>("9",			"tQuality 1");
	UnitTest_PacketNMEAType_Test<tQualityNoNull>("1",		"tQualityNoNull 1",		"1");
	UnitTest_PacketNMEAType_Test<tQualityNoNull>("8",		"tQualityNoNull 2",		"8");
	UnitTest_PacketNMEAType_TestERR<tQualityNoNull>(		"tQualityNoNull empty 1");
	UnitTest_PacketNMEAType_TestERR<tQualityNoNull>("",		"tQualityNoNull empty 2");
	UnitTest_PacketNMEAType_TestERR<tQualityNoNull>("9",	"tQualityNoNull 1");

	std::cout << std::endl;
}

}
