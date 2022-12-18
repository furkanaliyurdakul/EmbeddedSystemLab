#include "motorDriver.h"


MotorDriver::MotorDriver() {
    pinMode(MOTOR_A_PIN, OUTPUT);
    pinMode(MOTOR_B_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, HIGH);

    ledcSetup(CHANNEL_B, FREQUENCY, RESOLUTION);
    ledcAttachPin(MOTOR_A_PWM, CHANNEL_A);

    ledcSetup(CHANNEL_A, FREQUENCY, RESOLUTION);
    ledcAttachPin(MOTOR_B_PWM, CHANNEL_B);
}


void MotorDriver::setDirection(Direction direction) {
    switch (direction) {
        case Direction::Forward:
            digitalWrite(MOTOR_A_PIN, HIGH);
            digitalWrite(MOTOR_B_PIN, HIGH);
            break;
        case Direction::Back:
            digitalWrite(MOTOR_A_PIN, LOW);
            digitalWrite(MOTOR_B_PIN, LOW);
            break;
        case Direction::Left:
            digitalWrite(MOTOR_A_PIN, LOW);
            digitalWrite(MOTOR_B_PIN, HIGH);
            break;
        case Direction::Right:
            digitalWrite(MOTOR_A_PIN, HIGH);
            digitalWrite(MOTOR_B_PIN, LOW);
            break;
    }
}

void MotorDriver::setSpeed(Speed speed) {
    ledcWrite(CHANNEL_A, static_cast<int>(speed));
    ledcWrite(CHANNEL_B, static_cast<int>(speed));
}
