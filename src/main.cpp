#include <Arduino.h>

#include <AWS.h>
#include <ledDriver.h>
#include <main.h>
#include <motorDriver.h>
#include <sensorDriver.h>

// thread safe queue
QueueHandle_t messageQueue;

// required by arduino lib
void setup() {
    messageQueue = xQueueCreate(10, sizeof(char[1000]));
    Serial.begin(9600);
    while (!Serial) {}
    Serial.println("\nMain init");

    // flash the LED
    xTaskCreate(
            task1,  // function
            "Task 1",  // string name
            4096,  // stack size in words (not bytes)
            NULL,  // parameters
            1,  // priority
            NULL  // optional handle
    );

    // start the motors
//    xTaskCreate(
//            task2,  // function
//            "Task 2",  // string name
//            4096,  // stack size in words (not bytes)
//            NULL,  // parameters
//            1,  // priority
//            NULL  // optional handle
//    );

    // read the sensors
//    xTaskCreate(
//            task3,  // function
//            "Task 3",  // string name
//            4096,  // stack size in words (not bytes)
//            NULL,  // parameters
//            1,  // priority
//            NULL  // optional handle
//    );

    // communicate with AWS
    xTaskCreate(
            task4,  // function
            "Task 4",  // string name
            4096,  // stack size in words (not bytes)
            (void*) messageQueue,  // parameters
            1,  // priority
            NULL  // optional handle
    );
}

// required by arduino lib
void loop() {
    char message[1000];
    if (xQueueReceive(messageQueue, (void *)message, 0)) {
        Serial.println(message);
    }
}

// flash the LED
void task1(void * parameter) {
    LedDriver ledDriver;
    for (int i = 0; i < 5; i++) {
        ledDriver.on();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledDriver.off();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

// start the motors
void task2(void * parameter) {
    MotorDriver motorDriver;
    motorDriver.setSpeed(Speed::Medium);

    motorDriver.setDirection(Direction::Forward);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
//    motorDriver.setDirection(Direction::Back);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);
//    motorDriver.setDirection(Direction::Left);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);
//    motorDriver.setDirection(Direction::Right);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);

    motorDriver.setSpeed(Speed::Stop);

    vTaskDelete(NULL);
}

// read the sensors
void task3(void * parameter) {
    SensorDriver sensorDriver;
    sensorDriver.loop();
}

// read the sensors
void task4(void * parameter) {
    myawsclass aws(parameter);
    aws.connectAWS();
    while(true) {
        aws.stayConnected();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
