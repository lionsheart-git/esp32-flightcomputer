
#include <cstdio>
#include "CalibrationSequences.hpp"

void CalibrationSequences::SlidingAverage(sliding_average_t & data, double toAdd)
{

    printf("Data: %.2f, toAdd: %.2f, elements_count: %d, first_element: %d\n", data.data, toAdd, data.elements_count, data.first_element);

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
