
#ifndef ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP
#define ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP

#include <Sensors/SensorData.hpp>
#include "ServoControl.hpp"

/**
 * @enum FlightPhase
 * @brief Current phase of the flight.
 */
enum FlightPhase
{
    Idle, /**< Rocket is waiting to be put in launch detect state */
    LaunchDetection, /**< Launch detect phase */
    PoweredFlight, /**< Motor is burning and accelerating the rocket */
    UnpoweredFlight, /**< Rocket is still ascending but no longer accelerating. Motor burned out */
    Descend, /**< Rocket is descending */
    TouchedDown /**< Rocket landed on the ground */
};

/**
 * @class FlightControl
 * @brief Used to control different phases and events of the flight.
 */
class FlightControl
{
public:
    explicit FlightControl(SensorData &sensors);
    void CheckFlight();

    FlightPhase Phase() const;
    double MaxAltitude() const;

    void OpenRecovery();
    void CloseRecovery();
    void ServoMinPosition(int degrees);
    void ServoMaxPosition(int degrees);


private:
    bool isApogee();
    bool LaunchDetected();
    bool UnpoweredFlight();
    bool TouchedDown();

    SensorData &sensors_;
    FlightPhase phase_;

    double lastAccelerationDifference_;
    double lastAcceleration_;
    double lastAltitude;
    double maxAltitude_;
    uint8_t counter_;

    ServoControl recoveryServo_;
    static double LengthVector(const sensors_vec_t &accelAxis);
};


#endif //ESP32_FLIGHTCOMPUTER_FLIGHTCONTROLL_HPP
