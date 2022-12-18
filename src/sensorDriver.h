#ifndef sensorDriver_h
#define sensorDriver_h

void setDataReadyFlag();

class SensorDriver {
public:
    SensorDriver();
    void setup();
    void startReading();
};

#endif
