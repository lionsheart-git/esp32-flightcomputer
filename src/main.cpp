#include <Arduino.h>

#include <sys/time.h>


#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

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

    byte error, address;
    int nDevices;
    dlogn("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            dlog("I2C device found at address 0x");
            if (address < 16)
            {
                dlogs("0");
            }
            dlogsn("%x", address);
            nDevices++;
        } else if (error == 4)
        {
            dlog("Unknow error at address 0x");
            if (address < 16)
            {
                dlogs("0");
            }
            dlogsn("%x", address);
        }
    }
    if (nDevices == 0)
    {
        dlogn("No I2C devices found");
    } else
    {
        dlogn("done");
    }

    dlogn("%s", Sensors.Timestamp());


    dlogn("Temperature: %.2f *C", Sensors.Temperature());
    dlogn("Pressure: %0.2f hPa", Sensors.Pressure() / 100);
    dlogn("Approx Altitude: %0.2f m", Sensors.Altitude());

    Sensors.SmartDelay(1000);

}
