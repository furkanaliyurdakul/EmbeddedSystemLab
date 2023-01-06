#include <Arduino.h>

#include <ledDriver.h>
#include <main.h>
#include <motorDriver.h>
#include <sensorDriver.h>


int ledTimerDelay = 0;

// required by arduino lib
void setup() {
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
    xTaskCreate(
            task2,  // function
            "Task 2",  // string name
            4096,  // stack size in words (not bytes)
            NULL,  // parameters
            1,  // priority
            NULL  // optional handle
    );

    // read the sensors
    xTaskCreate(
            task3,  // function
            "Task 3",  // string name
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
    LedDriver ledDriver;
    for (;;) {
        ledDriver.on();
        vTaskDelay(ledTimerDelay / portTICK_PERIOD_MS);
        ledDriver.off();
        vTaskDelay(ledTimerDelay / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

// start the motors
void task2(void * parameter) {
    MotorDriver motorDriver;
    //motorDriver.setSpeed(Speed::Slow);
    while (true)
    {

      motorDriver.setSpeed(Speed::Slow);
      motorDriver.setDirection(Direction::Forward);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      motorDriver.setSpeed(Speed::Stop);
      vTaskDelay(300 / portTICK_PERIOD_MS);

      motorDriver.setSpeed(Speed::Slow);
      motorDriver.setDirection(Direction::Back);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      motorDriver.setSpeed(Speed::Stop);
      vTaskDelay(300 / portTICK_PERIOD_MS);
//    motorDriver.setDirection(Direction::Left);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);
//    motorDriver.setDirection(Direction::Right);
//    vTaskDelay(1000 / portTICK_PERIOD_MS);

//    motorDriver.setSpeed(Speed::Stop);

    }
    vTaskDelete(NULL);
}

// read the sensors
void task3(void * parameter) {
    SensorDriver sensorDriver;
    while(true){
    ledTimerDelay = sensorDriver.loop();
    }
}
