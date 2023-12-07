#include "utilsMIFARE.h"
#include "utilsTest.h"

#include <algorithm>
#include <iostream>

namespace utils
{

void UnitTest_CardMIFARE_Classic()
{
	std::cout << "\n""utils::card_MIFARE::classic\n";
//Sector:  0
// d0 f9 aa 1b  98 08 04 00  62 63 64 65  66 67 68 69    00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
// 00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00    00 00 00 00  00 00 ff 07  80 69 ff ff  ff ff ff ff
// Key A: xx xx xx xx xx xx
// Key B: ff ff ff ff ff ff
// Access: 0 0 0   0 0 0   0 0 0   0 0 1   69
// 
//All keys are set to FFFF FFFF FFFFh at chip delivery and the bytes 6, 7 and 8 are set to FF0780h.

	using tKeyID = card_MIFARE::classic::tKeyID;
	using tKey = card_MIFARE::classic::tKey;
	using tBlockID = card_MIFARE::classic::tBlockID;
	using tBlock = card_MIFARE::classic::tBlock;
	using tAccess = card_MIFARE::classic::tAccess;
	using tSector = card_MIFARE::classic::tSector;
	using tSector0 = card_MIFARE::classic::tSector0;
	using tCard1K = card_MIFARE::classic::tCard1K;

	union tAccessCalc
	{
		struct
		{
			std::uint32_t C1_0_ : 1;
			std::uint32_t C1_1_ : 1;
			std::uint32_t C1_2_ : 1;
			std::uint32_t C1_3_ : 1;
			std::uint32_t C2_0_ : 1;
			std::uint32_t C2_1_ : 1;
			std::uint32_t C2_2_ : 1;
			std::uint32_t C2_3_ : 1;
			std::uint32_t C3_0_ : 1;
			std::uint32_t C3_1_ : 1;
			std::uint32_t C3_2_ : 1;
			std::uint32_t C3_3_ : 1;
			std::uint32_t C1_0 : 1;
			std::uint32_t C1_1 : 1;
			std::uint32_t C1_2 : 1;
			std::uint32_t C1_3 : 1;
			std::uint32_t C2_0 : 1;
			std::uint32_t C2_1 : 1;
			std::uint32_t C2_2 : 1;
			std::uint32_t C2_3 : 1;
			std::uint32_t C3_0 : 1;
			std::uint32_t C3_1 : 1;
			std::uint32_t C3_2 : 1;
			std::uint32_t C3_3 : 1;
			std::uint32_t UserData : 8;
		}Field;
		std::uint32_t Value = 0;
	};

	struct tAccessBlock3TrailerCheck
	{
		bool R_KeyA = false;
		bool W_KeyA = false;
		bool R_Access = false;
		bool W_Access = false;
		bool R_KeyB = false;
		bool W_KeyB = false;
	};

	auto AccessBlock3TrailerTest = [](const std::string& testID, std::uint32_t value, tKeyID keyID, tAccessBlock3TrailerCheck accessCheck)
	{
		tAccess Access(value);
		if (!Access.good())
		{
			test::RESULT(testID, false);
			return;
		}
		std::cout << "                  " << Access.ToString() << '\n';
		bool Result =
			accessCheck.R_KeyA == Access.IsR_KeyA(keyID) &&
			accessCheck.W_KeyA == Access.IsW_KeyA(keyID) &&
			accessCheck.R_KeyB == Access.IsR_KeyB(keyID) &&
			accessCheck.W_KeyB == Access.IsW_KeyB(keyID) &&
			accessCheck.R_Access == Access.IsR_Access(keyID) &&
			accessCheck.W_Access == Access.IsW_Access(keyID);
		test::RESULT(testID, Result);
	};

	tAccessCalc AccessCalc;
	AccessCalc.Value = 0x698007FF;

	auto SetAccessCx = [&AccessCalc](tBlockID blockID, bool c1, bool c2, bool c3)
	{
		switch (blockID)
		{
		case tBlockID::Block_0:
			AccessCalc.Field.C1_0 = c1 ? 1 : 0;
			AccessCalc.Field.C1_0_ = c1 ? 0 : 1;
			AccessCalc.Field.C2_0 = c2 ? 1 : 0;
			AccessCalc.Field.C2_0_ = c2 ? 0 : 1;
			AccessCalc.Field.C3_0 = c3 ? 1 : 0;
			AccessCalc.Field.C3_0_ = c3 ? 0 : 1;
			break;
		case tBlockID::Block_1:
			AccessCalc.Field.C1_1 = c1 ? 1 : 0;
			AccessCalc.Field.C1_1_ = c1 ? 0 : 1;
			AccessCalc.Field.C2_1 = c2 ? 1 : 0;
			AccessCalc.Field.C2_1_ = c2 ? 0 : 1;
			AccessCalc.Field.C3_1 = c3 ? 1 : 0;
			AccessCalc.Field.C3_1_ = c3 ? 0 : 1;
			break;
		case tBlockID::Block_2:
			AccessCalc.Field.C1_2 = c1 ? 1 : 0;
			AccessCalc.Field.C1_2_ = c1 ? 0 : 1;
			AccessCalc.Field.C2_2 = c2 ? 1 : 0;
			AccessCalc.Field.C2_2_ = c2 ? 0 : 1;
			AccessCalc.Field.C3_2 = c3 ? 1 : 0;
			AccessCalc.Field.C3_2_ = c3 ? 0 : 1;
			break;
		case tBlockID::Block_3_Trailer:
			AccessCalc.Field.C1_3 = c1 ? 1 : 0;
			AccessCalc.Field.C1_3_ = c1 ? 0 : 1;
			AccessCalc.Field.C2_3 = c2 ? 1 : 0;
			AccessCalc.Field.C2_3_ = c2 ? 0 : 1;
			AccessCalc.Field.C3_3 = c3 ? 1 : 0;
			AccessCalc.Field.C3_3_ = c3 ? 0 : 1;
			break;
		}
	};

	SetAccessCx(tBlockID::Block_3_Trailer, false, false, false);
	AccessBlock3TrailerTest("1 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, true, true, false, true, true });
	AccessBlock3TrailerTest("1 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, false, false, false, false });

	SetAccessCx(tBlockID::Block_3_Trailer, false, true, false);
	AccessBlock3TrailerTest("2 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, true, false });
	AccessBlock3TrailerTest("2 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, false, false, false, false });

	SetAccessCx(tBlockID::Block_3_Trailer, true, false, false);
	AccessBlock3TrailerTest("3 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, false, false });
	AccessBlock3TrailerTest("3 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, true, true, false, false, true });

	SetAccessCx(tBlockID::Block_3_Trailer, true, true, false);
	AccessBlock3TrailerTest("4 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, false, false });
	AccessBlock3TrailerTest("4 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, true, false, false, false });

	SetAccessCx(tBlockID::Block_3_Trailer, false, false, true);
	AccessBlock3TrailerTest("5 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, true, true, true, true, true });
	AccessBlock3TrailerTest("5 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, false, false, false, false });

	AccessBlock3TrailerTest("5 Access - transport configuration, KeyA", 0x698007FF, tKeyID::A, { false, true, true, true, true, true });
	AccessBlock3TrailerTest("5 Access - transport configuration, KeyB", 0x698007FF, tKeyID::B, { false, false, false, false, false, false });

	SetAccessCx(tBlockID::Block_3_Trailer, false, true, true);
	AccessBlock3TrailerTest("6 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, false, false });
	AccessBlock3TrailerTest("6 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, true, true, true, false, true });

	SetAccessCx(tBlockID::Block_3_Trailer, true, false, true);
	AccessBlock3TrailerTest("7 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, false, false });
	AccessBlock3TrailerTest("7 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, true, true, false, false });

	SetAccessCx(tBlockID::Block_3_Trailer, true, true, true);
	AccessBlock3TrailerTest("8 Access, KeyA", AccessCalc.Value, tKeyID::A, { false, false, true, false, false, false });
	AccessBlock3TrailerTest("8 Access, KeyB", AccessCalc.Value, tKeyID::B, { false, false, true, false, false, false });

	struct tAccessCheck
	{
		bool R = false;
		bool W = false;
		bool Incr = false;
		bool Decr = false;
	};

	auto AccessTest = [](const std::string& testID, std::uint32_t value, tKeyID keyID, tBlockID blockID, tAccessCheck accessCheck)
	{
		tAccess Access(value);
		if (!Access.good())
		{
			test::RESULT(testID, false);
			return;
		}
		std::cout << "                  " << Access.ToString() << '\n';
		bool Result =
			accessCheck.R == Access.IsR_Data(keyID, blockID) &&
			accessCheck.W == Access.IsW_Data(keyID, blockID)&&
			accessCheck.Incr == Access.IsIncr_Data(keyID, blockID)&&
			accessCheck.Decr == Access.IsDecr_Data(keyID, blockID);
		test::RESULT(testID, Result);
	};

	auto AssessTestBlock = [&](tBlockID blockID)
	{
		std::cout << "\nBlock " << (int)blockID << '\n';

		AccessTest("Block - transport configuration, KeyA", 0x698007FF, tKeyID::A, blockID, { true, true, true, true });
		AccessTest("Block - transport configuration, KeyB", 0x698007FF, tKeyID::B, blockID, { true, true, true, true });

		AccessCalc.Value = 0x698007FF;

		SetAccessCx(blockID, false, false, false); // Transport Configuration
		AccessTest("1 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { true, true, true, true });
		AccessTest("1 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, true, true, true });

		SetAccessCx(blockID, false, true, false);
		AccessTest("2 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { true, false, false, false });
		AccessTest("2 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, false, false, false });

		SetAccessCx(blockID, true, false, false);
		AccessTest("3 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { true, false, false, false });
		AccessTest("3 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, true, false, false });

		SetAccessCx(blockID, true, true, false);
		AccessTest("4 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { true, false, false, true });
		AccessTest("4 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, true, true, true });

		SetAccessCx(blockID, false, false, true);
		AccessTest("5 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { true, false, false, true });
		AccessTest("5 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, false, false, true });

		SetAccessCx(blockID, false, true, true);
		AccessTest("6 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { false, false, false, false });
		AccessTest("6 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, true, false, false });

		SetAccessCx(blockID, true, false, true);
		AccessTest("7 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { false, false, false, false });
		AccessTest("7 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { true, false, false, false });

		SetAccessCx(blockID, true, true, true);
		AccessTest("8 Block, KeyA", AccessCalc.Value, tKeyID::A, blockID, { false, false, false, false });
		AccessTest("8 Block, KeyB", AccessCalc.Value, tKeyID::B, blockID, { false, false, false, false });
	};

	AssessTestBlock(tBlockID::Block_0);
	AssessTestBlock(tBlockID::Block_1);
	AssessTestBlock(tBlockID::Block_2);

	{
		tBlock Block0 = { 0xd0, 0xf9, 0xaa, 0x1b, 0x98, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69 };
		tBlock Block1 = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		tBlock Block2 = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		tBlock Block3 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x80, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

		tSector Sector(tKeyID::A);
		Sector.push_back_block(Block0);
		Sector.push_back_block(Block1);
		Sector.push_back_block(Block2);
		Sector.push_back_block(Block3);
		std::cout << Sector.ToJSON() << '\n';
		std::cout << Sector.ToString() << '\n';
	}

	{
		tBlock Block0 = { 0xd0, 0xf9, 0xaa, 0x1b, 0x98, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69 };
		tBlock Block1 = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		tBlock Block2 = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		tBlock Block3 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x80, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

		tSector0 Sector(tKeyID::A);
		Sector.push_back_block(Block0);
		Sector.push_back_block(Block1);
		Sector.push_back_block(Block2);
		Sector.push_back_block(Block3);
		std::cout << Sector.ToJSON() << '\n';
		std::cout << Sector.ToString() << '\n';
	}

	{
		std::vector<std::uint8_t> Data = {
			0xd0, 0xf9, 0xaa, 0x1b, 0x98, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
			0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x80, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
		};

		tSector0 Sector(tKeyID::A);
		Sector.push(Data);
		std::cout << Sector.ToJSON() << '\n';
		std::cout << Sector.ToString() << '\n';
	}

	{
		std::vector<std::uint8_t> Data = { 0xd0, 0xf9, 0xaa, 0x1b, 0x98, 0x45 };
		tKey Key(Data.begin(), Data.end());
		test::RESULT("tKey init vector", Key.Get() == 0x000045981baaf9d0);
	}

	{
		tKey Key;
		Key.Increment();
		test::RESULT("tKey Increment", Key.Get() == 0);
	}

	{
		tKey Key(0);
		Key.Decrement();
		test::RESULT("tKey Decrement", Key.Get() == 0x0000FFFFFFFFFFFF);
	}

	{
		tCard1K Card;

		std::vector<std::uint8_t> Data = {
			0xd0, 0xf9, 0xaa, 0x1b, 0x98, 0x08, 0x04, 0x00, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x80, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
		};

		Card.push(tSector0(tKeyID::A, tKey(), Data));
		for (std::uint8_t i = 0; i < 16; ++i)
		{
			Data[17] = i;
			Card.push(tSector(tKeyID::A, tKey(), Data));
		}
		std::cout << "\n" << Card.ToJSON() << '\n';
		std::cout <<"\nCard1K\n\n" << Card.ToString() << '\n';
	}

	std::cout << std::endl;
}

void UnitTest_CardMIFARE_Ultralight()
{
	using tBlock = card_MIFARE::ultralight::tBlock;

	std::cout << "\n""utils::card_MIFARE::ultralight\n";

	// {"sak": "00", "uid" : "04d2ea998c0280", "payload" : "04d2eab4998c028097480203fffffeff32333435363738393a3b3c3d3e3f404115ac1e000b00000042434445464748494a4b4c4d4e4f50515253545556575859"}

	{
		std::vector<std::uint8_t> Data = {
			0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97, // Serial number
			0x48, // Internal
			0x02,0x03, // Lock bytes (irreversible)
			0xff,0xff,0xfe,0xff, // OTP (irreversible)
			0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41, // User memory
			0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49, // User memory
			0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59, // User memory
		};
		card_MIFARE::ultralight::tCard Card(Data);
		std::cout << Card.ToJSON() << '\n';
		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();
		bool Result =
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40;
		test::RESULT("UL 1 c_tor", Result);
	}

	{
		std::vector<std::uint8_t> Data = {
			0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97,0x48,0x02,0x03,0xff,0xff,0xfe,0xff,
			0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,
			0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
			0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
		};
		card_MIFARE::ultralight::tCard Card;
		Card.push(Data);
		std::cout << Card.ToJSON() << '\n';
		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();
		bool Result =
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40;
		test::RESULT("UL 2 push", Result);
	}

	{
		std::vector<std::uint8_t> Data0 = { 0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97,0x48,0x02,0x03,0xff,0xff,0xfe,0xff };
		std::vector<std::uint8_t> Data1 = { 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41 };
		std::vector<std::uint8_t> Data2 = { 0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49 };
		std::vector<std::uint8_t> Data3 = { 0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59 };

		card_MIFARE::ultralight::tCard Card;
		Card.push_back_block(Data0);
		Card.push_back_block(Data1);
		Card.push_back_block(Data2);
		Card.push_back_block(Data3);
		std::cout << Card.ToJSON() << '\n';
		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();
		bool Result =
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40;
		test::RESULT("UL 3 push vector", Result);
	}
	
	{
		tBlock Data0 = { 0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97,0x48,0x02,0x03,0xff,0xff,0xfe,0xff };
		tBlock Data1 = { 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41 };
		tBlock Data2 = { 0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49 };
		tBlock Data3 = { 0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59 };

		card_MIFARE::ultralight::tCard Card;
		Card.push_back_block(Data0);
		Card.push_back_block(Data1);
		Card.push_back_block(Data2);
		Card.push_back_block(Data3);
		std::cout << Card.ToJSON() << '\n';
		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();
		bool Result =
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40 &&
			Data0 == Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_0_System) &&
			Data1 == Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_1)&&
			Data2 == Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_2)&&
			Data3 == Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_3);
		test::RESULT("UL 4 push block", Result);
	}

	{
		tBlock Data0 = { 0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97,0x48,0x02,0x03,0xff,0xff,0xfe,0xff };
		tBlock Data1 = { 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41 }; // User memory
		tBlock Data2 = { 0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49 }; // User memory
		tBlock Data3 = { 0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59 }; // User memory

		card_MIFARE::ultralight::tCard Card;
		Card.push_back_block(Data0);
		Card.push_back_block(Data1);
		Card.push_back_block(Data2);
		Card.push_back_block(Data3);
		std::cout << Card.ToJSON() << '\n';

		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();

		std::vector<std::uint8_t> UserMemoryPrev;
		UserMemoryPrev.insert(UserMemoryPrev.end(), Data1.begin(), Data1.end());
		UserMemoryPrev.insert(UserMemoryPrev.end(), Data2.begin(), Data2.end());
		UserMemoryPrev.insert(UserMemoryPrev.end(), Data3.begin(), Data3.end());

		std::vector<std::uint8_t> UserMemory = Card.GetUserMemory();
		assert(UserMemory.size() == 48);

		std::vector<std::uint8_t> Data1a = { 0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F }; // User memory
		std::vector<std::uint8_t> Data2a = { 0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F }; // User memory
		std::vector<std::uint8_t> Data3a = { 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F }; // User memory
		std::vector<std::uint8_t> UserMemoryNew;
		UserMemoryNew.insert(UserMemoryNew.end(), Data1a.begin(), Data1a.end());
		UserMemoryNew.insert(UserMemoryNew.end(), Data2a.begin(), Data2a.end());
		UserMemoryNew.insert(UserMemoryNew.end(), Data3a.begin(), Data3a.end());
		Card.SetUserMemory(UserMemoryNew);
		std::cout << Card.ToJSON() << '\n';

		bool Result =
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40 &&
			UserMemory == UserMemoryPrev &&
			Data0 == Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_0_System) &&
			std::equal(Data1a.begin(), Data1a.end(), Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_1)->begin()) &&
			std::equal(Data2a.begin(), Data2a.end(), Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_2)->begin()) &&
			std::equal(Data3a.begin(), Data3a.end(), Card.GetBlock(card_MIFARE::ultralight::tBlockID::Block_3)->begin());
		test::RESULT("UL 5 Get/SetUserMemory", Result);
	}
	
	{
		tBlock Data0 = { 0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97,0x48,0x02,0x03,0xff,0xff,0xfe,0xff };
		tBlock Data1 = { 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41 }; // User memory
		tBlock Data2 = { 0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49 }; // User memory
		tBlock Data3 = { 0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59 }; // User memory

		card_MIFARE::ultralight::tCard Card;
		Card.push_back_block(Data0);
		Card.push_back_block(Data1);
		Card.push_back_block(Data2);
		Card.push_back_block(Data3);
		std::cout << Card.ToJSON() << '\n';

		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		std::size_t UserMemAvailSize = Card.GetUserMemoryUnlockedSize();

		std::vector<std::uint8_t> UnlockedData = Card.ReadUserMemoryUnlocked();

		bool Result = UserMemAvailSize == UnlockedData.size();
		for (std::size_t i = 0, j = 50; i < UnlockedData.size(); ++i, ++j)
		{
			if (UnlockedData[i] != j)
				Result = false;
		}

		std::reverse(UnlockedData.begin(), UnlockedData.end());
		Card.WriteUserMemoryUnlocked(UnlockedData);
		std::cout << Card.ToJSON() << '\n';

		UnlockedData = Card.ReadUserMemoryUnlocked();

		Result = Result && UserMemAvailSize == UnlockedData.size();
		for (std::size_t i = 0, j = 89; i < UnlockedData.size(); ++i, --j)
		{
			if (UnlockedData[i] != j)
				Result = false;
		}

		Result = Result &&
			Lock.Value == 0x0302 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_09 == 1 &&
			UserMemAvailSize == 40;
		test::RESULT("UL 6 Read/WriteUserMemoryUnlocked", Result);
	}

	{
		std::vector<std::uint8_t> Data = {
			0x04,0xd2,0xea,0xb4,0x99,0x8c,0x02,0x80,0x97, // Serial number
			0x48, // Internal
			0x02,0x03, // Lock bytes (irreversible)
			0xff,0xff,0xfe,0xff, // OTP (irreversible)
			0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41, // User memory
			0x15,0xac,0x1e,0x00,0x0b,0x00,0x00,0x00,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49, // User memory
			0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59, // User memory
		};
		card_MIFARE::ultralight::tCard Card(Data);
		std::cout << Card.ToJSON() << '\n';
		card_MIFARE::ultralight::tLock Lock = Card.GetLock();
		Lock.Field.BL_OTP = 1;
		Lock.Field.L_05 = 1;
		Lock.Field.L_14 = 1;
		Card.SetLock(Lock);
		std::cout << Card.ToJSON() << '\n';
		Lock = Card.GetLock();

		bool Result =
			Lock.Field.BL_OTP == 1 &&
			Lock.Field.BL_09_04 == 1 &&
			Lock.Field.L_08 == 1 &&
			Lock.Field.L_05 == 1 &&
			Lock.Field.L_09 == 1 &&
			Lock.Field.L_14 == 1;
		test::RESULT("UL 7 Get/SetLock", Result);
	}

	std::cout << std::endl;
}

void UnitTest_CardMIFARE()
{
	UnitTest_CardMIFARE_Classic();
	UnitTest_CardMIFARE_Ultralight();
}

}