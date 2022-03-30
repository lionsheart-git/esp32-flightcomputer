#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"
#include "RealTimeClock.hpp"

void setup()
{
    Serial.begin(115200);
    // Init SD Card on correct SPI port.
    Wire.setPins(I2C_SDA1, I2C_SCL1);

    // Start RealTimeClock
    RTC.Begin();

    SDCard::Begin();

    Sensors.Begin();
    Sensors.Calibrate();
}

void loop()
{
    Sensors.UpdateData();

    printf("Time: %s\n", RTC.Timestamp());
    printf("Sats: %lu\n", Sensors.GNSS_Satellites());
    printf("Latitude: %f\n", Sensors.GNSS_Latitude());
    printf("Temperature: %.2f *C\n", Sensors.Temperature());
    printf("Pressure: %0.2f hPa\n", Sensors.Pressure() / 100);
    printf("Approx AltitudeAboveGround: %0.2f m\n", Sensors.AltitudeAboveGround());
    dlogn("%s;%f;%f;%lu;%lu;%f;%f;%f;", RTC.Timestamp(), Sensors.GNSS_Latitude(), Sensors.GNSS_Longitude(),
          Sensors.GNSS_Satellites(), Sensors.GNSS_HDOP(), Sensors.Pressure(), Sensors.Temperature(),
          Sensors.AltitudeAboveGround());

    Sensors.SmartDelay(1000);

}
