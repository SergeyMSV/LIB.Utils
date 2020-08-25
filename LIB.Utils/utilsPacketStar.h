///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketStar.h
//
//[STX='*' 1-Byte][PayloadSize 2-Bytes LittleEndian][Payload up to 1024-Bytes][CRC16 CCITT 2-Bytes (PayloadSize and Payload, except STX) LittleEndian]
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2015 07 17  |
// |     ...    |               | 
// |     16     |   2019 08 20  |
// |     17     |   2020 05 08  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsCRC.h"
#include "utilsPacket.h"

namespace utils
{
	namespace packet_Star
	{

template <class TPayload>
struct tFormatStar
{
	typedef unsigned short tFieldDataSize;

	enum : unsigned char { STX = '*' };//C++11

protected:
	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		std::size_t Size = std::distance(cbegin, cend);

		if (Size >= GetSize(0) && *cbegin == STX)
		{
			tFieldDataSize DataSize = 0;

			const tVectorUInt8::const_iterator Begin = cbegin + 1;
			const tVectorUInt8::const_iterator End = Begin + sizeof(tFieldDataSize);

			std::copy(Begin, End, reinterpret_cast<std::uint8_t*>(&DataSize));

			if (Size >= GetSize(DataSize) && VerifyCRC(Begin, sizeof(tFieldDataSize) + DataSize))
			{
				return tVectorUInt8(cbegin, cbegin + GetSize(DataSize));
			}
		}

		return tVectorUInt8();
	}

	static bool TryParse(const tVectorUInt8& packetVector, TPayload& payload)
	{
		if (packetVector.size() >= GetSize(0) && packetVector[0] == STX)
		{
			tFieldDataSize DataSize = 0;

			tVectorUInt8::const_iterator Begin = packetVector.cbegin() + 1;
			tVectorUInt8::const_iterator End = Begin + sizeof(tFieldDataSize);

			std::copy(Begin, End, reinterpret_cast<std::uint8_t*>(&DataSize));

			if (packetVector.size() == GetSize(DataSize) && VerifyCRC(Begin, sizeof(tFieldDataSize) + DataSize))
			{
				Begin = End;
				End += DataSize;

				payload = TPayload(Begin, End);

				return true;
			}
		}

		return false;
	}

	static std::size_t GetSize(std::size_t payloadSize) { return sizeof(STX) + sizeof(tFieldDataSize) + payloadSize + 2; };

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.reserve(GetSize(payload.size()));

		dst.push_back(STX);

		utils::Append(dst, static_cast<unsigned short>(payload.size()));

		for (auto i : payload)
		{
			dst.push_back(i);
		}

		const std::size_t DataSize = sizeof(tFieldDataSize) + payload.size();

		const std::uint16_t CRC = utils::crc::CRC16_CCITT(dst.end() - DataSize, dst.end());

		utils::Append(dst, CRC);
	}

private:
	static bool VerifyCRC(tVectorUInt8::const_iterator begin, std::size_t crcDataSize)
	{
		const std::uint16_t CRC = utils::crc::CRC16_CCITT(begin, begin + crcDataSize);

		const tVectorUInt8::const_iterator CRCBegin = begin + crcDataSize;

		const std::uint16_t CRCReceived = utils::Read<std::uint16_t>(CRCBegin, CRCBegin + sizeof(CRC));

		return CRC == CRCReceived;
	}
};

	}
}
