
#include "FlightControl.hpp"

#include "Logging/SystemLogger.hpp"

#define ALTITUDE_THRESHOLD 2

/**
 * @brief New FlightControl with sensors to retrieve data from.
 *
 * @param sensors Sensors to retrieve data from.
 */
FlightControl::FlightControl(SensorData &sensors)
: sensors_(sensors), phase_(FlightPhase::Idle), maxAltitude_(0.0), counter_(0), recoveryServo_(SERVO_PWM, 10, 150)
{
    slog_i("Started flight control.");
}

/**
 * @brief Checks the current flight for events.
 *
 * Detects flight phase and apogee.
 */
void FlightControl::CheckFlight()
{

    if (phase_ < FlightPhase::Descend && isApogee()) {
        slog_i("Eject Recovery");
        recoveryServo_.open();

        phase_ = FlightPhase::Descend;
    }
}

/**
 * @brief Detects the apogee of the rocket.
 *
 * Uses the barometric pressure to find the maximum altitude and activate the recovery system on descend.
 *
 * @return If the apogee is detected.
 * @retval True, if the rocket reached apogee.
 * @retval False, if the rocket has not reached the apogee yet.
 */
bool FlightControl::isApogee()
{
    double currentAltitude = sensors_.AltitudeAboveGround();

    if (maxAltitude_ + ALTITUDE_THRESHOLD < currentAltitude) {
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

/**
 * @brief Current phase of flight rocket is in.
 *
 * @return The phase of flight.
 */
FlightPhase FlightControl::Phase()
{
    return phase_;
}

/**
 * @brief Max altitude the rocket has reached.
 *
 * @return Max altitude.
 */
double FlightControl::MaxAltitude()
{
    return maxAltitude_;
}
