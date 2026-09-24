#include "utilsRand.h"

#include <iomanip>
#include <random>
#include <sstream>

#include <cstdint>

namespace utils
{
namespace rand
{

std::string MakeClientID(int a_size)
{
	std::mt19937_64 Rng(std::random_device{}()); // To ensure stronger cryptographic resilience, an OS-derived source is recommended.
	std::uniform_int_distribution<std::uint32_t> Dist(0, 255);
	std::ostringstream SStr;
	SStr << std::hex << std::uppercase << std::setfill('0');
	for (int i = 0; i < a_size; ++i)
		SStr << std::setw(2) << static_cast<int>(Dist(Rng));
	return SStr.str();
}

}
}
