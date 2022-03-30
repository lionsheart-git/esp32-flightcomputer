
#ifndef ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP
#define ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP


class BarometricSensor
{
public:
    static double Altitude(double currentPressure, double originPressure = 101325.0f);

    static double PressureAtSeaLevel(double pressure, double altitude);
};


#endif //ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP
