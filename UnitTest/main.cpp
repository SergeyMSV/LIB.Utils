#include <utilsTest.h>

namespace utils
{
	void UnitTest_Base();
	void UnitTest_Chrono();
	void UnitTest_CRC();
	void UnitTest_CryptoAES();
	void UnitTest_CryptoRSA32();
	void UnitTest_DALLAS();
	void UnitTest_File();
	void UnitTest_Key();
	void UnitTest_Linux();
	void UnitTest_tLog();
	void UnitTest_MIFARE();
	//void UnitTest_PacketBNTBX2();
	void UnitTest_PacketCameraVC0706();
	void UnitTest_PacketJSON();
	void UnitTest_PacketMQTTv3_1_1();
	void UnitTest_PacketNMEA();
	void UnitTest_PacketNMEAPayload();
	void UnitTest_PacketNMEAType();
	void UnitTest_PacketStar();
	void UnitTest_PacketTWR();
	void UnitTest_Path();
	void UnitTest_PatternCommmand();
	void UnitTest_PatternState();
	void UnitTest_PortSerial();
	void UnitTest_Test();
	void UnitTest_Trap();
	void UnitTest_Version();
}

int main()
{
	using namespace utils;

	UnitTest_Base();
	UnitTest_Chrono();
	UnitTest_CRC();
	UnitTest_CryptoAES();
	UnitTest_CryptoRSA32();
	UnitTest_DALLAS();
	UnitTest_File();
	UnitTest_Key();
	UnitTest_Linux();
	UnitTest_MIFARE();
	//UnitTest_PacketBNTBX2();
	UnitTest_PacketCameraVC0706();
	UnitTest_PacketJSON();
	UnitTest_PacketMQTTv3_1_1();
	UnitTest_PacketNMEA();
	UnitTest_PacketNMEAPayload();
	UnitTest_PacketNMEAType();
	UnitTest_PacketStar();
	UnitTest_PacketTWR();
	UnitTest_Path();
	UnitTest_PatternCommmand();
	UnitTest_PatternState();
	UnitTest_PortSerial();
	//UnitTest_Test();
	UnitTest_Trap();
	UnitTest_Version();

	UnitTest_tLog();

	utils::test::RESULT_Total();

	return 0;
}