
#include "GNSSSensor.hpp"

#include "PinConfiguration.hpp"

GNSSSensor::GNSSSensor()
        : gpsSerial_(2)
{
    calibration_struct_.elements_count = 100;

    // Initialize GPS serial
    gpsSerial_.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

bool GNSSSensor::Calibrate()
{
    if (!this->altitude.isValid()) {
        return false;
    }

    return true;
}

double GNSSSensor::GroundLevelAltitude()
{
    return calibration_struct_.data;
}

void GNSSSensor::OnDelay()
{
    while (gpsSerial_.available())
    {
        this->encode(gpsSerial_.read());
    }
}
