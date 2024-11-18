#include "utilsPacketMQTT.h"
#include "utilsTest.h"

//#include <cassert>

//#include <algorithm>
//#include <iostream>

namespace utils
{

using namespace packet_MQTT;

void UnitTest_PacketMQTT()
{
	tFixedHeader Fh = MakePUBLISH(false, 0xFF, false);
}

}