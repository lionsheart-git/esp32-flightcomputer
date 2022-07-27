#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"
#include "RealTimeClock.hpp"

#include "FlightControl/FlightControl.hpp"

#include <Utility/UtilityFunctions.hpp>

void setup()
{
    Serial.begin(115200);

    // Init SD Card on correct SPI port.
    Wire.setPins(I2C_SDA1, I2C_SCL1);

    // Start RealTimeClock
    RTC.Begin();

    // Starting SD Card
    SDCard::Begin();

    // Starting and calibrating Sensors
    Sensors.Begin();
    Sensors.Calibrate();

    // Starting flight control
    FlightControl fc = FlightControl(Sensors);

    // Main loop
    while (true)
    {
        //    UtilityFunctions::ScanI2CDevice(&Wire1);
        Sensors.UpdateData();

        printf("Time: %s\n", RTC.Timestamp());
        printf("Sats: %i\n", Sensors.GNSS_Satellites());
        printf("Temperature: %.2f *C\n", Sensors.Temperature());
        printf("Pressure: %0.2f hPa, Filtered pressure: %0.2f hPa\n", Sensors.Pressure() / 100,
               Sensors.FilteredPressure() / 100);
        printf("Approx AltitudeAboveGround: %0.2f m, FilteredAltitudeAboveGround: %0.2f m\n",
               Sensors.AltitudeAboveGround(), Sensors.FilteredAltitudeAboveGround());
        printf("Acceleration X: %f, Y: %f, Z: %f m/s^2\n", Sensors.Acceleration().x, Sensors.Acceleration().y,
               Sensors.Acceleration().z);
        printf("Rotation X: %f, Y: %f, Z: %f\n", Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

        fc.CheckFlight();
        printf("Max Altitude: %.2f m\n", fc.MaxAltitude());
        printf("Phase: %d\n", fc.Phase());

        dlogn("%s;%f;%f;%lu;%lu;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;", RTC.Timestamp(), Sensors.GNSS_Latitude(),
              Sensors.GNSS_Longitude(), Sensors.GNSS_Satellites(), Sensors.GNSS_HDOP(),
              Sensors.Pressure(), Sensors.FilteredPressure(), Sensors.Temperature(),
              Sensors.FilteredAltitudeAboveGround(), Sensors.AltitudeAboveGround(),
              Sensors.Acceleration().x, Sensors.Acceleration().y, Sensors.Acceleration().z,
              Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

        Sensors.SmartDelay(1000);
    }
}

// Not used
void loop()
{
}
