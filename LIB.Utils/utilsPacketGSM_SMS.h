///////////////////////////////////////////////////////////////////////////////////////////////////
// tGsmCinterionBGS2R1_PacketSMS
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// Standard ISO/IEC 114882, C++98
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 02 13  |
// |      2     |   2019 05 11  | Updated
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_SMS_H
#define LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_SMS_H

#include <libConfig.h>

#include <utilsConverter.h>

#include <vector>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketSMS
		{

std::vector<char> GetPduPayload(const std::vector<char>& data);

int GetSmsSize(const std::vector<char>& phoneNumber, const std::vector<char>& payload);
std::vector<char> GetSmsBody(const std::vector<char>& phoneNumber, const std::vector<char>& payload, unsigned int lifeTime);

		}
	}
}

#endif//LIB_MODULE_GSM_CINTERION_BGS2R1_PACKET_SMS_H
