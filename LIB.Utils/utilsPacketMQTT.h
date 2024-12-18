#include <queue>

#include <cstdint>

namespace utils
{
namespace packet_MQTT
{
// ... you cannot use a string that would encode to more than 65535 bytes.
// Unless stated otherwise all UTF-8 encoded strings can have any length in the range 0 to 65535 bytes.

// The character data in a UTF-8 encoded string MUST be well-formed UTF-8 as defined by the Unicode 190 specification [Unicode] and restated in RFC 3629 [RFC3629].
// In particular this data MUST NOT include 191 encodings of code points between U+D800 and U+DFFF.
// If a Server or Client receives a Control Packet 192 containing ill-formed UTF-8 it MUST close the Network Connection [MQTT-1.5.3-1].

// A UTF - 8 encoded string MUST NOT include an encoding of the null character U + 0000. If a receiver 195 (Server or Client) receives a Control Packet containing U + 0000 it MUST close the Network 196 Connection[MQTT - 1.5.3 - 2].

// A UTF-8 encoded sequence 0xEF 0xBB 0xBF is always to be interpreted to mean U+FEFF ("ZERO 207 WIDTH NO-BREAK SPACE") wherever it appears in a string and MUST NOT be skipped over or stripped 208 off by a packet receiver [MQTT-1.5.3-3].

enum class tControlPacketType
{
	Reserved_1,		// Forbidden
	CONNECT,		// Client to Server							Client request to connect to Server
	CONNACK,		// Server to Client							Connect acknowledgment
	PUBLISH,		// Client to Server or Server to Client		Publish message
	PUBACK,			// Client to Server or Server to Client		Publish acknowledgment
	PUBREC,			// Client to Server or Server to Client		Publish received (assured delivery part 1)
	PUBREL,			// Client to Server or Server to Client		Publish release (assured delivery part 2)
	PUBCOMP,		// Client to Server or Server to Client		Publish complete (assured delivery part 3)
	SUBSCRIBE,		// Client to Server							Client subscribe request
	SUBACK,			// Server to Client							Subscribe acknowledgment
	UNSUBSCRIBE,	// Client to Server							Unsubscribe request
	UNSUBACK,		// Server to Client							Unsubscribe acknowledgment
	PINGREQ,		// Client to Server							PING request
	PINGRESP,		// Server to Client							PING response
	DISCONNECT,		// Client to Server							Client is disconnecting
	Reserved_2		// Forbidden
};

// Where a flag bit is marked as �Reserved� in Table 2.2 - 243 Flag Bits, it is reserved for future use and MUST be set to the value listed in that table[MQTT - 2.2.2 - 1].If 244 invalid flags are received, the receiver MUST close the Network Connection[MQTT - 2.2.2 - 2].

union tFixedHeader
{
	struct
	{
		std::uint8_t Flags : 4;
		std::uint8_t ControlPacketType : 4;
	}Field;
	std::uint8_t Value = 0;
};

static tFixedHeader MakeFixedHeader(tControlPacketType type, std::uint8_t flags = 0)
{
	tFixedHeader Header{};
	Header.Field.ControlPacketType = static_cast<std::uint8_t>(type);
	Header.Field.Flags = flags;
	return Header;
}

static tFixedHeader MakeCONNECT()
{
	return MakeFixedHeader(tControlPacketType::CONNECT);
}

static tFixedHeader MakeCONNACK()
{
	return MakeFixedHeader(tControlPacketType::CONNACK);
}

static tFixedHeader MakePUBLISH(bool dup, std::uint8_t qos, bool retain)
{
	union
	{
		struct
		{
			std::uint8_t RETAIN : 1;
			std::uint8_t QoS : 2;
			std::uint8_t DUP : 1;
			std::uint8_t : 4;
		}Field;
		std::uint8_t Value = 0;
	}Flags;

	Flags.Field.DUP = dup ? 1 : 0;
	Flags.Field.QoS = qos;
	Flags.Field.RETAIN = retain ? 1 : 0;

	return MakeFixedHeader(tControlPacketType::PUBLISH, Flags.Value);
}


static tFixedHeader MakeSUBSCRIBE()
{
	return MakeFixedHeader(tControlPacketType::SUBSCRIBE, 0x02);
}


struct tRemainingLength
{
	// 0xFF, 0xFF, 0xFF, 0x7F = 256 MB
	union tLengthPart
	{
		struct
		{
			std::uint8_t Num : 6;
			std::uint8_t Continuation : 1;
		}Field;
		std::uint8_t Value = 0;
	};

};

class tPacket
{
};

class tPacketParser
{
	std::queue<std::uint8_t> m_Queue;

public:
	tPacketParser() = default;

	void push_back(std::vector<std::uint8_t>& data) {}

	virtual tPacket OnReceived() = 0;
};

}
}
