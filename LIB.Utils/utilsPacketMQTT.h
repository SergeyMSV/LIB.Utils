///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketMQTT
// 2024-11-22
// C++23
///////////////////////////////////////////////////////////////////////////////////////////////////
// Based on mqtt - v3.1.1 - os.pdf

// Find out:
// 1. packet MQTT can be the same for both MQTT-3.1.1 and MQTT-5.0
//    or compatible part can be defined separately

#include <expected> // C++ 23
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

// A UTF-8 encoded string MUST NOT include an encoding of the null character U + 0000. If a receiver 195 (Server or Client) receives a Control Packet containing U + 0000 it MUST close the Network 196 Connection[MQTT - 1.5.3 - 2].

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

enum class tError
{
	None,
	LengthTooShort,
	LengthTooLong,
	LengthNotAll, // There are not all of the bytes of the Length in the array.
	LengthOverflow,
};

// Where a flag bit is marked as “Reserved” in Table 2.2 - 243 Flag Bits, it is reserved for future use and MUST be set to the value listed in that table[MQTT - 2.2.2 - 1].If 244 invalid flags are received, the receiver MUST close the Network Connection[MQTT - 2.2.2 - 2].

union tFixedHeader
{
	struct
	{
		std::uint8_t Flags : 4;
		std::uint8_t ControlPacketType : 4;
	}Field;
	std::uint8_t Value = 0;
};

constexpr tFixedHeader MakeFixedHeader(tControlPacketType type, std::uint8_t flags = 0)
{
	tFixedHeader Header{};
	Header.Field.ControlPacketType = static_cast<std::uint8_t>(type);
	Header.Field.Flags = flags;
	return Header;
}

constexpr tFixedHeader MakeCONNECT()
{
	return MakeFixedHeader(tControlPacketType::CONNECT);
}

constexpr tFixedHeader MakeCONNACK()
{
	return MakeFixedHeader(tControlPacketType::CONNACK);
}

constexpr tFixedHeader MakePUBLISH(bool dup, std::uint8_t qos, bool retain)
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

constexpr tFixedHeader MakeSUBSCRIBE()
{
	return MakeFixedHeader(tControlPacketType::SUBSCRIBE, 0x02);
}

using tRemainingLengthParseExp = std::expected<std::uint32_t, tError>;
using tRemainingLengthToVectorExp = std::expected<std::vector<std::uint8_t>, tError>;

class tRemainingLength
{
	static constexpr std::size_t m_SizeMax = 4;

	union tLengthPart
	{
		struct
		{
			std::uint8_t Num : 7;
			std::uint8_t Continuation : 1;
		}Field;
		std::uint8_t Value = 0;
	};

	tRemainingLength() = delete;

public:
	static tRemainingLengthParseExp Parse(const std::vector<std::uint8_t>& data);
	static tRemainingLengthToVectorExp ToVector(std::uint32_t value);
};


//class tPacketParser
//{
//	std::queue<std::uint8_t> m_Queue;
//
//public:
//	tPacketParser() = default;
//
//	void push_back(std::vector<std::uint8_t>& data) {}
//
//	virtual tPacket OnReceived() = 0;
//};

// **** CONNECT

// The variable header for the CONNECT Packet consists of four fields in the following order: Protocol Name, Protocol Level, Connect Flags, and Keep Alive.

//#pragma pack(push, 1)
struct tVariableHeaderCONNECT
{
	static constexpr std::uint8_t ProtocolName[6] = { 0,4,'M','Q','T','T' }; // The string, its offset and length will not be changed by future versions of the MQTT specification.
	// [#] MQTT 3.1.1
	static constexpr std::uint8_t ProtocolLevel = 4; // The value of the Protocol Level field for the version 3.1.1 of the protocol is 4 (0x04).

	union tConnectFlags
	{
		struct
		{
			std::uint8_t Reserved : 1; // The Server MUST validate that the reserved flag in the CONNECT Control Packet is set to zero and disconnect the Client if it is not zero [MQTT-3.1.2-3].
			std::uint8_t CleanSession : 1;
			std::uint8_t WillFlag : 1;
			std::uint8_t WillQoS : 2; // [TBD] find out how it works
			std::uint8_t WillRetain : 1;
			std::uint8_t PasswordFlag : 1;
			std::uint8_t UserNameFlag : 1;
		}Field;
		std::uint8_t Value = 0;
	}ConnectFlags;

	// The Keep Alive is a time interval measured in seconds.
	// Expressed as a 16-bit word, it is the maximum time interval that is permitted to elapse between the point at which the Client finishes
	// transmitting one Control Packet and the point it starts sending the next.
	std::uint16_t KeepAlive = 0; // The maximum value is 18 hours 12 minutes and 15 seconds.
	
