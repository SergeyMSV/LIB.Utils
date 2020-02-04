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

	typedef tTime<6> tTime6;
	UnitTest_PacketNMEAPayload_Test<tTime6>();
	UnitTest_PacketNMEAPayload_Test<tTime6>("192758");
	UnitTest_PacketNMEAPayload_Test<tTime6>("000000");
	{
		tTime6 Val(17, 4, 19);

		std::cout << Val.ToString() << '\n';
	}

	typedef tTime<10> tTime10;
	UnitTest_PacketNMEAPayload_Test<tTime10>();
	UnitTest_PacketNMEAPayload_Test<tTime10>("192758.345");
	UnitTest_PacketNMEAPayload_Test<tTime10>("000000.000");
	{
		tTime10 Val(17, 4, 19.524);

		std::cout << Val.ToString() << '\n';
	}

	typedef tLatitude<9> tLatitude9;
	UnitTest_PacketNMEAPayload_Test<tLatitude9>();
	UnitTest_PacketNMEAPayload_Test<tLatitude9>(0);
	UnitTest_PacketNMEAPayload_Test<tLatitude9>("5539.5649", "S");
	UnitTest_PacketNMEAPayload_Test<tLatitude9>(31.45678);

	typedef tLatitude<11> tLatitude11;
	UnitTest_PacketNMEAPayload_Test<tLatitude11>();
	UnitTest_PacketNMEAPayload_Test<tLatitude11>(0);
	UnitTest_PacketNMEAPayload_Test<tLatitude11>("5539.564975", "N");
	UnitTest_PacketNMEAPayload_Test<tLatitude11>(31.4567834);

	typedef tLongitude<10> tLongitude10;
	UnitTest_PacketNMEAPayload_Test<tLongitude10>();
	UnitTest_PacketNMEAPayload_Test<tLongitude10>(0);
	UnitTest_PacketNMEAPayload_Test<tLongitude10>("03732.4119", "W");
	UnitTest_PacketNMEAPayload_Test<tLongitude10>(31.45678);

	typedef tLongitude<12> tLongitude12;
	UnitTest_PacketNMEAPayload_Test<tLongitude12>();
	UnitTest_PacketNMEAPayload_Test<tLongitude12>(0);
	UnitTest_PacketNMEAPayload_Test<tLongitude12>("03732.411956", "E");
	UnitTest_PacketNMEAPayload_Test<tLongitude12>(231.4567834);

	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>();
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>(23);
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>("34.5688");
	UnitTest_PacketNMEAPayload_Test<tFloat<2, 4>>(52.3434);

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
