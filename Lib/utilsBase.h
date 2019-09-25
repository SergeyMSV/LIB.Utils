///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsBase.h
//
// Standard ISO/IEC 114882, C++17
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |     ...    |   2014 09 24  |
// |      1     |   2019 08 18  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <vector>

namespace utils
{

typedef unsigned char tUInt8;
typedef std::vector<tUInt8> tVectorUInt8;

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, void>::type Append(tVectorUInt8& dst, const T& value)
{
	const tUInt8* Begin = reinterpret_cast<const tUInt8*>(&value);

	dst.insert<const tUInt8*>(dst.end(), Begin, Begin + sizeof(value));
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, tVectorUInt8>::type ToVector(const T& value)
{
	tVectorUInt8 Data;

	Data.reserve(sizeof(value));

	const tUInt8* Begin = reinterpret_cast<const tUInt8*>(&value);

	Data.insert(Data.end(), Begin, Begin + sizeof(value));

	return Data;
}

template<typename T, typename Iterator>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(Iterator first, Iterator last)
{
	T Data = 0;

	auto Size = std::distance(first, last);

	if (Size <= sizeof(T))
	{
		std::copy(first, last, reinterpret_cast<tUInt8*>(&Data));
	}

	return Data;
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const char* data, size_t dataSize)
{
	return Read<T, const char*>(data, data + dataSize);
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const unsigned char* data, size_t dataSize)
{
	const char* Begin = reinterpret_cast<const char*>(data);

	return Read<T, const char*>(Begin, Begin + dataSize);
}

enum tRadix
{
	tRadix_10 = 10,
	tRadix_16 = 16,
};

template<typename T, typename Iterator, int N = 20>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(Iterator first, Iterator last, tRadix radix)
{
	char Str[N];//[#] and +/- and 0x00

	int StrIndex = 0;

	for (; first != last && StrIndex < sizeof(Str) - 1; ++first)
	{
		char Byte = static_cast<char>(*first);

		if (Byte >= '0' && Byte <= '9' ||
			radix == tRadix_10 && Byte == '-' && StrIndex == 0 ||
			radix == tRadix_16 && (Byte >= 'A' && Byte <= 'F' || Byte >= 'a' && Byte <= 'f'))
		{
			Str[StrIndex++] = Byte;
		}
		else if (StrIndex != 0)
		{
			break;
		}
	}

	Str[StrIndex] = 0;

	if (Str[0] == '-' && radix == tRadix_10)
	{
		return static_cast<T>(strtol(Str, 0, radix));
	}

	return static_cast<T>(strtoul(Str, 0, radix));
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Read(const char* data, tRadix radix)
{
	size_t DataSize = strlen(data);

	return Read<T, const char*>(data, data + DataSize, radix);
}

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value, T>::type Reverse(T value)
{
	tUInt8* Begin = reinterpret_cast<tUInt8*>(&value);

	std::reverse<tUInt8*>(Begin, Begin + sizeof(value));

	return value;
}

//char FromBCD(char dataBCD); [TBD]
//char ToBCD(char dataBCD); [TBD]
}
