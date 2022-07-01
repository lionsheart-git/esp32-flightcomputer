
#include <cstdio>
#include "CalibrationSequences.hpp"

void CalibrationSequences::SlidingAverage(sliding_average_t & data, double toAdd)
{
    if (data.first_element)
    {
        data.data = toAdd;
        data.first_element = false;
        return;
    }

    double scalar1 = 1.0 / data.elements_count;
    double scalar2 = 1.0 - scalar1;

    // Calculates the average until.
    data.data = data.data * scalar2 + toAdd * scalar1;
}
