#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

#include "PinConfiguration.hpp"
#include "SDCard.hpp"

#include "Sensors/SensorData.hpp"
#include "RealTimeClock.hpp"

#include "FlightControl/FlightControl.hpp"
#include "Telemetry/LoRaCommunication.hpp"

#include <Utility/UtilityFunctions.hpp>

#define  RADIOLIB_DEBUG

void setup()
{
    Serial.begin(115200);

    // Init SD Card on correct SPI port.
    Wire.setPins(I2C_SDA1, I2C_SCL1);

    // Start RealTimeClock
    RTC.Begin();

    // SPI.begin(SD_SCK, SD_MISO, SD_MOSI);

    SPIClass sdSpi = SPIClass(VSPI);
    sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    pinMode(sdSpi.pinSS(), OUTPUT);

    // Starting SD Card
    SDCard::Begin(sdSpi);

    // Starting sensors
    Sensors.Begin();

//    SPIClass loraSpi(HSPI);
//    loraSpi.setClockDivider(32);
//    loraSpi.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_CS);
//    pinMode(sdSpi.pinSS(), OUTPUT);
//    LoRaCommunication lora = LoRaCommunication(SPI);

    // Starting flight control
    FlightControl fc = FlightControl(Sensors);

    bool readyForStart = false;
    String input;
    String substring;

    while (!readyForStart)
    {
        if (Serial.available())
        {
            input = Serial.readStringUntil('\n');
            Serial.printf("Gotten: %s\n", input.c_str());

            int index = input.indexOf(' ');
            substring = input.substring(0, index);

            Serial.printf("Tokenized: %s\n", substring.c_str());

            if (substring == "FC+START")
            {
                readyForStart = true;
                break;
            }
            else if (substring == "SV+OPEN")
            {
                fc.OpenRecovery();
            }
            else if (substring == "SV+CLOSE")
            {
                fc.CloseRecovery();
            }
            else if (substring == "SV+MAX")
            {
                substring = input.substring(index + 1);
                int degrees = substring.toInt();
                fc.ServoMaxPosition(degrees);
            }
            else if (substring == "SV+MIN")
            {
                substring = input.substring(index + 1);
                int degrees = substring.toInt();
                fc.ServoMinPosition(degrees);
            } else if (substring == "SYS+RESET") {
                ESP.restart();
            }
            else {
                Serial.println("Could not match input.");
            }
        }
        else {
            Serial.println("No input.");
        }
        delay(1000);
    }

    // Calibrate sensors
    Sensors.Calibrate();
//
    SPIClass loraSpi(HSPI);
    loraSpi.setClockDivider(32);
    loraSpi.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_CS);
    pinMode(sdSpi.pinSS(), OUTPUT);
    LoRaCommunication lora = LoRaCommunication(SPI);

    // Starting flight control
    FlightControl fc = FlightControl(Sensors);

    char message[250];

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

        dlogn("%s;%f;%f;%u;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;", RTC.Timestamp(), Sensors.GNSS_Latitude(),
              Sensors.GNSS_Longitude(), Sensors.GNSS_Satellites(), Sensors.GNSS_HDOP(),
              Sensors.Pressure(), Sensors.FilteredPressure(), Sensors.Temperature(),
              Sensors.FilteredAltitudeAboveGround(), Sensors.AltitudeAboveGround(),
              Sensors.Acceleration().x, Sensors.Acceleration().y, Sensors.Acceleration().z,
              Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

        snprintf(message, sizeof(message), "%s;%f;%f;%u;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;", RTC.Timestamp(), Sensors.GNSS_Latitude(),
                 Sensors.GNSS_Longitude(), Sensors.GNSS_Satellites(), Sensors.GNSS_HDOP(),
                 Sensors.Pressure(), Sensors.FilteredPressure(), Sensors.Temperature(),
                 Sensors.FilteredAltitudeAboveGround(), Sensors.AltitudeAboveGround(),
                 Sensors.Acceleration().x, Sensors.Acceleration().y, Sensors.Acceleration().z,
                 Sensors.Gyro().x, Sensors.Gyro().y, Sensors.Gyro().z);

         lora.transmit(message);

        Sensors.SmartDelay(1000);
    }
}

// Not used
void loop()
{
}
