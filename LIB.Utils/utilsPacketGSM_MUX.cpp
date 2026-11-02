#include "modGsmCinterionBGS2R1_PacketMUX.h"

//#include <string.h>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketMUX
		{

const char ContainerSTX = 0xF9;
const char ContainerETX = 0xF9;

std::vector<char> FindPacket(std::vector<char>& receivedData)
{
	std::vector<char> Packet;

	const int PacketLength_Max = 4098;//4095 Bytes data in accordance with ???

	size_t ReceivedStxPos = 0;

	while (1)
	{
		for (; ReceivedStxPos < receivedData.size(); ++ReceivedStxPos)
		{
			if (receivedData[ReceivedStxPos] == ContainerSTX)
				break;
		}

		if (ReceivedStxPos >= receivedData.size())
			break;

		if (receivedData.size() - ReceivedStxPos >= ContainerSize)
		{
			tPacketFieldLength FieldLength;

			FieldLength.Value = receivedData[ReceivedStxPos + 3];

			int PayloadLength = FieldLength.Field.Length;

			if (FieldLength.Field.EA == 0)
			{
				PayloadLength |= (unsigned char)receivedData[ReceivedStxPos + 4];
			}

			if (PayloadLength < 0)
			{
				ReceivedStxPos++;
				continue;
			}

			int PacketLength = PayloadLength + ContainerSize;
			if (FieldLength.Field.EA == 0)
			{
				PacketLength += 1;
			}

			if (PacketLength > PacketLength_Max)
			{
				ReceivedStxPos++;
				continue;
			}

			if (PacketLength <= receivedData.size() - ReceivedStxPos && receivedData[ReceivedStxPos + PacketLength - 1] == ContainerETX)
			{
				unsigned char FieldFCSReceived = receivedData[ReceivedStxPos + PacketLength - 2];

				int StartFCSIndex = ReceivedStxPos + 1;

				unsigned char FieldFCS = utils::crc::CRC08_GSM0710FCS(&receivedData[StartFCSIndex], 3);

				if (FieldFCSReceived == FieldFCS)
				{
					Packet = std::vector<char>(receivedData.begin() + ReceivedStxPos, receivedData.begin() + ReceivedStxPos + PacketLength);

					receivedData.erase(receivedData.begin(), receivedData.begin() + ReceivedStxPos + PacketLength);

					break;
				}
				else
				{
					ReceivedStxPos++;
				}
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
tPacket::tPacket()
{
	m_Address.Value = 0;
	m_Control.Value = 0;
}

tPacket::tPacket(tPacketFieldControlFrameType frameType, bool command, unsigned char dlci)
{
	m_Address.Value = 0;

	m_Address.Field.EA = 1;
	m_Address.Field.CR = (command ? 1 : 0);
	m_Address.Field.DLCI = dlci;

	m_Control.Value = 0;

	m_Control.FrameType = frameType;

	m_Control.Field.PF = 1;//For each DLCI, only one frame with P = 1 may ever be expected. (wm01_mux_guide_v13.pdf p.27)
}

tPacket::tPacket(tPacketFieldControlFrameType frameType, bool command, unsigned char dlci, std::vector<char>& payload)
{
	m_Address.Value = 0;

	m_Address.Field.EA = 1;
	m_Address.Field.CR = (command ? 1 : 0);
	m_Address.Field.DLCI = dlci;

	m_Control.Value = 0;

	m_Control.FrameType = frameType;

	m_Control.Field.PF = 1;//For each DLCI, only one frame with P = 1 may ever be expected. (wm01_mux_guide_v13.pdf p.27)

	m_Payload = payload;
}

bool tPacket::TryParse(const std::vector<char>& packetRaw, tPacket& packet)
{
	tPacket Pack;// (payload);

	if (packetRaw.size() >= ContainerSize)
	{
		int Index = 1;

		Pack.m_Address.Value = packetRaw[Index++];
		Pack.m_Control.Value = packetRaw[Index++];

		int PayloadSize = 0;

		tPacketFieldLength Length;

		Length.Value = packetRaw[Index++];

		PayloadSize = Length.Field.Length;

		////In case of Cinterion BGS2 it should be 1
		//if (Length.Field.EA == 0)//E/A = 0: two octets for the Length Indicator
		//{
		//	Length.Value = packetRaw[Index++];

		//	PayloadSize += (Length.Field.Length << 8);
		//}

		Pack.m_Payload = std::vector<char>(PayloadSize);

		if (packetRaw.size() >= ContainerSize + PayloadSize)
		{
			for (int i = 0; i < PayloadSize; ++i)
			{
				Pack.m_Payload[i] = packetRaw[Index++];
			}

			unsigned char FCS = utils::crc::CRC08_GSM0710FCS(&packetRaw[1], 3);

			if (FCS == static_cast<unsigned char>(packetRaw[Index++]))
			{
				packet = Pack;

				return true;
			}
		}
	}

	return false;
}

std::vector<char> tPacket::ToVector()
{
	std::vector<char> Packet;

	Packet.reserve(ContainerSize + m_Payload.size());

	Packet.push_back(ContainerSTX);
	Packet.push_back(static_cast<char>(m_Address.Value));
	Packet.push_back(static_cast<char>(m_Control.Value));

	tPacketFieldLength Length;
	Length.Value = 0;

	Length.Field.EA = 1;//In case of Cinterion BGS2 it should be 1
	Length.Field.Length = m_Payload.size();

	Packet.push_back(static_cast<char>(Length.Value));

	Packet.insert(Packet.end(), m_Payload.begin(), m_Payload.end());

	unsigned char FCS = utils::crc::CRC08_GSM0710FCS(&Packet[1], 3);

	Packet.push_back(static_cast<char>(FCS));

	Packet.push_back(ContainerETX);

	return Packet;
}

/*std::vector<char> MakePacket(std::vector<char>& payload)
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
}*/
///////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}
