
#ifndef ESP32_FLIGHTCOMPUTER_LORACOMMUNICATION_HPP
#define ESP32_FLIGHTCOMPUTER_LORACOMMUNICATION_HPP


#include <RadioLib.h>

class LoRaCommunication
{
public:
    explicit LoRaCommunication(SPIClass & spi);

    bool transmit(const char * str);

private:
    RFM95 radio;

};


#endif //ESP32_FLIGHTCOMPUTER_LORACOMMUNICATION_HPP
