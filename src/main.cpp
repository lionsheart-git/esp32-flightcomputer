#include <Arduino.h>

#include <sys/time.h>


#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"

SensorData sensorData_;

void setup()
{
    Serial.begin(115200);

    // Init SD Card on correct SPI port.
    SDCard::Begin();
}

void loop()
{
    dlogn("Sats: %d", sensorData_.Satellites());

    dlogn("RTC Time: %s.", sensorData_.Time().timestamp().c_str());

    sensorData_.SmartDelay(1000);

}
