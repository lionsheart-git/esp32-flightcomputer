#include <Arduino.h>

#include <sys/time.h>


#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "Utility/UtilityFunctions.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"

void setup()
{
    Serial.begin(115200);
    Sensors.Begin();

#ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

    // Init SD Card on correct SPI port.
    SDCard::Begin();
}

void loop()
{
    dlogn("Sats: %d", Sensors.Satellites());

    UtilityFunctions::ScanI2CDevice();

    dlogn("%s", Sensors.Timestamp());

    dlogn("Temperature: %.2f *C", Sensors.Temperature());
    dlogn("Pressure: %0.2f hPa", Sensors.Pressure() / 100);
    dlogn("Approx Altitude: %0.2f m", Sensors.Altitude());

    Sensors.SmartDelay(1000);

}
