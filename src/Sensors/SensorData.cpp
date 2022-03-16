#include "SensorData.hpp"

#include <Arduino.h>

#include "Logging/SystemLogger.hpp"

std::list<GNSSSensor *> SensorData::gnssSensors_;

void SensorData::SmartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        // Call ReadSerial for all GPS objects
    } while (millis() - start < ms);
}
//@todo Figure out whether to use exceptions or return codes to signal invalid readings
uint32_t SensorData::Satellites()
{
    uint32_t satellites = 0;
    for (auto & sensor : gnssSensors_) {
        if (sensor->gps_.satellites.isValid()) {
            satellites += sensor->gps_.satellites.value();
        }
    }

    return satellites / gnssSensors_.size();
}

int32_t SensorData::HDOP()
{
    int32_t hdop = 0;
    for (auto & sensor : gnssSensors_) {
        if (sensor->gps_.hdop.isValid()) {
            hdop += sensor->gps_.hdop.value();
        }
    }

    return hdop / gnssSensors_.size();
}

double SensorData::Latitude()
{
    double latitude = 0;
    for (auto & sensor : gnssSensors_) {
        if (sensor->gps_.location.isValid()) {
            latitude += sensor->gps_.location.lat();
        }
    }

    return latitude / gnssSensors_.size();
}

double SensorData::Longitude()
{
    double longitude = 0;
    for (auto & sensor : gnssSensors_) {
        if (sensor->gps_.location.isValid()) {
            longitude += sensor->gps_.location.lng();
        }
    }

    return longitude / gnssSensors_.size();
}

uint32_t SensorData::Time()
{
    return 0;
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

bool SensorData::RegisterSensor(GNSSSensor * sensor)
{
    SensorData::gnssSensors_.push_back(sensor);
    slog_d("Registered GNSS Sensor id %d", sensor->Id());
    return true;
}

bool SensorData::DeregisterSensor(GNSSSensor *sensor)
{
    SensorData::gnssSensors_.remove(sensor);
    slog_d("Deregistered GNSS Sensor id %d", sensor->Id());
    return false;
}


