///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketStar2.h
//
//[STX='*' 1-Byte][PayloadSize 2-Bytes LittleEndian][Payload up to 1024-Bytes][CRC16 CCITT 2-Bytes (PayloadSize and Payload, except STX) LittleEndian]
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 04 15  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsPacketStar.h"

namespace utils
{
	namespace packet_Star
	{

template <class TPayload>
struct tFormatStar2 : public tFormatStar<TPayload>
{
	typedef unsigned char tMsgID;

private:
	tMsgID MsgID;

public:
	tFormatStar2()
	{
		MsgID = 0;
	}

	tMsgID GetMsgID() { return MsgID; }

protected:
	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		std::size_t Size = std::distance(cbegin, cend);

		if (Size >= GetSize(0) && *cbegin == tFormatStar2::STX)
		{
			unsigned short DataSize = 0;

			tVectorUInt8::const_iterator CBegin = cbegin + 1;
			tVectorUInt8::const_iterator CEnd = CBegin + sizeof(DataSize);

			std::copy(CBegin, CEnd, reinterpret_cast<tUInt8*>(&DataSize));

			if (Size >= GetSize(DataSize - 1))
			{
				//Get CRC
				//if (CheckCRC(packetVector))

				return tVectorUInt8(cbegin, cbegin + GetSize(DataSize - 1));
			}
		}

		return tVectorUInt8();
	}

	static bool TryParse(const tVectorUInt8& packetVector, /*tFormatStar2& format,*/ TPayload& payload)
	{
		if (packetVector.size() >= GetSize(0) && packetVector[0] == tFormatStar2::STX)
		{
			unsigned short DataSize = 0;

			tVectorUInt8::const_iterator CBegin = packetVector.cbegin() + 1;
			tVectorUInt8::const_iterator CEnd = CBegin + sizeof(DataSize);

			std::copy(CBegin, CEnd, reinterpret_cast<tUInt8*>(&DataSize));

			if (packetVector.size() == GetSize(DataSize - 1))
			{
				//Get CRC
				//if (CheckCRC(packetVector))

				CBegin = CEnd;
				CEnd += 1;

				format.MsgID = *CBegin;

				CBegin = CEnd;
				CEnd += DataSize - 1;//-MsgID

				payload = TPayload(CBegin, CEnd);

				return true;
			}
		}

		return false;
	}

	static std::size_t GetSize(std::size_t payloadSize) { return tFormatStar<TPayload>::GetSize(payloadSize) + 1; };

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		unsigned short CRC = 0x0304;//[TBD]

		dst.push_back(tFormatStar2::STX);

		utils::Append(dst, static_cast<unsigned short>(payload.GetSize() + 1));
		utils::Append(dst, MsgID);

		payload.Append(dst);

		utils::Append(dst, CRC);
	}
};

	}
}
