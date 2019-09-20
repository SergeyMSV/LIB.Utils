///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsPacketBNTBX2.h
//
// Standard ISO/IEC 114882, C++98
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 06 20  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsCRC.h"
#include "utilsPacket.h"

namespace utils
{
	namespace packet_BNTBX
	{

template <class TPayload>
struct tFormatBNTBX2
{
	static const unsigned char STX = 0x5F;

	typedef unsigned int tDeviceID;

	enum tMsgID
	{
		tMsgID_00 = 0x00,
		tMsgID_08 = 0x08,
		tMsgID_09,
	};

	typedef unsigned char tMsgVER;

private:
	tDeviceID DeviceID;
	tMsgID MsgID;
	tMsgVER MsgVER;

public:
	tFormatBNTBX2()
	{
		DeviceID = 0;
		MsgID = tMsgID_00;
		MsgVER = 0;
	}

	tMsgID GetMsgID() { return MsgID; }
	tMsgVER GetMsgVER() { return MsgVER; }

protected:
	template <class tMsg>
	void SetPayloadIDs(const tMsg& msg)
	{
		MsgID = static_cast<tMsgID>(tMsg::ID);
		MsgVER = static_cast<tMsgVER>(tMsg::VER);
	}

	static tVectorUInt8 TestPacket(tVectorUInt8::const_iterator cbegin, tVectorUInt8::const_iterator cend)
	{
		size_t Size = std::distance(cbegin, cend);

		if (Size >= GetSize(0) && *cbegin == STX)
		{
			unsigned short PayloadSize = 0;

			tVectorUInt8::const_iterator CBegin = cbegin + 7;
			tVectorUInt8::const_iterator CEnd = CBegin + sizeof(PayloadSize);

			std::copy(CBegin, CEnd, (unsigned char*)& PayloadSize);

			
			if (Size >= GetSize(PayloadSize))
			{
				unsigned short CRC = 0;

				tVectorUInt8::const_iterator CBegin_CRC = CEnd + PayloadSize;

				std::copy(CBegin_CRC, CBegin_CRC + sizeof(CRC), reinterpret_cast<tUInt8*>(&CRC));

				if (CRC == utils::crc::CRC16_BNTBX2(cbegin + 1, CBegin_CRC))
				{
					return tVectorUInt8(cbegin, cbegin + GetSize(PayloadSize));
				}
			}
		}

		return tVectorUInt8();
	}

	static bool TryParse(const tVectorUInt8& packetVector, tFormatBNTBX2& format, TPayload& payload)
	{
		if (packetVector.size() >= GetSize(0) && packetVector[0] == STX)
		{
			tVectorUInt8::const_iterator CBegin = packetVector.cbegin() + 1;
			tVectorUInt8::const_iterator CEnd = CBegin + sizeof(format.DeviceID);

			std::copy(CBegin, CEnd, reinterpret_cast<tUInt8*>(&format.DeviceID));

			CBegin = CEnd;
			CEnd += 1;

			format.MsgID = static_cast<tMsgID>(*CBegin);

			CBegin = CEnd;
			CEnd += 1;

			format.MsgVER = static_cast<tMsgVER>(*CBegin);

			unsigned short PayloadSize = 0;

			CBegin = CEnd;
			CEnd += sizeof(PayloadSize);

			std::copy(CBegin, CEnd, reinterpret_cast<tUInt8*>(&PayloadSize));

			if (packetVector.size() == GetSize(PayloadSize))
			{
				unsigned short CRC = 0;

				tVectorUInt8::const_iterator CBegin_CRC = CEnd + PayloadSize;

				std::copy(CBegin_CRC, CBegin_CRC + sizeof(CRC), reinterpret_cast<tUInt8*>(&CRC));

				if (CRC == utils::crc::CRC16_BNTBX2(packetVector.cbegin() + 1, CBegin_CRC))
				{
					CBegin = CEnd;
					CEnd += PayloadSize;

					payload = TPayload(CBegin, CEnd);

					return true;
				}
			}
		}

		return false;
	}

	static size_t GetSize(size_t payloadSize) { return 11 + payloadSize; };

	void Append(tVectorUInt8& dst, const TPayload& payload) const
	{
		dst.push_back(STX);

		utils::Append(dst, DeviceID);

		dst.push_back(static_cast<tUInt8>(MsgID));
		dst.push_back(static_cast<tUInt8>(MsgVER));

		utils::Append(dst, static_cast<unsigned short>(payload.GetSize()));

		payload.Append(dst);

		unsigned short CRC = utils::crc::CRC16_BNTBX2(dst.cbegin() + 1, dst.cend());

		utils::Append(dst, CRC);
	}
};

	}
}