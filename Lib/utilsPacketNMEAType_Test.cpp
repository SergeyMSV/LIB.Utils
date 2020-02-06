#include "utilsPacketNMEAType.h"

#include "utilsBase.h"
#include "utilsTest.h"

#include <iostream>
#include <iomanip>
//#include <tuple>

namespace utils
{

template <class T>
void UnitTest_PacketNMEAPayload_Test()
{
	T Val;
	std::cout << Val.ToString() << '\n';
}

template <class T, class TArg>
void UnitTest_PacketNMEAPayload_Test(TArg arg)
{
	T Val(arg);
	std::cout << Val.ToString() << '\n';
}

template <class T, class TArg>
void UnitTest_PacketNMEAPayload_Test(TArg arg1, TArg arg2)
{
	T Val(arg1, arg2);
	std::cout << Val.ToString() << '\n';
}

void UnitTest_PacketNMEAType()
{
	std::cout << "\n""utils::packet_NMEA::Payload\n";

	using namespace utils::packet_NMEA::Type;

	UnitTest_PacketNMEAPayload_Test<tDate>();
	UnitTest_PacketNMEAPayload_Test<tDate>("120517");
	//std::tuple<tUInt8, tUInt8, tUInt8> Args = std::make_tuple( 17, 4, 19 );
	//UnitTest_PacketNMEAPayload_Test<tDate>(Args);
	{
		tDate Val(17, 4, 19);

		std::cout << Val.ToString() << '\n';
	}

	typedef tTime<0> tTime0;
	UnitTest_PacketNMEAPayload_Test<tTime0>();
	UnitTest_PacketNMEAPayload_Test<tTime0>("192758");
	UnitTest_PacketNMEAPayload_Test<tTime0>("000000");
	{
		tTime0 Val(17, 4, 19);

		std::cout << Val.ToString() << '\n';
	}

	typedef tTime<3> tTime3;
	UnitTest_PacketNMEAPayload_Test<tTime3>();
	UnitTest_PacketNMEAPayload_Test<tTime3>("192758.345");
	UnitTest_PacketNMEAPayload_Test<tTime3>("000000.000");
	{
		tTime3 Val(17, 4, 19.524);

		std::cout << Val << '\n';
		std::cout << Val.ToString() << '\n';
	}

	typedef tLatitude<2> tLatitude2;
	UnitTest_PacketNMEAPayload_Test<tLatitude2>();
	UnitTest_PacketNMEAPayload_Test<tLatitude2>(0);
	UnitTest_PacketNMEAPayload_Test<tLatitude2>("5539.56", "S");
	UnitTest_PacketNMEAPayload_Test<tLatitude2>(31.45);
	UnitTest_PacketNMEAPayload_Test<tLatitude2>(34);

	typedef tLatitude<4> tLatitude4;
	UnitTest_PacketNMEAPayload_Test<tLatitude4>();
	UnitTest_PacketNMEAPayload_Test<tLatitude4>(0);
	UnitTest_PacketNMEAPayload_Test<tLatitude4>("5539.5649", "S");
	UnitTest_PacketNMEAPayload_Test<tLatitude4>(31.45678);

	typedef tLatitude<6> tLatitude6;
	UnitTest_PacketNMEAPayload_Test<tLatitude6>();
	UnitTest_PacketNMEAPayload_Test<tLatitude6>(0);
	UnitTest_PacketNMEAPayload_Test<tLatitude6>("5539.564975", "N");
	UnitTest_PacketNMEAPayload_Test<tLatitude6>(31.4567834);

	typedef tLongitude<2> tLongitude2;
	UnitTest_PacketNMEAPayload_Test<tLongitude2>();
	UnitTest_PacketNMEAPayload_Test<tLongitude2>(0);
	UnitTest_PacketNMEAPayload_Test<tLongitude2>("03732.41", "W");
	UnitTest_PacketNMEAPayload_Test<tLongitude2>(31.45);
	UnitTest_PacketNMEAPayload_Test<tLongitude2>(31);

	typedef tLongitude<4> tLongitude4;
	UnitTest_PacketNMEAPayload_Test<tLongitude4>();
	UnitTest_PacketNMEAPayload_Test<tLongitude4>(0);
	UnitTest_PacketNMEAPayload_Test<tLongitude4>("03732.4119", "W");
	UnitTest_PacketNMEAPayload_Test<tLongitude4>(31.45678);

	typedef tLongitude<6> tLongitude6;
	UnitTest_PacketNMEAPayload_Test<tLongitude6>();
	UnitTest_PacketNMEAPayload_Test<tLongitude6>(0);
	UnitTest_PacketNMEAPayload_Test<tLongitude6>("03732.411956", "E");
	UnitTest_PacketNMEAPayload_Test<tLongitude6>(231.4567834);

	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>();
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>(23);
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>("34.5688");
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>(52.3434);
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>(2.34);

	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>();
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>(0);
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>("4.56");
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>(2.34);
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>("1234.56");
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 2>>(3.3418);

	UnitTest_PacketNMEAPayload_Test<tFloat<0, 4>>();
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 4>>(0);
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 4>>("4.5736");
	UnitTest_PacketNMEAPayload_Test<tFloat<0, 4>>(2.34);

	//UnitTest_PacketNMEAPayload_Test<tFloat<0, 0>>();//fract is just of max length (6 chars)
	//UnitTest_PacketNMEAPayload_Test<tFloat<0, 0>>(0);
	//UnitTest_PacketNMEAPayload_Test<tFloat<0, 0>>("4.5736");
	//UnitTest_PacketNMEAPayload_Test<tFloat<0, 0>>(72.34);

	//UnitTest_PacketNMEAPayload_Test<tFloat<2, 0>>();//doesn't exist
	//UnitTest_PacketNMEAPayload_Test<tFloat<2, 0>>(0);//doesn't exist
	//UnitTest_PacketNMEAPayload_Test<tFloat<2, 0>>("4.5736");//doesn't exist
	//UnitTest_PacketNMEAPayload_Test<tFloat<2, 0>>(2.34);//doesn't exist

	UnitTest_PacketNMEAPayload_Test<tUInt<4>>();
	UnitTest_PacketNMEAPayload_Test<tUInt<4>>(0);
	UnitTest_PacketNMEAPayload_Test<tUInt<4>>("4356");
	UnitTest_PacketNMEAPayload_Test<tUInt<4>>(2734);

	UnitTest_PacketNMEAPayload_Test<tUInt<0>>();
	UnitTest_PacketNMEAPayload_Test<tUInt<0>>(0);
	UnitTest_PacketNMEAPayload_Test<tUInt<0>>("43586");
	UnitTest_PacketNMEAPayload_Test<tUInt<0>>(27384);

	std::cout << std::endl;
}

}
