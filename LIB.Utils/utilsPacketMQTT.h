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
#include <optional>
//#include <queue>
#include <string>
//#include <utility>
#include <vector>

#include <cstdint>

//#include <iostream> // TEST

namespace utils
{
namespace packet_MQTT
{

// MQTT 3.1.1
constexpr char DefaultProtocolName[] = "MQTT"; // The string, its offset and length will not be changed by future versions of the MQTT specification.
constexpr std::uint8_t DefaultProtocolLevel = 4; // The value of the Protocol Level field for the version 3.1.1 of the protocol is 4 (0x04).

// MQTT 3.1
//constexpr char DefaultProtocolName[] = "MQIsdp";
//constexpr std::uint8_t DefaultProtocolLevel = 3;


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
	PacketTooShort,
	PacketType,
	VariableHeaderTooShort,
	PayloadTooShort,
	//Format,
	ProtocolName,
	ProtocolLevel,
};

// 3.3 PUBLISH – Publish message - 3.3.1 Fixed header [VERIFY]
/*enum class tQoS : std::uint8_t // [!] it might be only for publish
{
	AtMostOnceDelivery,
	AtLeastOnceDelivery,
	ExactlyOnceDelivery,
	Reserved_MustNotBeUsed
};*/

#pragma pack(push, 1)
union tUInt16
{
	struct
	{
		std::uint16_t LSB : 8;
		std::uint16_t MSB : 8;
	}Field;
	std::uint16_t Value = 0;

	tUInt16() = default;
	tUInt16(std::uint16_t value) :Value(value) {} // not explicit

	tUInt16& operator=(std::uint16_t value)
	{
		Value = value;
		return *this;
	}
};
#pragma pack(pop)

class tString : public std::string
{
public:
	tString() = default;
	tString(const std::string& value) : std::string(value) {} // not explicit
	tString(std::string&& value) : std::string(value) {} // not explicit

	std::size_t GetSize() const { return size() + 2; }

	static std::expected<std::string, tError> Parse(const std::vector<std::uint8_t>& data, std::size_t& offset);

	std::vector<std::uint8_t> ToVector() const;
};

union tFixedHeader
{
	struct
	{
		std::uint8_t Flags : 4;
		std::uint8_t ControlPacketType : 4;
	}Field;
	std::uint8_t Value = 0;

	tFixedHeader() = default;
	tFixedHeader(std::uint8_t value) :Value(value) {} // not explicit

	tControlPacketType GetControlPacketType() const { return static_cast<tControlPacketType>(Field.ControlPacketType); }

	bool operator==(const tFixedHeader& val) const { return Value == val.Value; }
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
	static tRemainingLengthParseExp Parse(const std::vector<std::uint8_t>& data, std::size_t& offset);
	static tRemainingLengthToVectorExp ToVector(std::uint32_t value);
};

std::expected<tControlPacketType, tError> TestPacket(const std::vector<std::uint8_t>& data);

template <class VH, class PL>
class tPacket
{
protected:
	tFixedHeader m_FixedHeader{};

	std::optional<VH> m_VariableHeader;
	std::optional<PL> m_Payload;

private:
	tPacket() = default;

protected:
	explicit tPacket(tFixedHeader fixedHeader) :m_FixedHeader(fixedHeader) {}

public:
	virtual ~tPacket() {}

	std::optional<VH> GetVariableHeader() { return m_VariableHeader; }
	std::optional<PL> GetPayload() { return m_Payload; }

	static std::expected<tPacket, tError> Parse(const std::vector<std::uint8_t>& data)
	{
		if (data.empty())
			return std::unexpected(tError::PacketTooShort);

		tFixedHeader FixHeader = data[0];
		// [TBD] Verify Received packet Type !!!
		//if (static_cast<tControlPacketType>(FixHeader.Field.ControlPacketType) != tControlPacketType::CONNECT) // specific
		//	return std::unexpected(tError::PacketType);

		tPacket Pack{};
		Pack.m_FixedHeader = FixHeader;

		std::size_t DataOffset = 1; // data[0]

		auto RLengtExp = tRemainingLength::Parse(data, DataOffset);
		if (!RLengtExp.has_value())
			return std::unexpected(RLengtExp.error());
		if (*RLengtExp > data.size() - DataOffset)
			return std::unexpected(tError::PacketTooShort);
		/// |||| - the same in TestPacket

		auto VarHeadExp = VH::Parse(data, DataOffset);
		if (!VarHeadExp.has_value())
			return std::unexpected(VarHeadExp.error());
		Pack.m_VariableHeader = *VarHeadExp;

		auto PayloadExp = PL::Parse(*Pack.m_VariableHeader, data, DataOffset);
		if (!PayloadExp.has_value())
			return std::unexpected(PayloadExp.error());
		Pack.m_Payload = *PayloadExp;

		return Pack;
	}

	std::vector<std::uint8_t> ToVector() const
	{
		std::vector<std::uint8_t> Data;
		if (m_VariableHeader.has_value())
			Data.append_range(m_VariableHeader->ToVector());
		if (m_Payload.has_value())
			Data.append_range(m_Payload->ToVector());
		auto RemainingLength = tRemainingLength::ToVector(static_cast<std::uint32_t>(Data.size()));
		if (!RemainingLength.has_value())
			return {};
		Data.insert_range(Data.begin(), *RemainingLength);
		Data.insert(Data.begin(), m_FixedHeader.Value);
		return Data;
	}

