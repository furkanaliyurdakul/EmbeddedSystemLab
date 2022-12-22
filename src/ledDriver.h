#ifndef ledDriver_h
#define ledDriver_h

#include "Arduino.h"

#define LED_PIN 2

class LedDriver {
public:
    LedDriver();
    void on();
    void off();
};


#endif
