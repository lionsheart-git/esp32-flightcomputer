
#include "FlightControl.hpp"

#include "Logging/SystemLogger.hpp"
#include "Constants.hpp"

#define ALTITUDE_THRESHOLD 5
#define LAUNCH_DETECT_ALTITUDE 24
#define LAUNCH_DETECT_ACCELERATION 30

/**
 * @brief New FlightControl with sensors to retrieve data from.
 *
 * @param sensors Sensors to retrieve data from.
 */
FlightControl::FlightControl(SensorData &sensors)
        : sensors_(sensors), phase_(FlightPhase::Idle), maxAltitude_(0.0), counter_(0),
          recoveryServo_(SERVO_PWM, 70, 180)
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
    if (phase_ < FlightPhase::PoweredFlight && LaunchDetected())
    {
        phase_ = FlightPhase::PoweredFlight;
    }

//    if (phase_ == FlightPhase::PoweredFlight && UnpoweredFlight()) {
//        phase_ = FlightPhase::UnpoweredFlight;
//    }

    if (phase_ < FlightPhase::Descend && isApogee())
    {
        slog_i("Eject Recovery");
        recoveryServo_.open();

        phase_ = FlightPhase::Descend;
    }

    if (phase_ >= FlightPhase::Descend && TouchedDown())
    {
        phase_ = FlightPhase::TouchedDown;
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
    double currentAltitude = sensors_.FilteredAltitudeAboveGround();

    if (maxAltitude_ + ALTITUDE_THRESHOLD < currentAltitude)
    {
        maxAltitude_ = currentAltitude;
        counter_ = 0;
    } else if (currentAltitude + ALTITUDE_THRESHOLD < maxAltitude_ && counter_ > LOGGING_FREQUENCY_HZ)
    {
        slog_i("Apogee detected!");
        counter_ = 0;
        return true;
    } else if (currentAltitude + ALTITUDE_THRESHOLD < maxAltitude_)
    {
        counter_++;
    }
    return false;
}

/**
 * @brief Current phase of flight rocket is in.
 *
 * @return The phase of flight.
 */
FlightPhase FlightControl::Phase() const
{
    return phase_;
}

/**
 * @brief Max altitude the rocket has reached.
 *
 * @return Max altitude.
 */
double FlightControl::MaxAltitude() const
{
    return maxAltitude_;
}

/**
 * @brief Detects launch by acceleration and height.
 *
 * @return True, if a launch happened.
 */
bool FlightControl::LaunchDetected()
{
    double currentAltitude = sensors_.FilteredAltitudeAboveGround();

    if (currentAltitude > LAUNCH_DETECT_ALTITUDE)
    {
        slog_i("Launch detected with altitude: %.2f", currentAltitude);
        return true;
    }

    sensors_vec_t accelAxis = sensors_.Acceleration();

    double acceleration = LengthVector(accelAxis);

    if (acceleration > LAUNCH_DETECT_ACCELERATION)
    {
        if (counter_ < LOGGING_FREQUENCY_HZ)
        {
            counter_++;
            return false;
        }

        counter_ = 0;
        slog_i("Launch detected with acceleration: %.2f", acceleration);
        return true;
    }

    return false;
}

double FlightControl::LengthVector(const sensors_vec_t &accelAxis)
{
    return sqrt(std::pow(accelAxis.x, 2) + std::pow(accelAxis.y, 2) + std::pow(accelAxis.z, 2));
}

/**
 * @brief Detects the touch down at the end of descend.
 *
 * @return True, if touch down detected.
 */
bool FlightControl::TouchedDown()
{
    double currentAltitude = sensors_.FilteredAltitudeAboveGround();

    double altitudeDifference = std::abs(lastAltitude - currentAltitude);

    if (lastAltitude == 0)
    {
        lastAltitude = currentAltitude;
    } else if (altitudeDifference < ALTITUDE_THRESHOLD && counter_ > LOGGING_FREQUENCY_HZ)
    {
        lastAltitude = currentAltitude;
        counter_ = 0;
        return true;
    } else if (altitudeDifference < ALTITUDE_THRESHOLD)
    {
        lastAltitude = currentAltitude;
        counter_++;
    }
    return false;
}

/**
 * @brief Checks if the motor burned out.
 * @todo Find correct calculation.
 *
 * @return True, if the motor burned out.
 */
bool FlightControl::UnpoweredFlight()
{
    sensors_vec_t accelAxis = sensors_.Acceleration();

    double acceleration = LengthVector(accelAxis);

    double accelerationDifference = acceleration - lastAcceleration_;

    if (lastAcceleration_ == 0 && lastAccelerationDifference_ == 0)
    {
        lastAcceleration_ = acceleration;
        lastAccelerationDifference_ = accelerationDifference;
    } else if (accelerationDifference < accelerationDifference && counter_ > LOGGING_FREQUENCY_HZ)
    {
        lastAcceleration_ = acceleration;
        lastAccelerationDifference_ = accelerationDifference;
        counter_ = 0;
        return true;
    } else if (accelerationDifference < accelerationDifference)
    {
        lastAcceleration_ = acceleration;
        lastAccelerationDifference_ = accelerationDifference;
        counter_++;
    }
    return false;
}

void FlightControl::OpenRecovery()
{
    recoveryServo_.open();
}

void FlightControl::CloseRecovery()
{
    recoveryServo_.close();
}

void FlightControl::ServoMinPosition(int degrees)
{
    recoveryServo_.SetMinPosition(degrees);
}

void FlightControl::ServoMaxPosition(int degrees)
{
    recoveryServo_.SetMaxPosition(degrees);
}
