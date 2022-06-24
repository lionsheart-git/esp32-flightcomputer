#ifndef ESP32_FLIGHTCOMPUTER_SERVOCONTROL_HPP
#define ESP32_FLIGHTCOMPUTER_SERVOCONTROL_HPP

#include <Servo.h>

class ServoControl
{
public:
    ServoControl(int pin, int minPosition = 0, int maxPosition = 180);

    void write(int degrees);

    void open();
    void close();

private:
    int pin_;
    int maxPosition_;
    int minPosition_;

    Servo servo_;

};


#endif //ESP32_FLIGHTCOMPUTER_SERVOCONTROL_HPP
