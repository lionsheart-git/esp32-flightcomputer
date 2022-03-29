
#ifndef ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP
#define ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP


class BarometricSensor
{
public:
    static double Altitude(double pressure, double seaLevel = 1013.25f);

    static double PressureAtSeaLevel(double pressure, double altitude);
};


#endif //ESP32_FLIGHTCOMPUTER_BAROMETRICSENSOR_HPP
