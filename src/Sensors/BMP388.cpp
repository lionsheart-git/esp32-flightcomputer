/**
 * @file BMP388.cpp
 * @copydoc BMP388.hpp
 */

#include "BMP388.hpp"

#include <cmath>

#include "Logging/SystemLogger.hpp"
#include "Utility/CalibrationSequences.hpp"

#define SLIDING_AVERAGE_ELEMENTS 100

BMP388::BMP388()
        : Adafruit_BMP3XX()
{
    this->GroundLevelPressure_.elements_count = 100;
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

    // Initial pressure so difference check does not succeed.
    if (GroundLevelPressure_.first_element)
    {
        CalibrationSequences::SlidingAverage(GroundLevelPressure_, this->pressure);
        return false;
    }

    // Sliding average over all pressure values.
    CalibrationSequences::SlidingAverage(GroundLevelPressure_, this->pressure);

    // Check before and after difference
    if (abs(this->pressure - GroundLevelPressure_.data) < 1.0f)
    {
        return true;
    }

    slog_d("Pressure: %.2f, Ground level pressure: %.2f, difference: %f", this->pressure, GroundLevelPressure_.data,
           abs(this->pressure - GroundLevelPressure_.data));
    return false;
}

double BMP388::GroundLevelPressure() const
{
    return GroundLevelPressure_.data;
}
