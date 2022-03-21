#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

SensorData::SensorData()
        : gpsSerial_(2)
{}

void SensorData::Begin()
{
    Wire.setPins(I2C_SDA, I2C_SCL);

    if (!rtc_.begin())
    {
        slog_e("Couldn't find RTC");
    }

    if (rtc_.lostPower())
    {
        slog_w("RTC lost power, setting time to compile time.");
        rtc_.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    if (!bmp_.begin_I2C())
    {
        slog_e("Error initializing BMP388 via I2C. ADDR: 0x77.");
    }

    // Set up oversampling and filter initialization
    bmp_.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp_.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp_.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp_.setOutputDataRate(BMP3_ODR_50_HZ);

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

    if (!gps_.satellites.isValid())
    {
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

const char *SensorData::Timestamp()
{
    return rtc_.now().timestamp().c_str();
}

double SensorData::Pressure()
{
    return bmp_.readPressure();
}

double SensorData::Altitude()
{
    return bmp_.readAltitude(1032);
}

float SensorData::Temperature()
{
    return bmp_.readTemperature();
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

SensorData Sensors = SensorData();