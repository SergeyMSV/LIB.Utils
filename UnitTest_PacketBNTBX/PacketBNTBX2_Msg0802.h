///////////////////////////////////////////////////////////////////////////////////////////////////
// PacketBNTBX2_Msg0802
//
// Standard ISO/IEC 114882, C++17
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 08 20  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>

#include <exception>
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tMsgBNTBX0802Cmd
{
	enum { ID = 0x08, VER = 0x02 };

	unsigned char CheckID;

	explicit tMsgBNTBX0802Cmd(unsigned char checkID)
	{
		CheckID = checkID;
	}

	explicit tMsgBNTBX0802Cmd(const tVectorUInt8 payload)
	{
		if (payload.size() == 1)
		{
			CheckID = payload[0];
		}
		else
		{
			throw std::exception("tMsgBNTBX0802Cmd");//[TBD] Invalid argument
		}
	}

	static size_t GetSize() { return 1; };

	tVectorUInt8 ToVector() const
	{
		tVectorUInt8 MsgVector;

		MsgVector.push_back(CheckID);

		return MsgVector;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tMsgBNTBX0802Rsp
{
	enum { ID = 0x08, VER = 0x02 };

	unsigned char CERR;

	explicit tMsgBNTBX0802Rsp(unsigned char cerr)
	{
		CERR = cerr;
	}

	explicit tMsgBNTBX0802Rsp(const tVectorUInt8 payload)
	{
		if (payload.size() == 1)
		{
			CERR = payload[0];
		}
		else
		{
			throw std::exception("tMsgBNTBX0802Rsp");//[TBD] Invalid argument
		}
	}

	static size_t GetSize() { return 1; };

	tVectorUInt8 ToVector() const
	{
		tVectorUInt8 MsgVector;

		MsgVector.push_back(CERR);

		return MsgVector;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////