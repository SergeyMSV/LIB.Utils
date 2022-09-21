namespace utils
{
	void UnitTest_Base();
	void UnitTest_Chrono();
	void UnitTest_CRC();
	void UnitTest_CryptoAES();
	void UnitTest_CryptoRSA32();
	void UnitTest_Key();
	void UnitTest_tLog();
	void UnitTest_PacketBNTBX2();
	void UnitTest_PacketCameraVC0706();
	void UnitTest_PacketNMEA();
	void UnitTest_PacketNMEAPayload();
	void UnitTest_PacketNMEAType();
	void UnitTest_PacketStar();
	void UnitTest_Path();
	void UnitTest_PatternCommmand();
	void UnitTest_PatternState();
	void UnitTest_SerialPort();
	void UnitTest_Test();
	void UnitTest_Trap();
}

int main()
{
	using namespace utils;

	UnitTest_Base();
	UnitTest_Chrono();
	UnitTest_CRC();
	UnitTest_CryptoAES();
	UnitTest_CryptoRSA32();
	UnitTest_Key();
	//UnitTest_PacketBNTBX2();
	UnitTest_PacketCameraVC0706();
	UnitTest_PacketNMEA();
	UnitTest_PacketNMEAPayload();
	UnitTest_PacketNMEAType();
	UnitTest_PacketStar();
	UnitTest_Path();
	UnitTest_PatternCommmand();
	UnitTest_PatternState();
	UnitTest_SerialPort();
	//UnitTest_Test();
	UnitTest_Trap();

	UnitTest_tLog();

	return 0;
}