#include "utilsPacketNMEAPayloadPUBX.h"

namespace utils
{
namespace packet
{
namespace nmea
{
namespace ublox
{
///////////////////////////////////////////////////////////////////////////////////////////////////
bool IsUnknownMessage(const base::tContentTXT& content)
{
	return content.Msg.ends_with("unknown msg");
}

bool IsVersionHW(const base::tContentTXT& content)
{
	return content.Msg.starts_with("ANTARIS");
}

bool IsVersionExt(const base::tContentTXT& content)
{
	return content.Msg.starts_with("EXT");
}

bool IsVersionInt(const base::tContentTXT& content)
{
	return content.Msg.starts_with("INT");
}

bool IsLicense(const base::tContentTXT& content)
{
	return content.Msg.starts_with("LIC");
}

bool IsQDS(const base::tContentTXT& content)
{
	return content.Msg.starts_with("QDS");
}
///////////////////////////////////////////////////////////////////////////////////////////////////
}
}
}
}
