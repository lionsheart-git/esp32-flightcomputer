#ifndef FLIGHTCOMPUTER_SENSORDATA_HPP
#define FLIGHTCOMPUTER_SENSORDATA_HPP

#include <stdint.h>
#include <list>

typedef struct Time {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} Time_t;

class SensorData
{
public:
    static void SmartDelay(unsigned long ms);

    //GNSS
    static uint32_t Satellites();
    static int32_t HDOP();
    static double Latitude();
    static double Longitude();

    // Date and time
    Time_t Time();
    uint8_t Hour();
    uint8_t Minute();
    uint8_t Second();
    uint32_t Date();
    uint16_t Year();
    uint8_t Month();
    uint8_t Day();

    // Environment
    double Altitude();
    float Temperature();
    float Humidity();
    float UV();
    float Lux();
};


#endif //FLIGHTCOMPUTER_SENSORDATA_HPP
