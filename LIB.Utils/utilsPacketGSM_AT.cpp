#include "modGsmCinterionBGS2R1_PacketAT.h"

#include <string.h>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketAT
		{

std::vector<char> FindPacket(std::vector<char>& receivedData)
{
	std::vector<char> Packet;

	unsigned int ReceivedStxPos = 0;
	unsigned int ReceivedEtxPos = 0;

	while(1)
	{
		while (ReceivedStxPos + 1 < receivedData.size() && receivedData[ReceivedStxPos] != '^' && receivedData[ReceivedStxPos] != '+' &&
			(receivedData[ReceivedStxPos] != 'A' || receivedData[ReceivedStxPos + 1] != 'T') &&
			(receivedData[ReceivedStxPos] != 'O' || receivedData[ReceivedStxPos + 1] != 'K') &&
			(receivedData[ReceivedStxPos] < '0' || receivedData[ReceivedStxPos] > 'Z'))
		{
			ReceivedStxPos++;
		}

		if (ReceivedStxPos >= receivedData.size())
			break;

		if (receivedData[ReceivedStxPos] == '>')
		{
			receivedData.erase(receivedData.begin(), receivedData.begin() + ReceivedStxPos + 1);

			Packet.push_back('>');

			return Packet;
		}

		if (receivedData.size() - ReceivedStxPos > 0)
		{
			ReceivedEtxPos = ReceivedStxPos;

			while (ReceivedEtxPos < receivedData.size() && receivedData[ReceivedEtxPos] != 0x0A)//<CR><LF>(0x0D 0x0A)
			{
				ReceivedEtxPos++;
			}

			if (ReceivedEtxPos >= receivedData.size() || ReceivedStxPos >= ReceivedEtxPos)
				break;

			unsigned int PacketSize = ReceivedEtxPos - ReceivedStxPos - 1;//-<CR>

			if (PacketSize > PacketSizeMax)
			{
				ReceivedStxPos++;
				continue;
			}

			if (PacketSize <= PacketSizeMax && receivedData.size() - ReceivedStxPos >= PacketSize)
			{
				Packet = std::vector<char>(receivedData.begin() + ReceivedStxPos, receivedData.begin() + ReceivedStxPos + PacketSize);

				receivedData.erase(receivedData.begin(), receivedData.begin() + ReceivedStxPos + PacketSize);

				break;
			}
			else
			{
				ReceivedStxPos++;
			}
		}
		else
		{
			break;
		}
	}

	return Packet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char> MakePacket(std::vector<char>& payload)
{
	std::vector<char> Packet;

	Packet.push_back('A');
	Packet.push_back('T');

	Packet.insert(Packet.end(), payload.begin(), payload.end());

	Packet.push_back(0x0D);//<CR>
	Packet.push_back(0x0A);//<LF>

	return Packet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CompareMessage(std::vector<char> payload, char* constMsg)
{
	unsigned int Size = strlen(constMsg);

	if (payload.size() >= Size)
	{
		for (unsigned int i = 0; i < Size; ++i)
		{
			if (constMsg[i] != payload[i])
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char>::iterator tMsg::Find(std::vector<char>::iterator dataBegin, std::vector<char>::iterator dataEnd, char symbol)
{
	for (std::vector<char>::iterator i = dataBegin; i != dataEnd; ++i)
	{
		if (*i == symbol)
			return i;
	}

	return dataEnd;
}

int tMsg::GetFieldsQty(std::vector<char>& payload)
{
	std::vector<char>::iterator PtrBegin = payload.begin();

	for (int i = 0; i < 100; ++i)
	{
		PtrBegin = Find(PtrBegin + 1, payload.end(), ',');

		if (PtrBegin == payload.end())
		{
			return i;
		}
	}

	return -1;
}
/*
bool tMsg::ParseDate(std::vector<char>& dataField, char& year, char& month, char& day)
{
	if (dataField.size() == 6)//260216
	{
		char Data[3];

		Data[2] = 0;

		Data[0] = dataField[0];
		Data[1] = dataField[1];

		day = (char)strtoul(Data, 0, 10);

		Data[0] = dataField[2];
		Data[1] = dataField[3];

		month = (char)strtoul(Data, 0, 10);

		Data[0] = dataField[4];
		Data[1] = dataField[5];

		year = (char)strtoul(Data, 0, 10);

		return true;
	}

	return false;
}

bool tMsg::ParseTime(std::vector<char>& dataField, char& hour, char& minute, char& second)
{
	if (dataField.size() == 10)//162407.000
	{
		char Data[3];

		Data[2] = 0;

		Data[0] = dataField[0];
		Data[1] = dataField[1];

		hour = (char)strtoul(Data, 0, 10);

		Data[0] = dataField[2];
		Data[1] = dataField[3];

		minute = (char)strtoul(Data, 0, 10);

		Data[0] = dataField[4];
		Data[1] = dataField[5];

		second = (char)strtoul(Data, 0, 10);

		return true;
	}

	return false;
}

bool tMsg::ParseDouble(std::vector<char>& dataField, double& value)
{
	if (dataField.size() < 10)
	{
		char Data[10];

		memset(Data, 0x00, sizeof(Data));

		for (unsigned int i = 0; i < dataField.size(); ++i)
		{
			Data[i] = dataField[i];
		}

		value = strtod(Data, 0);

		return true;
	}

	return false;
}

bool tMsg::ParseLatitude(std::vector<char>& dataField, double& value)
{
#ifdef LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	if (dataField.size() >= LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_9)//ddmm.mmmm or ddmm.mmmmmm
#else//LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	if (dataField.size() == LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE)//ddmm.mmmm or ddmm.mmmmmm
#endif//LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	{
		char Data[3];

		Data[0] = dataField[0];
		Data[1] = dataField[1];
		Data[2] = 0;

		value = (char)strtoul(Data, 0, 10);//degrees - first 2 figures dd

		std::vector<char> DataRemains(dataField.begin() + 2, dataField.end());

		double Remains;

		ParseDouble(DataRemains, Remains);

		value += Remains / 60;

		return true;
	}

	return false;
}

bool tMsg::ParseLongitude(std::vector<char>& dataField, double& value)
{
#ifdef LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	if (dataField.size() >= LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_9 + 1)//dddmm.mmmm or dddmm.mmmmmm
#else//LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	if (dataField.size() == LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE + 1)//dddmm.mmmm or dddmm.mmmmmm
#endif//LIB_MODULE_GNSS_NMEA_PACKETS_LATITUDE_SIZE_ANY
	{
		char Data[4];

		Data[0] = dataField[0];
		Data[1] = dataField[1];
		Data[2] = dataField[2];
		Data[3] = 0;

		value = (char)strtoul(Data, 0, 10);//degrees - first 2 figures dd

		std::vector<char> DataRemains(dataField.begin() + 3, dataField.end());

		double Remains;

		ParseDouble(DataRemains, Remains);

		value += Remains / 60;

		return true;
	}

	return false;
}*/
///////////////////////////////////////////////////////////////////////////////////////////////////
tMsgCOPS::tMsgCOPS()
{
	m_Mode = -1;
	m_Format = -1;
}

tMsgCOPS::tMsgCOPS(std::vector<char>& payload)
{
	m_Mode = -1;
	m_Format = -1;

//at+cops?
//+COPS: 0
//
//OK
//at+cops?
//+COPS: 0,0,"MegaFon RUS"
//
//OK

	if (payload.size() > 7 && payload[0] == '+' && payload[1] == 'C' && payload[2] == 'O' && payload[3] == 'P' && payload[4] == 'S' && payload[5] == ':')
	{
		std::vector<char>::iterator PtrBegin = Find(payload.begin(), payload.end(), ':');
		std::vector<char>::iterator PtrEnd = PtrBegin;

		for (int i = 0; i < 3; ++i)//there are up to 3 fields in COPS
		{
			if (PtrBegin != payload.end())
			{
				PtrEnd = Find(PtrBegin + 1, payload.end(), ',');

				if (PtrEnd > PtrBegin)
				{
					std::vector<char> Field(PtrBegin + 1, PtrEnd);

					Field.push_back(0);

					switch (i)
					{
					case 0:
						m_Mode = utils::Read<char>(&Field[0], utils::tRadix_10);
						break;
					case 1:
						m_Format = utils::Read<char>(&Field[0], utils::tRadix_10);
						break;
					case 2:
						m_OperatorName.insert(0, &Field[0]);
						break;
					}
				}
			}
			else
			{
				return;
			}

			PtrBegin = Find(PtrBegin + 1, payload.end(), ',');
		}
	}
}

bool tMsgCOPS::TryParse(std::vector<char>& payload, tMsgCOPS& message)
{
	tMsgCOPS Message(payload);

	if (Message.m_Mode != -1)
	{
		message = Message;

		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tMsgCREG::tMsgCREG()
{
	m_RegStatus = -1;
}

tMsgCREG::tMsgCREG(std::vector<char>& payload)
{
	m_RegStatus = -1;

	if (payload.size() > 7 && payload[0] == '+' && payload[1] == 'C' && payload[2] == 'R' && payload[3] == 'E' && payload[4] == 'G' && payload[5] == ':')
	{
		std::vector<char>::iterator PtrBegin = Find(payload.begin(), payload.end(), ':');
		std::vector<char>::iterator PtrEnd = PtrBegin;

		for (int i = 0; i < 3; ++i)//there are up to 3 fields in COPS
		{
			if (PtrBegin != payload.end())
			{
				PtrEnd = Find(PtrBegin + 1, payload.end(), ',');

				if (PtrEnd > PtrBegin)
				{
					std::vector<char> Field(PtrBegin + 1, PtrEnd);

					Field.push_back(0);

					switch (i)
					{
					//case 0://<urcMode>
					//	m_RegStatus = (char)utils::ReadUInt(&Field[0], utils::tRadix_10);
					//	break;
					case 1://<regStatus>
						m_RegStatus = utils::Read<char>(&Field[0], utils::tRadix_10);
						break;
					//case 2:
					//	m_OperatorName.insert(m_OperatorName.end(), Field.begin(), Field.end());
					//	break;
					}
				}
			}
			else
			{
				return;
			}

			PtrBegin = Find(PtrBegin + 1, payload.end(), ',');
		}
	}
}

bool tMsgCREG::TryParse(std::vector<char>& payload, tMsgCREG& message)
{
	tMsgCREG Message(payload);

	if (Message.m_RegStatus != -1)
	{
		message = Message;

		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tMsgCSQ::tMsgCSQ()
{
	m_RSSI = -1;
	m_BER = -1;
}

tMsgCSQ::tMsgCSQ(std::vector<char>& payload)
{
	m_RSSI = -1;
	m_BER = -1;

	if (payload.size() > 8 && payload[0] == '+' && payload[1] == 'C' && payload[2] == 'S' && payload[3] == 'Q' && payload[4] == ':')
	{
		std::vector<char>::iterator PtrBegin = Find(payload.begin(), payload.end(), ':');
		std::vector<char>::iterator PtrEnd = PtrBegin;

		for (int i = 0; i < 2; ++i)//there are 2 fields in CSQ
		{
			if (PtrBegin != payload.end())
			{
				PtrEnd = Find(PtrBegin + 1, payload.end(), ',');

				if (PtrEnd > PtrBegin)
				{
					std::vector<char> Field(PtrBegin + 1, PtrEnd);

					Field.push_back(0);

					switch (i)
					{
					case 0://RSSI
						m_RSSI = utils::Read<char>(&Field[0], utils::tRadix_10);
						break;
					case 1://BER
						m_BER = utils::Read<char>(&Field[0], utils::tRadix_10);
						break;
					}
				}
			}
			else
			{
				return;
			}

			PtrBegin = Find(PtrBegin + 1, payload.end(), ',');
		}
	}
}

bool tMsgCSQ::TryParse(std::vector<char>& payload, tMsgCSQ& message)
{
	tMsgCSQ Message(payload);

	if (Message.m_RSSI != -1)
	{
		message = Message;

		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
tMsgSMONC::tMsgSMONC()
{

}

tMsgSMONC::tMsgSMONC(std::vector<char>& payload)
{
	if (payload.size() > 8 && payload[0] == '^' && payload[1] == 'S' && payload[2] == 'M' && payload[3] == 'O' && payload[4] == 'N' && payload[5] == 'C' && payload[6] == ':')
	{
		std::vector<char>::iterator PtrBegin = Find(payload.begin(), payload.end(), ':');
		std::vector<char>::iterator PtrEnd = PtrBegin;

		int FieldQty = GetFieldsQty(payload) + 1;

		int BaseQty = FieldQty / 9;

		for (int j = 0; j < BaseQty; ++j)
		{
			tMsgSMONC_BS BaseStation;

			memset(&BaseStation, 0, sizeof(BaseStation));

			for (int i = 0; i < 9; ++i)//there are 9 fields for a BaseStation
			{
				if (PtrBegin != payload.end())
				{
					PtrEnd = Find(PtrBegin + 1, payload.end(), ',');

					if (PtrEnd > PtrBegin)
					{
						std::vector<char> Field(PtrBegin + 1, PtrEnd);

						Field.push_back(0);

						switch (i)
						{
						case 0: BaseStation.MCC = utils::Read<unsigned short>(&Field[0], utils::tRadix_10); break;
						case 1: BaseStation.MNC = utils::Read<unsigned short>(&Field[0], utils::tRadix_10); break;
						case 2: BaseStation.LAC = utils::Read<unsigned short>(&Field[0], utils::tRadix_16); break;
						case 3: BaseStation.Cell = utils::Read<unsigned short>(&Field[0], utils::tRadix_16); break;
						case 4: BaseStation.BSIC = utils::Read<unsigned char>(&Field[0], utils::tRadix_10); break;
						case 5: BaseStation.chann = utils::Read<unsigned short>(&Field[0], utils::tRadix_10); break;
						case 6: BaseStation.RSSI = utils::Read<unsigned char>(&Field[0], utils::tRadix_10); break;
						case 7: BaseStation.C1 = utils::Read<unsigned char>(&Field[0], utils::tRadix_10); break;
						case 8: BaseStation.C2 = utils::Read<unsigned char>(&Field[0], utils::tRadix_10); break;
						}
					}
				}
				else
				{
					return;
				}

				PtrBegin = Find(PtrBegin + 1, payload.end(), ',');
			}

			m_BaseStations.push_back(BaseStation);
		}
	}
}

bool tMsgSMONC::TryParse(std::vector<char>& payload, tMsgSMONC& message)
{
	tMsgSMONC Message(payload);

	if (Message.m_BaseStations.size() != 0)
	{
		message = Message;

		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}
