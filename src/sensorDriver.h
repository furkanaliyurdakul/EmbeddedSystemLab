#ifndef sensorDriver_h
#define sensorDriver_h


class SensorDriver {
public:
    explicit SensorDriver(void * parameter);
    void loop();
private:
    QueueHandle_t messageQueue;
};

#endif
