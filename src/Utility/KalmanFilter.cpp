
#include "KalmanFilter.hpp"

KalmanFilter::KalmanFilter(double varianceMeasurement, double varianceProcess)
        : varianceMeasurement_(varianceMeasurement), varianceProcess_(varianceProcess), pc_(0.0), g_(0.0), p_(1.0),
          xp_(0.0), zp_(0.0), xe_(0.0)
{
}

double KalmanFilter::Update(double measurement)
{
    pc_ = p_ + varianceProcess_;
    g_ = pc_ / (pc_ + varianceMeasurement_);
    p_ = (1 - g_) * pc_;
    xp_ = xe_;
    zp_ = xp_;
    xe_ = g_ * (measurement - zp_) + xp_;

    return xe_;
}

double KalmanFilter::Value() const
{
    return xe_;
}
