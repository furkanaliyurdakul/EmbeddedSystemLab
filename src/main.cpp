#include <Arduino.h>
#include <main.h>
#include <motorDriver.h>
#include <sensorDriver.h>


// required by arduino lib
void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);

    xTaskCreate(
            task1,  // function
            "Task 1",  // string name
            4096,  // stack size in words (not bytes)
            NULL,  // parameters
            1,  // priority
            NULL  // optional handle
    );

    xTaskCreate(
            task2,  // function
            "Task 2",  // string name
            4096,  // stack size in words (not bytes)
            NULL,  // parameters
            1,  // priority
            NULL  // optional handle
    );
}

// required by arduino lib
void loop() {
    delay(1000);
}

// flash the LED
void task1(void * parameter) {
    for (int i = 0; i < 5; i++) {
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

// start the motors
void task2(void * parameter) {
    MotorDriver motorDriver;
    motorDriver.setSpeed(Speed::Fast);

    motorDriver.setDirection(Direction::Forward);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    motorDriver.setDirection(Direction::Back);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    motorDriver.setDirection(Direction::Left);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    motorDriver.setDirection(Direction::Right);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    motorDriver.setSpeed(Speed::Stop);

    vTaskDelete(NULL);
}
