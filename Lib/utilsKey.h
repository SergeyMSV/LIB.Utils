///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsKey.h
//
// Standard ISO/IEC 114882, C++17
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2019 09 30  |
// |            |               | 
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "utilsBase.h"

#include <cassert>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <string>
#include <vector>

namespace utils
{

namespace type
{

template <unsigned int size>
struct tKey1
{
	enum { Size = size };//Items Qty
	tUInt32 Value[size];

	//tArray1() in union it's deleted by default
	//{
	//	std::memset(Value, 0, Size);
	//}

	tUInt32& operator [] (std::size_t i)
	{
		assert(i < Size);

		return Value[i];
	}

	bool operator == (const tKey1& value)
	{
		return std::memcmp(Value, value.Value, Size * 4) == 0;
	}

	bool operator != (const tKey1& value)
	{
		return std::memcmp(Value, value.Value, Size * 4) != 0;
	}
};

template <unsigned int size>
struct tKey2 : public tKey1<size>
{
	tKey2()
	{
		std::memset(this->Value, 0, sizeof(this->Value));
	}
};

}

union tKey256
{
	typedef type::tKey1<8> tKeyValue;

	enum { type_key = true };

	struct
	{
		tUInt32 A;
		tUInt32 B;
		tUInt32 C;
		tUInt32 D;
		tUInt32 E;
		tUInt32 F;
		tUInt32 G;
		tUInt32 H;
	}Field;

	tKeyValue Value;
};

union tKey192
{
	typedef type::tKey1<6> tKeyValue;

	enum { type_key = true };

	struct
	{
		tUInt32 A;
		tUInt32 B;
		tUInt32 C;
		tUInt32 D;
		tUInt32 E;
		tUInt32 F;
	}Field;

	tKeyValue Value;
};

union tKey128
{
	typedef type::tKey1<4> tKeyValue;

	enum { type_key = true };

	struct
	{
		tUInt32 A;
		tUInt32 B;
		tUInt32 C;
		tUInt32 D;
	}Field;

	tKeyValue Value;
};

union tKey96
{
	typedef type::tKey1<3> tKeyValue;

	enum { type_key = true };

	struct
	{
		tUInt32 A;
		tUInt32 B;
		tUInt32 C;
	}Field;

	tKeyValue Value;
};

union tKey64
{
	typedef type::tKey1<2> tKeyValue;

	enum { type_key = true };

	struct
	{
		tUInt32 A;
		tUInt32 B;
	}Field;

	tKeyValue Value;
};

union tKey32
{
	typedef type::tKey1<1> tKeyValue;

	enum { type_key = true , };

	struct
	{
		tUInt32 A;
	}Field;

	tKeyValue Value;
};

template <class T>
typename std::enable_if<std::is_union<T>::value && T::type_key, bool>::type operator == (const T& val1, const T& val2)
{
	return std::memcmp(val1.Value.Value, val2.Value.Value, T::tKeyValue::Size) == 0;
}

template <class T>
typename std::enable_if<std::is_union<T>::value && T::type_key, bool>::type operator != (const T& val1, const T& val2)
{
	return std::memcmp(val1.Value.Value, val2.Value.Value, T::tKeyValue::Size) != 0;
}

template <class T>
typename std::enable_if<T::type_key, std::string>::type ToString(const T& value)
{
	const std::size_t Size = T::tKeyValue::Size;

	std::string LocalString;

	LocalString.reserve(Size * 9);

	for (int i = Size; i > 0; --i)
	{
		char Str[20];

		std::sprintf(Str, "%08X", value.Value.Value[i - 1]);

		LocalString += Str;

		if (i != 1)
		{
			LocalString += '-';
		}
	}

	return LocalString;
}

}
