
#ifndef FLIGHTCOMPUTER_SENSOR_HPP
#define FLIGHTCOMPUTER_SENSOR_HPP

#include <stdint.h>

class Sensor
{
public:
    Sensor();
    uint16_t Id() const;

private:
    uint16_t const id_; /**< @brief Id of the sensor instance. */
    static uint16_t idCounter; /**< @brief Global id counter. */

};


#endif //FLIGHTCOMPUTER_SENSOR_HPP
