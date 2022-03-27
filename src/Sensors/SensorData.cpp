#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"

SensorData::SensorData()
        : gpsSerial_(2)
{
}

void SensorData::Begin()
{
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

    SmartDelay(5000);
    if (gps_.charsProcessed() < 10)
    {
        slog_e("No GPS data received: check wiring");
    }
}

void SensorData::SmartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (gpsSerial_.available())
        {
            SensorData::gps_.encode(gpsSerial_.read());
        }
    } while (millis() - start < ms);
}

//@todo Figure out whether to use exceptions or return codes to signal invalid readings
uint32_t SensorData::GNSS_Satellites()
{

    if (!gps_.satellites.isValid())
    {
        slog_w("Invalid gps satellites value.");
    }

    return gps_.satellites.value();
}

int32_t SensorData::GNSS_HDOP()
{
    int32_t hdop = 0;

    return hdop;
}

double SensorData::GNSS_Latitude()
{
    double latitude = 0;

    return latitude;
}

double SensorData::GNSS_Longitude()
{
    double longitude = 0;

    return longitude;
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

bool SensorData::Calibrate()
{
    slog_i("Starting sensor calibration");
    unsigned long start = millis();
    bool success = false;
    while (!success)
    {
        if (bmp_.Calibrate())
        {
            success = true;
        }
        SmartDelay(100);
    }

    slog_i("Ground level pressure: %f Pa", bmp_.GroundLevelPressure);

    slog_i("Sensor calibration took: %ld", millis() - start);
    return true;
}

void SensorData::UpdateData()
{
    bmp_.performReading();
}

SensorData Sensors = SensorData();