#include "Sensor.hpp"

uint16_t Sensor::idCounter = 0;

/**
 * @brief Creates a new sensor.
 *
 * Increments the global id counter.
 */
Sensor::Sensor()
        : id_(Sensor::idCounter)
{
    Sensor::idCounter++;
}

/**
 * @brief ID of the sensor instance.
 *
 * Counted globally.
 *
 * @return ID of the sensor instance.
 */
uint16_t Sensor::Id() const
{
    return id_;
}