	std::vector<std::uint8_t> ToVector()
	{
		std::vector<std::uint8_t> Data;
		Data.insert(Data.end(), std::begin(ProtocolName), std::end(ProtocolName));
		Data.push_back(ProtocolLevel);
		Data.push_back(ConnectFlags.Value);
		Data.push_back(static_cast<std::uint8_t>(KeepAlive << 8));
		Data.push_back(static_cast<std::uint8_t>(KeepAlive));
		return Data;
	}
};
//#pragma pack(pop)

struct tPayloadCONNECT // The payload of the CONNECT Packet contains one or more length-prefixed fields, whose presence is determined by the flags in the variable header.
{
	// The Client Identifier (ClientId) identifies the Client to the Server. 
	// The Server MUST allow ClientIds which are between 1 and 23 UTF - 8 encoded bytes in length, and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".
	std::string ClientId;

	// These fields, if present, MUST appear in the order Client Identifier, Will Topic, Will Message, User Name, Password
	std::string WillTopic;
	std::string WillMessage;

	std::string UserName;
	std::string Password;
	
	//std::vector<std::uint8_t> ToVector();
};

//------------------
// Some types of MQTT Control Packets contain a variable header component. It resides between the fixed header and the payload.
// 
// 3.1 CONNECT – Client requests a connection to a Server, p.23
template <class VH, class PL>
class tPacket
{
	tFixedHeader m_FixedHeader{};

protected:
	VH m_VariableHeader; // [TBD] may be std::optional or std::variable
	PL m_Payload; // [TBD] may be std::optional or std::variable

protected:
	tPacket(tFixedHeader fixedHeader)
		:m_FixedHeader(fixedHeader)
	{
	}
	virtual ~tPacket() {}
};

class tPacketCONNECT : public tPacket<tVariableHeaderCONNECT, tPayloadCONNECT>
{
public:
	tPacketCONNECT() :tPacket(MakeCONNECT()) {}
	tPacketCONNECT(const std::string& clientId, const std::string& willTopic, const std::string& willMessage, const std::string& userName, const std::string& password)
		:tPacket(MakeCONNECT())
	{
		m_VariableHeader.ConnectFlags.Field.WillQoS = 1; // [TBD] TEST
		m_VariableHeader.ConnectFlags.Field.CleanSession = 1; // [TBD] TEST
		m_VariableHeader.KeepAlive = 10; // [TBD] TEST

		SetClientId(clientId);
		SetWill(willTopic, willMessage);
		SetUser(userName, password);
	}

	void SetClientId(const std::string& value)
	{
		// The Server MAY allow ClientId’s that contain more than 23 encoded bytes.
		// The Server MAY allow ClientId’s that contain characters not included in the list given above.
		// 
		// A Server MAY allow a Client to supply a ClientId that has a length of zero bytes,
		// however if it does so the Server MUST treat this as a special case and assign a unique ClientId to that Client.
		// It MUST then process the CONNECT packet as if the Client had provided that unique ClientId [MQTT - 3.1.3 - 6].
		// 
		// If the Client supplies a zero - byte ClientId, the Client MUST also set CleanSession to 1 [MQTT - 3.1.3 - 7].
		m_Payload.ClientId = value;
	}

	void SetWill(const std::string& topic, const std::string& message)
	{
		m_VariableHeader.ConnectFlags.Field.WillFlag = !topic.empty() && !message.empty();
		m_Payload.WillTopic = topic;
		m_Payload.WillMessage = message;

		if (!m_VariableHeader.ConnectFlags.Field.WillFlag)
			m_VariableHeader.ConnectFlags.Field.WillQoS = 0; // If the Will Flag is set to 0, then the Will QoS MUST be set to 0 (0x00) [MQTT-3.1.2-13].
	}

	void SetUser(const std::string& name, const std::string& password)
	{
		m_VariableHeader.ConnectFlags.Field.UserNameFlag = !name.empty();
		m_VariableHeader.ConnectFlags.Field.PasswordFlag = !name.empty() && !password.empty(); // [TBD] verify it (write here reference to the doc.)
		m_Payload.UserName = name;
		if (m_VariableHeader.ConnectFlags.Field.PasswordFlag)
			m_Payload.Password = password;
	}

	std::vector<std::uint8_t> ToVector()
	{
		return m_VariableHeader.ToVector();
		//std::vector<std::uint8_t> Data;
		//auto VariableHeaderBegin = reinterpret_cast<std::uint8_t*>(&m_VariableHeader);
		//auto VariableHeaderEnd = VariableHeaderBegin + sizeof(m_VariableHeader);
		//Data.insert(Data.end(), VariableHeaderBegin, VariableHeaderEnd);
		////Data.insert(Data.end(), std::begin(ProtocolName), std::end(ProtocolName));
		//return Data;
	}
};

}
}
