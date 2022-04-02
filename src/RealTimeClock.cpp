/**
 * @file RealTimeClock.cpp
 * @copydoc RealTimeClock.hpp
 */
#include "RealTimeClock.hpp"

#include "Logging/SystemLogger.hpp"

RealTimeClock RTC = RealTimeClock();

RealTimeClock::RealTimeClock()
        : RTC_DS3231()
{

}

void RealTimeClock::Begin()
{
    if (!RTC.begin())
    {
        slog_e("Couldn't find RTC");
    }

    if (RTC.lostPower())
    {
        slog_w("RTC lost power, setting time to compile time.");
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

const char *RealTimeClock::Timestamp()
{
    return RTC.now().timestamp().c_str();
}
