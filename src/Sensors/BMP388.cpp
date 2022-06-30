/**
 * @file BMP388.cpp
 * @copydoc BMP388.hpp
 */

#include "BMP388.hpp"

#include <cmath>

#include "Logging/SystemLogger.hpp"

#define SLIDING_AVERAGE_ELEMENTS 100

BMP388::BMP388()
        : Adafruit_BMP3XX(), GroundLevelPressure(0.0), counter_(0)
{
}

/**
 * @brief Calibrates the ground level.
 *
 * Takes the average of two readings.
 *
 * @return True, if the ground level pressure is found.
 */
bool BMP388::Calibrate()
{
    // Read latest data
    this->performReading();


    double scalar1 = 1.0 / SLIDING_AVERAGE_ELEMENTS;
    double scalar2 = 1.0 - scalar1;

    if (counter_ == 0)
    {
        GroundLevelPressure = this->pressure;
        counter_++;
        return false;
    }

    // Calculates the average until two values are close enough together.
    GroundLevelPressure = GroundLevelPressure * scalar2 + this->pressure * scalar1;


    // Check before and after difference
    if (abs(this->pressure - GroundLevelPressure) < 0.01f)
    {
        return true;
    }

    slog_d("Pressure: %.2f, Ground level pressure: %.2f, difference: %f", this->pressure, GroundLevelPressure,
           abs(this->pressure - GroundLevelPressure));
    return false;
}
