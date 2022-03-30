#include "BarometricSensor.hpp"

#include <math.h>

/**
 * @brief Calculates current altitude from pressure and pressure at sea level.
 *
 * Can only be used throughout the Troposphere (up to 11km).
 *
 * @param pressure
 * @param seaLevel
 * @return
 */
double BarometricSensor::Altitude(double currentPressure, double originPressure)
{
    double atmosphericPressure = currentPressure / 100.0f;
    double atmosphericSeaLevel = originPressure / 100.0f;
    return 44330.0f * (1.0 - pow(atmosphericPressure / atmosphericSeaLevel, 0.190223f));

    // SparkFun: altitude = ((float)powf(sea_level_pressure / currentPressure, 0.190223f) - 1.0f) * (temperature + 273.15f) / 0.0065f;
}

/**
 * @brief Calculates the seaLevel pressure from altitude and current pressure.
 *
 * Taken from datasheet of BMP085 https://cdn-shop.adafruit.com/datasheets/BST-BMP085-DS000-06.pdf.
 *
 * @param pressure Pressure at current altitude.
 * @param altitude Current altitude.
 * @return Pressure as it would be at sea level.
 */
double BarometricSensor::PressureAtSeaLevel(double pressure, double altitude)
{
    return pressure / pow(1.0f - (altitude / 44330.0f), 0.190223f);
}


