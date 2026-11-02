///////////////////////////////////////////////////////////////////////////////////////////////////
// tGsmCinterionBGS2R1_PacketAT
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 02 13  |
// |      2     |   2017 05 16  | Corrected FindPacket in order to catch "NO CARRIER"
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_AT_H
#define LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_AT_H

#include <libConfig.h>

#include <utilsConverter.h>

#include <string>
#include <vector>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketAT
		{

const int PacketSizeMax = 1023;

std::vector<char> FindPacket(std::vector<char>& receivedData);

std::vector<char> MakePacket(std::vector<char>& payload);

bool CompareMessage(std::vector<char> payload, char* constMsg);
///////////////////////////////////////////////////////////////////////////////////////////////////
class tMsg
{
public:
	virtual ~tMsg() { }

protected:
	std::vector<char>::iterator Find(std::vector<char>::iterator dataBegin, std::vector<char>::iterator dataEnd, char symbol);

	int GetFieldsQty(std::vector<char>& payload);

	//bool ParseDate(std::vector<char>& dataField, char& year, char& month, char& day);
	//bool ParseTime(std::vector<char>& dataField, char& hour, char& minute, char& second);
	//bool ParseDouble(std::vector<char>& dataField, double& value);
	//bool ParseLatitude(std::vector<char>& dataField, double& value);
	//bool ParseLongitude(std::vector<char>& dataField, double& value);

	//	unsigned char GetGNSS()
	//	{
	//		switch (*(message + 2))
	//		{
	//		case 'P': return 1; // GPS
	//		case 'L': return 2;	// GLONASS
	//		case 'N': return 3; // mixed
	//		}
	//		return 0;
	//	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class tMsgCOPS : public tMsg
{
	char m_Mode;
	char m_Format;
	std::string m_OperatorName;

public:
	tMsgCOPS();

private:
	tMsgCOPS(std::vector<char>& payload);

public:
	static bool TryParse(std::vector<char>& payload, tMsgCOPS& message);

	char GetMode() { return m_Mode; }
	char GetFormat() { return m_Format; }
	std::string GetOperatorName() { return m_OperatorName; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class tMsgCREG : public tMsg
{
	char m_RegStatus;

public:
	tMsgCREG();

private:
	tMsgCREG(std::vector<char>& payload);

public:
	static bool TryParse(std::vector<char>& payload, tMsgCREG& message);

	char GetRegStatus() { return m_RegStatus; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class tMsgCSQ : public tMsg
{
	char m_RSSI;
	char m_BER;

public:
	tMsgCSQ();

private:
	tMsgCSQ(std::vector<char>& payload);

public:
	static bool TryParse(std::vector<char>& payload, tMsgCSQ& message);

	char GetRSSI() { return m_RSSI; }
	char GetBER() { return m_BER; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
struct tMsgSMONC_BS
{
	unsigned short MCC;
	unsigned short MNC;
	unsigned short LAC;
	unsigned short Cell;
	unsigned char BSIC;
	unsigned short chann;
	unsigned char RSSI;
	unsigned char C1;
	unsigned char C2;
};

class tMsgSMONC : public tMsg
{
	std::vector<tMsgSMONC_BS> m_BaseStations;

public:
	tMsgSMONC();

private:
	tMsgSMONC(std::vector<char>& payload);

public:
	static bool TryParse(std::vector<char>& payload, tMsgSMONC& message);

	std::vector<tMsgSMONC_BS> GetBaseStations() { return m_BaseStations; }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}

#endif//LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_AT_H
