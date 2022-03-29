#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"
#include "Logging/DataLogger.hpp"
#include "BarometricSensor.hpp"

SensorData::SensorData()
        : gpsSerial_(2)
{
}

/**
 * @brief Starts all sensors.
 */
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

/**
 * @brief Delay that keeps feeding TinyGPS the GNSS data.
 *
 * Should always be used if a GNSS module ist connected.
 *
 * @param ms Milliseconds the delay should be.
 */
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

/**
 * @brief Calibrates all sensors if they need to.
 *
 * @return True, if all sensors are successfully calibrated.
 */
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

/**
 * Updates the data of all sensors to the newest version.
 */
void SensorData::UpdateData()
{
    bmp_.performReading();
}

//@todo Figure out whether to use exceptions or return codes to signal invalid readings

/**
 * @brief Returns the current number of satellites used by the GNSS.
 *
 * @return Number of satellites in use.
 */
uint32_t SensorData::GNSS_Satellites()
{

    if (!gps_.satellites.isValid())
    {
        slog_w("Invalid gps satellites value.");
    }

    return gps_.satellites.value();
}

/**
 * @brief Returns the Horizontal Dilution Of Precision (HDOP).
 *
 * The smaller the better.
 *
 * @return The HDOP of the current GNSS signal.
 */
int32_t SensorData::GNSS_HDOP()
{
    int32_t hdop = 0;

    return hdop;
}

/**
 * @brief Current latitude.
 *
 * @return The current latitude.
 */
double SensorData::GNSS_Latitude()
{
    double latitude = 0;

    return latitude;
}

/**
 * @brief Current longitude.
 *
 * @return The current longitude.
 */
double SensorData::GNSS_Longitude()
{
    double longitude = 0;

    return longitude;
}

/**
 * @brief Returns the pressure of the BMP388.
 *
 * Need to call UpdateData() to update to latest data first.
 *
 * @return Pressure of the BMP388.
 */
double SensorData::Pressure()
{
    return bmp_.pressure;
}

/**
 * @brief Get the temperature of the barometric sensor.
 *
 * Need to call UpdateData() to update to latest data first.
 *
 * @return
 */
float SensorData::Temperature()
{
    return bmp_.temperature;
}

/**
 * @brief Calculates the altitude above ground level in meters.
 *
 * Only works throughout the Troposphere up to 11km.
 * Data first needs to be updated through UpdateData().
 *
 * @return[m] The current altitude above ground.
 */
double SensorData::AltitudeAboveGround()
{
    return BarometricSensor::Altitude(bmp_.pressure, bmp_.GroundLevelPressure);
}

/**
 * @brief Calculates the altitude above sea level in meters.
 *
 * Currently done with standard pressure of 1013.25. Should later be changed
 * when pressure at sea level is calculated at current location.
 *
 * @return[m] The current altitude at sea level at sea levels in m.
 */
double SensorData::AltitudeAboveSeaLevel()
{
    return BarometricSensor::Altitude(bmp_.pressure);
}


SensorData Sensors = SensorData();