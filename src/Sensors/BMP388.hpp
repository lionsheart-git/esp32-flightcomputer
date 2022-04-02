/**
 * @file BMP388.hpp
 * @brief BMP388 definition.
 */
#ifndef ESP32_FLIGHTCOMPUTER_BMP388_HPP
#define ESP32_FLIGHTCOMPUTER_BMP388_HPP

#include "Adafruit_BMP3XX.h"

class BMP388 : public Adafruit_BMP3XX
{
public:
    BMP388();
    bool Calibrate();

public:
    double GroundLevelPressure;

private:
    uint16_t counter_;
};


#endif //ESP32_FLIGHTCOMPUTER_BMP388_HPP
