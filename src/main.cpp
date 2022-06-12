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

    SDCard::Begin();

    Sensors.Begin();
    Sensors.Calibrate();

    FlightControl fc = FlightControl(Sensors);

    while (true) {
        //    UtilityFunctions::ScanI2CDevice(&Wire1);
        Sensors.UpdateData();

        printf("Time: %s\n", RTC.Timestamp());
        printf("Sats: %lu\n", Sensors.GNSS_Satellites());
        printf("Temperature: %.2f *C\n", Sensors.Temperature());
        printf("Pressure: %0.2f hPa\n", Sensors.Pressure() / 100);
        printf("Approx AltitudeAboveGround: %0.2f m\n", Sensors.AltitudeAboveGround());
        printf("Acceleration X: %f, Y: %f, Z: %f m/s^2\n", Sensors.Acceleration().x, Sensors.Acceleration().y,
               Sensors.Acceleration().z);
        printf("Rotation X: %f, Y: %f, Z: %f\n", Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

        fc.CheckFlight();
        printf("Max Altitude: %.2f m\n", fc.MaxAltitude());
        printf("Phase: %d\n", fc.Phase());

        dlogn("%s;%f;%f;%lu;%lu;%f;%f;%f;%f;%f;%f;%f;%f;%f;", RTC.Timestamp(), Sensors.GNSS_Latitude(),
              Sensors.GNSS_Longitude(),
              Sensors.GNSS_Satellites(), Sensors.GNSS_HDOP(), Sensors.Pressure(), Sensors.Temperature(),
              Sensors.AltitudeAboveGround(), Sensors.Acceleration().x, Sensors.Acceleration().y, Sensors.Acceleration().z,
              Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

        Sensors.SmartDelay(100);
    }
}

void loop()
{
}
