/**
 * @file RealTimeClock.hpp
 * @brief Access to DS3231 as clock.
 */
#ifndef ESP32_FLIGHTCOMPUTER_REALTIMECLOCK_HPP
#define ESP32_FLIGHTCOMPUTER_REALTIMECLOCK_HPP

#include <RTClib.h>

class RealTimeClock : public RTC_DS3231 {
public:
    RealTimeClock();
    void Begin();
    const char * Timestamp();
};

extern RealTimeClock RTC;


#endif //ESP32_FLIGHTCOMPUTER_REALTIMECLOCK_HPP
