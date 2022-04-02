/**
 * @file BMP388.cpp
 * @copydoc BMP388.hpp
 */

#include "BMP388.hpp"

#include <cmath>

#include "Logging/SystemLogger.hpp"

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

    if (counter_ == 0) {
        GroundLevelPressure = this->pressure;
        counter_++;
        return false;
    }

    // Calculates the average until two values are close enough together.
    //@todo Change to a better average calculation
    GroundLevelPressure = (GroundLevelPressure + this->pressure) / 2;

    // Check before and after difference
    if (abs(this->pressure - GroundLevelPressure) < 0.01f) {
        return true;
    }

    slog_d("Ground level pressure: %.2f, difference: %f", GroundLevelPressure, abs(this->pressure - GroundLevelPressure));
    return false;
}
