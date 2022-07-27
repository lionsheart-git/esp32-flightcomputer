
#ifndef ESP32_FLIGHTCOMPUTER_KALMANFILTER_HPP
#define ESP32_FLIGHTCOMPUTER_KALMANFILTER_HPP

class KalmanFilter
{
public:
    KalmanFilter(double varianceMeasurement, double varianceProcess);

    double Update(double measurement);
    double Value() const;

private:
    double varianceMeasurement_;
    double varianceProcess_;
    double pc_;
    double g_;
    double p_;
    double xp_;
    double zp_;
    double xe_;
};

#endif //ESP32_FLIGHTCOMPUTER_KALMANFILTER_HPP
