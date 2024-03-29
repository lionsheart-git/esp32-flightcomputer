#ifndef ESP32_FLIGHTCOMPUTER_UTILITYFUNCTIONS_HPP
#define ESP32_FLIGHTCOMPUTER_UTILITYFUNCTIONS_HPP

#include <Arduino.h>

#include <Wire.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

class UtilityFunctions
{
public:
    static void ScanI2CDevice(TwoWire* wireInstance = &Wire);

    static void EnableLoRaSPI();

    static void EnableSDSPI();

    static void DisableLoRaSPI();

    static void DisableSDSPI();
};


#endif //ESP32_FLIGHTCOMPUTER_UTILITYFUNCTIONS_HPP
