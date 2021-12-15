#include "utilsPacketStar.h"

namespace utils
{
	namespace packet_Star
	{

tPacketStarCmd tPacketStarCmd::MakeSomeMessage_01(const tVectorUInt8& msgData)
{
	tPayloadCmd::value_type Cmd;
	Cmd.MsgId = 0x01;
	Cmd.Payload = msgData;
	return tPacketStarCmd(Cmd);
}

// ... Make-functions for other packets

	}
}
