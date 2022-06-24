
#include "ServoControl.hpp"
#include "Logging/SystemLogger.hpp"


ServoControl::ServoControl(int pin, int minPosition, int maxPosition)
: pin_(pin), minPosition_(minPosition), maxPosition_(maxPosition)
{
    if (!servo_.attach(pin)) {
        slog_e("Servo not attached!");
    }

    servo_.detach();
}

void ServoControl::write(int degrees)
{
    servo_.attach(pin_);

    servo_.write(degrees);

    servo_.detach();
}

void ServoControl::open()
{
    slog_i("Servo[%d]: Opening", pin_);

    servo_.attach(pin_);

    for (int pos = maxPosition_; pos >= minPosition_; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servo_.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }

    servo_.detach();
}

void ServoControl::close()
{
    slog_i("Servo[%d]: Closing", pin_);

    servo_.attach(pin_);

    for (int pos = minPosition_; pos <= maxPosition_; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servo_.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }

//    servo_.write(maxPosition_);
//    delay(15);

    servo_.detach();
}


