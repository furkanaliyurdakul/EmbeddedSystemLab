// This example shows how to read from all three channels on
// the OPT3101 and store the results in arrays.  It also shows
// how to use the sensor in a non-blocking way: instead of
// waiting for a sample to complete, the sensor code runs
// quickly so that the loop() function can take care of other
// tasks at the same time.

#include <OPT3101.h>
#include <Wire.h>

#include "sensorDriver.h"

OPT3101 sensor;

uint16_t amplitudes[3];
int16_t distances[3];

SensorDriver::SensorDriver(void * parameter) {
    messageQueue = (QueueHandle_t) parameter;

    Serial.println("SensorDriver init");

    Wire.setPins(27, 26);
    Wire.begin();

    sensor.init();
    if (sensor.getLastError()) {
        Serial.print("Failed to initialize OPT3101: error ");
        Serial.println(sensor.getLastError());
        while (1) {}
    }

    sensor.setFrameTiming(256);
    sensor.setChannel(0);
    sensor.setBrightness(OPT3101Brightness::Adaptive);

    sensor.startSample();
}

void SensorDriver::loop() {
    while (true) {
        while (!sensor.isSampleDone()) {}

        sensor.readOutputRegs();

        amplitudes[sensor.channelUsed] = sensor.amplitude;
        distances[sensor.channelUsed] = sensor.distanceMillimeters;

        // process after reading the last channel
        if (sensor.channelUsed == 2) {
            char buffer[50];
            sprintf(
                buffer,
                "left:%d middle:%d right:%d",
                distances[0] > 0 ? distances[0] : 1000,  // ignore invalid negative readings
                distances[1] > 0 ? distances[1] : 1000,
                distances[2] > 0 ? distances[2] + 50 : 1000  // sensor unreliable, only accept readings between 50 and 100
            );
            xQueueSend(messageQueue, buffer, 0);
            // Serial.printf("L:%imm C:%imm R:%imm\n", distances[0], distances[1], distances[2]);
        }

        sensor.nextChannel();
        sensor.startSample();
    }
}