	bool operator==(const tPacket& val) const
	{
		return m_FixedHeader == val.m_FixedHeader && m_VariableHeader == val.m_VariableHeader && m_Payload == val.m_Payload;
	}
};

template <class VH>
struct tPayloadEmpty
{
	static std::expected<tPayloadEmpty, tError> Parse(const VH& variableHeader, const std::vector<std::uint8_t>& data, std::size_t& offset) { return tPayloadEmpty(); }

	std::vector<std::uint8_t> ToVector() const { return {}; }

	bool operator==(const tPayloadEmpty& value) const = default;
};


// **** CONNECT

// The variable header for the CONNECT Packet consists of four fields in the following order: Protocol Name, Protocol Level, Connect Flags, and Keep Alive.



struct tVariableHeaderCONNECT
{
	tString ProtocolName; // The string, its offset and length will not be changed by future versions of the MQTT specification.
	std::uint8_t ProtocolLevel; // The value of the Protocol Level field for the version 3.1.1 of the protocol is 4 (0x04).

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
	tUInt16 KeepAlive = 0; // The maximum value is 18 hours 12 minutes and 15 seconds.
	
	tVariableHeaderCONNECT() :ProtocolName(DefaultProtocolName), ProtocolLevel(DefaultProtocolLevel) {}

	static std::expected<tVariableHeaderCONNECT, tError> Parse(const std::vector<std::uint8_t>& data, std::size_t& offset);

	std::size_t GetSize() const { return ProtocolName.GetSize() + 4; }

	std::vector<std::uint8_t> ToVector() const;

	bool operator==(const tVariableHeaderCONNECT& val) const;
};

struct tPayloadCONNECT // The payload of the CONNECT Packet contains one or more length-prefixed fields, whose presence is determined by the flags in the variable header.
{
	// The Client Identifier (ClientId) identifies the Client to the Server. 
	// The Server MUST allow ClientIds which are between 1 and 23 UTF - 8 encoded bytes in length, and that contain only the characters "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".
	tString ClientId;

	// These fields, if present, MUST appear in the order Client Identifier, Will Topic, Will Message, User Name, Password
	std::optional<tString> WillTopic;
	std::optional<tString> WillMessage;

	std::optional<tString> UserName;
	std::optional<tString> Password;

	static std::expected<tPayloadCONNECT, tError> Parse(const tVariableHeaderCONNECT& variableHeader, const std::vector<std::uint8_t>& data, std::size_t& offset);

	std::vector<std::uint8_t> ToVector() const;

	bool operator==(const tPayloadCONNECT& value) const = default;
};

class tPacketCONNECT : public tPacket<tVariableHeaderCONNECT, tPayloadCONNECT>
{
public:
	tPacketCONNECT() :tPacket(MakeCONNECT()) {}
	tPacketCONNECT(const std::string& clientId, const std::string& willTopic, const std::string& willMessage, const std::string& userName, const std::string& password)
		:tPacket(MakeCONNECT())
	{
		m_VariableHeader = tVariableHeaderCONNECT{};
		m_VariableHeader->ConnectFlags.Field.WillQoS = 1; // [TBD] TEST
		m_VariableHeader->ConnectFlags.Field.CleanSession = 1; // [TBD] TEST
		m_VariableHeader-> KeepAlive.Value = 11; // [TBD] TEST

		m_Payload = tPayloadCONNECT{};

		SetClientId(clientId);
		SetWill(willTopic, willMessage);
		SetUser(userName, password);
	}

	void SetClientId(const std::string& value);
	void SetWill(const std::string& topic, const std::string& message);
	void SetUser(const std::string& name, const std::string& password);
};


enum class tConnectReturnCode : std::uint8_t
{
	ConnectionAccepted,
	ConnectionRefused_UnacceptableProtocolVersion, // The Server does not support the level of the MQTT protocol requested by the Client.
	ConnectionRefused_IdentifierRejected, // The Client identifier is correct UTF-8 but not allowed by the Server.
	ConnectionRefused_ServerUnavailable, // The Network Connection has been made but the MQTT service is unavailable.
	ConnectionRefused_BadUserNameOrPassword, // The data in the user name or password is malformed.
	ConnectionRefused_NotAuthorized, // The Client is not authorized to connect.
	Reserved // 6-255 Reserved for future use.
};

struct tVariableHeaderCONNACK
{
	union tConnectAcknowledgeFlags
	{
		struct
		{
			std::uint8_t SP : 1; // It is the Session Present Flag.
			std::uint8_t Reserved : 7; // Bits 7-1 are reserved and MUST be set to 0.
		}Field;
		std::uint8_t Value = 0;
	}ConnectAcknowledgeFlags;
	tConnectReturnCode ConnectReturnCode = tConnectReturnCode::Reserved;

	tVariableHeaderCONNACK() = default;

	static std::expected<tVariableHeaderCONNACK, tError> Parse(const std::vector<std::uint8_t>& data, std::size_t& offset);

	static std::size_t GetSize() { return 2; }

	std::vector<std::uint8_t> ToVector() const;

	bool operator==(const tVariableHeaderCONNACK& val) const;
};

using tPayloadCONNACK = tPayloadEmpty<tVariableHeaderCONNACK>;

class tPacketCONNACK : public tPacket<tVariableHeaderCONNACK, tPayloadCONNACK>
{
public:
	tPacketCONNACK() :tPacket(MakeCONNACK()) {}
	tPacketCONNACK(bool sessionPresent, tConnectReturnCode connRetCode)
		:tPacket(MakeCONNACK())
	{
		m_VariableHeader = tVariableHeaderCONNACK{};
		m_VariableHeader->ConnectAcknowledgeFlags.Field.SP = sessionPresent;
		m_VariableHeader->ConnectReturnCode = connRetCode;

		m_Payload = tPayloadCONNACK{};
	}
};

}
}
