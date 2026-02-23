#include "utilsTest.h"

#include <iomanip>
#include <iostream>

#ifndef __GNUC__
#include <Windows.h>
#endif//__GNUC__

namespace utils
{
	namespace test
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __GNUC__
enum tConsoleColor
{
	tConsoleColor_Black = 0,
	tConsoleColor_Blue = 1,
	tConsoleColor_Green = 2,
	tConsoleColor_Cyan = 3,
	tConsoleColor_Red = 4,
	tConsoleColor_Magenta = 5,
	tConsoleColor_Brown = 6,
	tConsoleColor_LightGray = 7,
	tConsoleColor_DarkGray = 8,
	tConsoleColor_LightBlue = 9,
	tConsoleColor_LightGreen = 10,
	tConsoleColor_LightCyan = 11,
	tConsoleColor_LightRed = 12,
	tConsoleColor_LightMagenta = 13,
	tConsoleColor_Yellow = 14,
	tConsoleColor_White = 15
};
#else//__GNUC__
enum tConsoleColor
{
	tConsoleColor_Black = 0,
	tConsoleColor_Red = 1,
	tConsoleColor_Green = 2,
	tConsoleColor_Yellow = 3,
	tConsoleColor_Blue = 4,
	tConsoleColor_Magenta = 5,
	tConsoleColor_Cyan = 6,
	tConsoleColor_White = 7
};
#endif//__GNUC__
///////////////////////////////////////////////////////////////////////////////////////////////////
void ASSERT(bool value)
{
	if (value)
	{
		std::cout << "\n\n";

#ifndef __GNUC__
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, (WORD)((tConsoleColor_White << 4) | tConsoleColor_Black));
#else//__GNUC__
		std::cout << "\x1b[3" << tConsoleColor_Black << ";4" << tConsoleColor_White << "m";
#endif//__GNUC__

		std::cout << "ASSERT -> ERROR";

#ifndef __GNUC__
		SetConsoleTextAttribute(hConsole, (WORD)((tConsoleColor_Black << 4) | tConsoleColor_LightGray));
#else//__GNUC__
		std::cout << "\x1b[0m";
#endif//__GNUC__

		std::cout << "\n\n\n";

		std::cin.get();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int RESULT_CountTests = 0;
int RESULT_CountErrors = 0;
void RESULT(const char* msg, bool result)
{
	++RESULT_CountTests;

#ifndef __GNUC__
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//÷вет всего фона - белый. ÷вет всего текста - черный
	//system("color F0");
	//puts("Hello World!");
	//÷вет символов - желтый. ÷вет фона - темно-серый
	//SetConsoleTextAttribute(hConsole, (WORD)((DarkGray << 4) | Yellow));
#endif//__GNUC__

	std::cout << std::setw(70) << std::setfill('.') << std::setiosflags(std::ios::left) << msg;

	if (result)
	{
		std::cout << "OK";
	}
	else
	{
#ifndef __GNUC__
		SetConsoleTextAttribute(hConsole, (WORD)((tConsoleColor_Red << 4) | tConsoleColor_Yellow));
#else//__GNUC__
		std::cout << "\x1b[3" << tConsoleColor_Yellow << ";4" << tConsoleColor_Red << "m";
#endif//__GNUC__

		++RESULT_CountErrors;

		std::cout << "ERROR";

#ifndef __GNUC__
		SetConsoleTextAttribute(hConsole, (WORD)((tConsoleColor_Black << 4) | tConsoleColor_LightGray));
#else//__GNUC__
		std::cout << "\x1b[0m";
#endif//__GNUC__
	}

	std::cout << std::endl;
}

void RESULT(const std::string& msg, bool result)
{
	RESULT(msg.c_str(), result);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void RESULT_Total()
{
	std::cout << std::dec << "Errors: " << RESULT_CountErrors << " (from tests: " << RESULT_CountTests << ")\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void WARNING(const char* msg, bool show)
{
	if (show)
	{
#ifndef __GNUC__
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, (WORD)(tConsoleColor_LightRed));
#else//__GNUC__
		std::cout << "\x1b[3" << tConsoleColor_Red << "m";
		//std::cout << "\x1b[3" << tConsoleColor_Red << ";4" << tConsoleColor_Black << "m";
#endif//__GNUC__

		std::cout<<"WARNING: " << msg;

#ifndef __GNUC__
		SetConsoleTextAttribute(hConsole, (WORD)((tConsoleColor_Black << 4) | tConsoleColor_LightGray));
#else//__GNUC__
		std::cout << "\x1b[0m";
#endif//__GNUC__

		std::cout << std::endl;
	}
}

void WARNING(const std::string& msg, bool show)
{
	WARNING(msg.c_str(), show);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}