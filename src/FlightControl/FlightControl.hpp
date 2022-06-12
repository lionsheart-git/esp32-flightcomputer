
#ifndef ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP
#define ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP

#include <Sensors/SensorData.hpp>

enum FlightPhase {
    Idle,
    LaunchDetection,
    PoweredFlight,
    UnpoweredFlight,
    Descend,
    TouchedDown
};


class FlightControl
{
public:
    explicit FlightControl(SensorData &sensors);
    FlightPhase Phase();
    void CheckFlight();
    double MaxAltitude();


private:
    bool isApogee();

    SensorData& sensors_;
    FlightPhase phase_;

    double maxAltitude_;
    uint8_t counter_;
};


#endif //ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP
