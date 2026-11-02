#include "modGsmCinterionBGS2R1_PacketSMS.h"

#include <stdio.h>

namespace mod
{
	namespace GsmCinterionBGS2R1
	{
		namespace PacketSMS
		{

std::vector<char> GetPduPayload(const std::vector<char>& data)
{
	// > 07
	// 91
	// 97 62 92 90 90 F0
	// 04
	// > 0B
	// 91
	// 97 62 92 03 25 F7
	// 00
	// F4
	// 90 90 32 61 02 40 61
	// 0B
	// 24 0A 90 01 00 12 00 87 D6 0A 0D

	// Length of the SMSC information
	unsigned char Offset = utils::Read<unsigned char>(std::string(data.begin(), data.begin() + 2).c_str(), utils::tRadix_16) * 2;

	Offset += 4;	// first octet comprising "Length of the SMSC information" and
					// "first octet of this SMS_DELIVER message"
					// Address-length, length of the sender number
	unsigned char TmpValue = utils::Read<unsigned char>(std::string(data.begin() + Offset, data.begin() + Offset + 2).c_str(), utils::tRadix_16);

	Offset += TmpValue;

	if (TmpValue % 2)
	{
		Offset += 1;
	}

	Offset += 11 * 2;
	// Address-length, length of the sender number	- 1 * 2
	// Type of address of the sender number			- 1 * 2
	// TP-PID - Protocol identifier					- 1 * 2
	// TP-DSC - Data coding scheme					- 1 * 2
	// TP-SCTS - Time stamp							- 7 * 2

	unsigned char Length = utils::Read<unsigned char>(std::string(data.begin() + Offset, data.begin() + Offset + 2).c_str(), utils::tRadix_16);

	// TP-UDL - User data length					- 1 * 2 [*] it depends on TP-DSC
	std::vector<char>::const_iterator Begin = data.begin() + Offset + 2;

	if (Begin < data.end())
	{
		std::string PayloadStr(data.begin() + Offset + 2, data.end());

		if (PayloadStr.size() % 2 == 0)
		{
			int PayloadSize = PayloadStr.size() / 2;

			std::vector<char> Payload(PayloadSize);

			for (size_t i = 0, j = 0; i < PayloadStr.size(); i += 2, ++j)
			{
				Payload[j] = utils::Read<unsigned char>(std::string(PayloadStr.begin() + i, PayloadStr.begin() + i + 2).c_str(), utils::tRadix_16);
			}

			return Payload;
		}
	}

	return std::vector<char>();
}

std::vector<char> EncodePduPhoneNumber(const std::vector<char>& phoneNumber)
{
	size_t PhoneNumberEncSize = phoneNumber.size();

	if (PhoneNumberEncSize % 2)
	{
		PhoneNumberEncSize++;
	}

	std::vector<char> PhoneNumberEnc;

	size_t i = 0;

	for (; i < phoneNumber.size() - 1; i += 2)
	{
		PhoneNumberEnc.push_back(phoneNumber[i + 1]);
		PhoneNumberEnc.push_back(phoneNumber[i]);
	}

	if (PhoneNumberEncSize > phoneNumber.size())
	{
		PhoneNumberEnc.push_back('F');
		PhoneNumberEnc.push_back(phoneNumber[i]);
	}

	return PhoneNumberEnc;
}

unsigned char GetPduTPValidityPeriod(unsigned int value)
{
	unsigned char Lifetime = 0;

	value = value > 5 ? value : 5;
	value = value / 60 < 10584 ? value : 10584 * 60;

	if (value / 60 <= 12) // up to 12 hours
	{
		Lifetime = (unsigned char)(value / 5 - 1);
	}
	else if (value / 60 <= 24) // up to 24 hours
	{
		Lifetime = (unsigned char)((value - 12 * 60) / 30 + 143);
	}
	else if (value / 60 <= 720) // up to 30 days
	{
		Lifetime = (unsigned char)(value / 24 * 60 + 166);
	}
	else if (value / 60 <= 10584) // up to 63 weeks
	{
		Lifetime = (unsigned char)(value / 7 * 24 * 60 + 192);
	}
	return Lifetime;
}

int GetSmsSize(const std::vector<char>& phoneNumber, const std::vector<char>& payload)
{
	int Size = 2 // (excepting length of SMSC = 1)
		+ 2 // Address length and type of address
		+ phoneNumber.size() / 2//[srg]2016-09-26 THAT'S NOT CORRECT (that's correct if a number is an even number, if odd - it will be less than is to be)
		+ 2 // TP-PID and TP-DSC
		+ 2 // TP-Validity-Period and TP-User-data-Length
		+ payload.size();

	return Size;
}

std::vector<char> GetSmsBody(const std::vector<char>& phoneNumber, const std::vector<char>& payload, unsigned int lifeTime)
{
	std::vector<char> PhoneNumber = phoneNumber;

	if (PhoneNumber[0] == '+')
	{
		PhoneNumber.erase(PhoneNumber.begin());

		std::vector<char> PhoneNumberSMS = EncodePduPhoneNumber(PhoneNumber);

		// [#] The SMSC-number must be set in a SIM-card or a modem

		int DataHeaderSize = PhoneNumberSMS.size() + 18;//+ 6 + 12
		int DataPayloadSize = payload.size() * 2 + 1;//2;// +1 for '\x1a'
		int DataSize = DataHeaderSize + DataPayloadSize;

		std::vector<char> Data(DataSize);
	   //char *strHDR = "001100";
	   //char *strAIF = "91"; // it indicates international format of the address
	   //char *strTP_PID = "00";
	   //char *strTP_DSC = "F4";
		int PhoneNumberSMSSize = PhoneNumberSMS.size();

		PhoneNumberSMS.push_back(0);//'\0' to make a string

#ifdef LIB_MODULE_GSM_SMS_FLASH
		sprintf(&Data[0], "001100%02X91%s00F4%02X%02X",
#else
		sprintf(&Data[0], "001100%02X91%s0004%02X%02X",
#endif//LIB_MODULE_GSM_SMS_FLASH
			PhoneNumberSMSSize,
			&PhoneNumberSMS[0],
			GetPduTPValidityPeriod(lifeTime),
			payload.size()); // Total: 16 + strHDR + address

		unsigned short *DataPtr = (unsigned short*)&Data[DataHeaderSize];

		for (unsigned int i = 0; i < payload.size(); ++i)
		{
			sprintf((char*)DataPtr++, "%02X", (unsigned char)payload[i]);
		}

		*(Data.end() - 1) = '\x1a';

		return Data;
	}
	else
	{
		std::vector<char> Data;
		Data.push_back('\x1a');

		return Data;
	}
}

		}
	}
}
