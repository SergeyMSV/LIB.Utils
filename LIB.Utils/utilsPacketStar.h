///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketStar.h
//
//[STX='*' 1-Byte][PayloadSize 2-Bytes LittleEndian][Payload up to 1024-Bytes][CRC16 CCITT 2-Bytes (PayloadSize and Payload, except STX) LittleEndian]
// 
//tPayloadCommon
//[STX='*' 1-Byte][PayloadSize 2-Bytes LittleEndian][MsgId 1-Byte][Payload up to 1023-Bytes][CRC16 CCITT 2-Bytes (PayloadSize and Payload, except STX) LittleEndian]
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2015 07 17  |
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
	typedef uint16_t tFieldDataSize;

	enum : uint8_t { STX = '*' };

protected:
	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		const size_t Size = std::distance(cbegin, cend);

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

		return {};
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

	static size_t GetSize(size_t payloadSize) { return sizeof(STX) + sizeof(tFieldDataSize) + payloadSize + 2; }//2 - for CRC

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.reserve(GetSize(payload.size()));

		dst.push_back(STX);

		utils::Append(dst, static_cast<uint16_t>(payload.size()));

		for (const auto i : payload)
		{
			dst.push_back(i);
		}

		const size_t DataSize = sizeof(tFieldDataSize) + payload.size();

		const uint16_t CRC = utils::crc::CRC16_CCITT(dst.end() - DataSize, dst.end());

		utils::Append(dst, CRC);
	}

private:
	static bool VerifyCRC(tVectorUInt8::const_iterator begin, size_t crcDataSize)
	{
		const uint16_t CRC = utils::crc::CRC16_CCITT(begin, begin + crcDataSize);

		const tVectorUInt8::const_iterator CRCBegin = begin + crcDataSize;

		const uint16_t CRCReceived = utils::Read<uint16_t>(CRCBegin, CRCBegin + sizeof(CRC));

		return CRC == CRCReceived;
	}
};

//[MsgId 1-Byte][Payload up to 1023-Bytes]
struct tPayloadCommon
{
	struct tData
	{
		uint8_t MsgId = 0;
		tVectorUInt8 Payload;

		tData() = default;
		tData(uint8_t msgId, const tVectorUInt8& payload)
			:MsgId(msgId), Payload(payload)
		{}
		tData(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		{
			if (cbegin == cend)
				return;

			MsgId = *cbegin;

			if (++cbegin == cend)
				return;

			Payload = tVectorUInt8(cbegin, cend);
		}

		size_t size() const
		{
			return sizeof(MsgId) + Payload.size();
		}

		uint8_t operator[] (const size_t index) const
		{
			if (index >= size())
				return 0;

			if (index == 0)
				return MsgId;

			return Payload[index - 1];
		}

		bool operator == (const tData& val) const
		{
			return
				MsgId == val.MsgId &&
				Payload == val.Payload;
		}
		bool operator != (const tData& val) const
		{
			return !(*this == val);
		}
	};

	typedef tData value_type;

	class tIterator
	{
		friend struct tPayloadCommon;

		const tPayloadCommon* m_pObj = nullptr;

		const size_t m_DataSize = 0;
		size_t m_DataIndex = 0;

		tIterator() = delete;
		tIterator(const tPayloadCommon* obj, bool begin)
			:m_pObj(obj), m_DataSize(m_pObj->size())
		{
			if (m_DataSize > 0)
			{
				if (begin)
				{
					m_DataIndex = 0;
				}
				else
				{
					m_DataIndex = m_DataSize;
				}
			}
		}

	public:
		tIterator& operator ++ ()
		{
			if (m_DataIndex < m_DataSize)
			{
				++m_DataIndex;
			}

			return *this;
		}

		bool operator != (const tIterator& val) const
		{
			return m_DataIndex != val.m_DataIndex;
		}

		const uint8_t operator * () const
		{
			return m_pObj->Data[m_DataIndex];
		}
	};

	typedef tIterator iterator;

	value_type Data{};

	tPayloadCommon() = default;

	explicit tPayloadCommon(const value_type& data)
		:Data(data)
	{}

	tPayloadCommon(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
		:Data(cbegin, cend)
	{}

	size_t size() const
	{
		return Data.size();
	}

	iterator begin() const
	{
		return iterator(this, true);
	}

	iterator end() const
	{
		return iterator(this, false);
	}
};

	}
}
