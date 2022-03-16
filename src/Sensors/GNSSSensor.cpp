#include "GNSSSensor.hpp"

#include <Arduino.h>

#include <Sensors/SensorData.hpp>
#include "Logging/SystemLogger.hpp"

GNSSSensor::GNSSSensor(const int8_t RXPin, const int8_t TXPin, uint32_t GPSBaudRate)
: gps_(), softwareSerial_(RXPin, TXPin)
{
    slog_d("Created GNSS Sensor id %d", this->Id());
    softwareSerial_.begin(GPSBaudRate);
    SensorData::RegisterSensor(this);
}

void GNSSSensor::ReadSerial()
{
    while (softwareSerial_.available())
    {
        gps_.encode(softwareSerial_.read());
    }
}

GNSSSensor::~GNSSSensor()
{
    SensorData::DeregisterSensor(this);
}

bool GNSSSensor::IsValid()
{
    return true;
}

