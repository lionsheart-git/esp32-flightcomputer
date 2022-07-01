/**
 * @file BMP388.hpp
 * @brief BMP388 definition.
 */
#ifndef ESP32_FLIGHTCOMPUTER_BMP388_HPP
#define ESP32_FLIGHTCOMPUTER_BMP388_HPP

#include "Adafruit_BMP3XX.h"
#include "Utility/CalibrationSequences.hpp"

class BMP388 : public Adafruit_BMP3XX
{
public:
    BMP388();
    bool Calibrate();
    double GroundLevelPressure() const;

private:
    sliding_average_t GroundLevelPressure_;
};


#endif //ESP32_FLIGHTCOMPUTER_BMP388_HPP
