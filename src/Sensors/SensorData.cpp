#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

SensorData::SensorData()
: gpsSerial_(2), i2c_(2)
{
    // Initialize i2c connection
    if (!Wire.begin(I2C_SDA, I2C_SCL)) {
        slog_e("Error initializing I2C connection on SDA %d SCL %d. Check wiring.", I2C_SDA, I2C_SCL);
    }

    // Initialize real time clock
    if(!rtc_.begin()) {
        slog_e("Error initializing RTC.");
    }

    if (rtc_.lostPower()) {
        slog_w("RTC lost power, setting time to compile time.");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc_.adjust(DateTime(__DATE__, __TIME__));
    }

    // Initialize Bmp388
    if (!bmp_.begin_I2C()) {
        slog_e("Error initializing BMP388. ADDR: 0x77.");
    }

    // Initialize GPS serial
    gpsSerial_.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    SmartDelay(100);
}

void SensorData::SmartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (gpsSerial_.available())
        {
            gps_.encode(gpsSerial_.read());
        }
    } while (millis() - start < ms);

}

//@todo Figure out whether to use exceptions or return codes to signal invalid readings
uint32_t SensorData::Satellites()
{

    if (!gps_.satellites.isValid()) {
        slog_w("Invalid gps satellites value.");
    }

    return gps_.satellites.value();
}

int32_t SensorData::HDOP()
{
    int32_t hdop = 0;

    return hdop;
}

double SensorData::Latitude()
{
    double latitude = 0;

    return latitude;
}

double SensorData::Longitude()
{
    double longitude = 0;

    return longitude;
}

DateTime SensorData::Time()
{
    return rtc_.now();
}

uint8_t SensorData::Hour()
{
    return 0;
}

uint8_t SensorData::Minute()
{
    return 0;
}

uint8_t SensorData::Second()
{
    return 0;
}

uint32_t SensorData::Date()
{
    return 0;
}

uint16_t SensorData::Year()
{
    return 0;
}

uint8_t SensorData::Month()
{
    return 0;
}

uint8_t SensorData::Day()
{
    return 0;
}

double SensorData::Altitude()
{
    return 0;
}

float SensorData::Temperature()
{
    return 0;
}

float SensorData::Humidity()
{
    return 0;
}

float SensorData::UV()
{
    return 0;
}

float SensorData::Lux()
{
    return 0;
}
