#ifndef motor_h
#define motor_h

#include "Arduino.h"

#define MOTOR_A_PIN 19
#define MOTOR_A_PWM 5
#define MOTOR_B_PIN 17
#define MOTOR_B_PWM 18
#define ENABLE_PIN 22
#define FREQUENCY 20000
#define CHANNEL_A 0
#define CHANNEL_B 1
#define RESOLUTION 8

enum class Direction {
    Forward = 0,
    Back = 1,
    Left = 2,
    Right = 3
};

enum class Speed {
    Stop = 0,
    Slow = 155,
    Medium = 183,  // usb:200 battery:183
    Fast = 230
};

class MotorDriver {
  public:
    MotorDriver();
    void setDirection(Direction direction);
    void setSpeed(Speed speed);
};

#endif
