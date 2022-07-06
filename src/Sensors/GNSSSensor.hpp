
#ifndef ESP32_FLIGHTCOMPUTER_GNSSSENSOR_HPP
#define ESP32_FLIGHTCOMPUTER_GNSSSENSOR_HPP

#include <HardwareSerial.h>
#include <TinyGPS++.h>

#include "Utility/CalibrationSequences.hpp"

class GNSSSensor : public TinyGPSPlus
{
public:
    GNSSSensor();

    bool Calibrate();
    double GroundLevelAltitude();
    void OnDelay();

private:
    HardwareSerial gpsSerial_;
    sliding_average_t calibration_struct_;

};


#endif //ESP32_FLIGHTCOMPUTER_GNSSSENSOR_HPP
