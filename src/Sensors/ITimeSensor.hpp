#ifndef FLIGHTCOMPUTER_ITIMESENSOR_HPP
#define FLIGHTCOMPUTER_ITIMESENSOR_HPP

#include <stdint.h>

#include "Sensor.hpp"

typedef struct Time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Time_t;

class ITimeSensor : public Sensor
{
public:

    virtual Time_t Time() = 0;
    virtual uint8_t Hour() = 0;
    virtual uint8_t Minute() = 0;
    virtual uint8_t Second() = 0;

};

#endif //FLIGHTCOMPUTER_ITIMESENSOR_HPP
