
#include "FlightControl.hpp"

#include "Logging/SystemLogger.hpp"

#define ALTITUDE_THRESHOLD 2

FlightControl::FlightControl(SensorData &sensors)
: sensors_(sensors), state_(FlightState::Idle), maxAltitude_(0.0), counter_(0)
{
    slog_i("Started flight control.");
}

FlightState FlightControl::State()
{
    return state_;
}

void FlightControl::checkFlight()
{
    if (isApogee()) {
        slog_i("Eject Recovery");
        state_ = FlightState::Descend;
    }
}

bool FlightControl::isApogee()
{
    double currentAltitude = sensors_.AltitudeAboveGround();

    if (maxAltitude_ < currentAltitude + ALTITUDE_THRESHOLD) {
        maxAltitude_ = currentAltitude;
        counter_ = 0;
    } else if (currentAltitude + ALTITUDE_THRESHOLD < maxAltitude_ && counter_ > 100) {
        slog_i("Apogee detected!");
        return true;
    } else if (currentAltitude + ALTITUDE_THRESHOLD < maxAltitude_) {
        counter_++;
    }
    return false;
}

double FlightControl::MaxAltitude()
{
    return maxAltitude_;
}
