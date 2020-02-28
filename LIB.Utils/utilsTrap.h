///////////////////////////////////////////////////////////////////////////////////////////////////
// utilsTrap.h
//
// Standard ISO/IEC 114882, C++14
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 03 28  |
// |      2     |   2017 04 28  | Corrected tMessageTrap::Try(...)
// |      3     |   2020 01 08  | Refactored
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

#include <cstddef>

#include <vector>

namespace utils
{
	namespace trap
	{

template <const char * Data, std::size_t DataSize, bool DataReverse = false>
class tTrap
{
	std::size_t m_DataIndex = 0;

public:
	bool Try(std::vector<char>& data)
	{
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			if (DataReverse)
			{
				if (Data[DataSize - 1 - m_DataIndex++] == data[i])
				{
					if (m_DataIndex == DataSize)
					{
						m_DataIndex = 0;

						return true;
					}
				}
				else if (Data[DataSize - 1] == data[i])
				{
					m_DataIndex = 1;
				}
				else
				{
					m_DataIndex = 0;
				}
			}
			else
			{
				if (Data[m_DataIndex++] == data[i])
				{
					if (m_DataIndex == DataSize)
					{
						m_DataIndex = 0;

						return true;
					}
				}
				else if (Data[0] == data[i])
				{
					m_DataIndex = 1;
				}
				else
				{
					m_DataIndex = 0;
				}
			}
		}

		return false;
	}
};

	}
}
