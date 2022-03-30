#include "BMP388.hpp"

#include <cmath>

#include "Constants.hpp"
#include "Logging/SystemLogger.hpp"

BMP388::BMP388()
        : Adafruit_BMP3XX(), GroundLevelPressure(0.0), counter_(0)
{
}

bool BMP388::Calibrate()
{
    // Read latest data
    this->performReading();

    // Calculate average of value before and current
    double temp = GroundLevelPressure / counter_;
    GroundLevelPressure += pressure;

    counter_++;
    if (abs((GroundLevelPressure / counter_) - temp) < 0.01f) {
        GroundLevelPressure /= counter_;
        return true;
    }
    slog_d("Ground level pressure: %.2f, difference: %f", GroundLevelPressure / counter_, abs((GroundLevelPressure / counter_) - temp));
    return false;
}
