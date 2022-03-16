#ifndef FLIGHTCOMPUTER_GNSSSENSOR_HPP
#define FLIGHTCOMPUTER_GNSSSENSOR_HPP

#include "Sensor.hpp"
#include "ITimeSensor.hpp"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "PinConfiguration.hpp"

class GNSSSensor : public Sensor,
{
public:
    explicit GNSSSensor(int8_t RXPin = GPS_RX, int8_t TXPin = GPS_TX, uint32_t GPSBaudRate = 9600);
    ~GNSSSensor();

    bool IsValid();

    void ReadSerial();

    TinyGPSPlus gps_;

private:
    SoftwareSerial softwareSerial_;

};


#endif //FLIGHTCOMPUTER_GNSSSENSOR_HPP
