#include "ledDriver.h"

LedDriver::LedDriver() {
    Serial.println("LedDriver init");
    pinMode(LED_PIN, OUTPUT);
}

void LedDriver::on() {
    digitalWrite(LED_PIN, HIGH);
}

void LedDriver::off() {
    digitalWrite(LED_PIN, LOW);
}
