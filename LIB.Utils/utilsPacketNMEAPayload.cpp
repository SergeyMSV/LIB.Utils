#include "utilsPacketNMEAPayload.h"

#include <cmath>

namespace utils
{
namespace packet
{
namespace nmea
{
namespace wnro
{
// How It Works
// • 10-bit counter: Legacy GPS NAV (L1) messages store the week number in 10 binary digits (0–1023).
// • Integer overflow: After week 1023, the counter wraps back to 0.
// • Modern signals: Newer CNAV civilian signals use a 13-bit counter (0–8191), rolling over roughly every 157 years.
// Key Dates
// • First rollover: August 21–22, 1999.
// • Second rollover: April 6–7, 2019.
// • Next 10-bit rollover: Around November 2038 (1024 weeks after April 2019).
static std::chrono::weeks GetWnroShift(std::chrono::year_month_day a_dateGPS, std::chrono::year_month_day a_dateNow)
{
    if (!a_dateGPS.ok() || !a_dateNow.ok())
        return {};
    const auto diff = std::chrono::sys_days(a_dateNow) - std::chrono::sys_days(a_dateGPS); // duration<days>
    const int n = static_cast<int>(std::lround(diff.count() / 7168.0)); // Find the nearest number of rollover cycles (1 cycle = 1024 weeks = 7168 days)
	// Check if the date aligns closely with a rollover boundary (tolerance: ±7 days)
	// This prevents applying correction to default/invalid dates like 1980-01-06
	if (std::abs(diff.count() - n * 7168) > 7)
		return {};
    return std::chrono::weeks(n * 1024);
}

std::chrono::year_month_day GetCorrectedDateWNRO(std::time_t a_tm, std::chrono::year_month_day a_dateNow)
{
	auto tp = std::chrono::system_clock::from_time_t(a_tm); // system_clock::time_point
	const auto tp_ymd = std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(tp) };
	const std::chrono::weeks wnroShift = wnro::GetWnroShift(tp_ymd, a_dateNow);
	tp += wnroShift;
	return std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(tp) };
}

}
}
}
}
