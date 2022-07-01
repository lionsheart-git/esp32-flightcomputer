
#ifndef ESP32_FLIGHTCOMPUTER_CALIBRATIONSEQUENCES_HPP
#define ESP32_FLIGHTCOMPUTER_CALIBRATIONSEQUENCES_HPP

typedef struct sliding_average {
    bool first_element = true;
    int elements_count = 0;
    double data = 0;
} sliding_average_t;

class CalibrationSequences
{
public:
    static void SlidingAverage(sliding_average_t & data, double toAdd);

};


#endif //ESP32_FLIGHTCOMPUTER_CALIBRATIONSEQUENCES_HPP
