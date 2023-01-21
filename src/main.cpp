#include <string>
#include <vector>
#include <regex>
#include <cmath>

#include <Arduino.h>

#include <AWS.h>
#include <ledDriver.h>
#include <main.h>
#include <motorDriver.h>
#include <sensorDriver.h>

using namespace std;

int TARGET_AREA = 5;  // coordinate units
int SENSOR_TRIGGER_DISTANCE = 100;  // mm
int TURN_MULTIPLIER = 25;  // ms = angle * multiplier
int AWS_MESSAGE_TIMEOUT = 1500;  // ms

int target_x = 0, target_y = 0;
int rover_x = 0, rover_y = 0, rover_angle = 0;

uint32_t last_message_millis;

QueueHandle_t messageQueue;  // thread safe queue
QueueHandle_t motorQueue;

// required by arduino lib
void setup() {
    messageQueue = xQueueCreate(1, sizeof(char[1000]));
    motorQueue = xQueueCreate(1, sizeof(char[1000]));
    xQueueReset(messageQueue);
    xQueueReset(motorQueue);

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
            (void*) motorQueue,  // parameters
            1,  // priority
            NULL  // optional handle
    );

    // read the sensors
    xTaskCreate(
            task3,  // function
            "Task 3",  // string name
            4096,  // stack size in words (not bytes)
            (void*) messageQueue,  // parameters
            1,  // priority
            NULL  // optional handle
    );

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
        // tokanize digits from message
        vector<int> tokens = tokenizeMessage(message);

        switch(tokens.size()) {
            case 4:  // Rover message - "{5: [(273, 132), 294]}"
                if (target_x == 0 || target_y == 0) break;  // ignore message if no target
                if (tokens[0] == 5) {
                    if (millis() < last_message_millis + AWS_MESSAGE_TIMEOUT) break;  // ignore messages util timeout
                    rover_x = tokens[1];
                    rover_y = tokens[2];
                    rover_angle = tokens[3];
                    // if not at target
                    if (getDistance((double) target_x, (double) target_y, (double) rover_x, (double) rover_y) > TARGET_AREA) {
                        // rotate towards target
                        double angle = getAngle((double) rover_x, (double) rover_y, (double) target_x, (double) target_y);
                        angle -= rover_angle;
                        xQueueSend(motorQueue, to_string(angle).c_str(), 0);
                    }
                    else {
                        xQueueSend(motorQueue, "STOP", 0);
                        Serial.println("** Target Reached **");
                    }
                    last_message_millis = millis();
                }
                else {
                    Serial.println("Invalid rover QR");
                    Serial.println(message);
                }
                break;
            case 3:  // Sensor message - "left:100 middle:110 right:90"
//                if (tokens[0] < SENSOR_TRIGGER_DISTANCE || tokens[1] < SENSOR_TRIGGER_DISTANCE || tokens[2] < SENSOR_TRIGGER_DISTANCE) {
//                    xQueueSend(motorQueue, target_x < rover_x ? "-45" : "45", 0);
//                    vTaskDelay(2000 / portTICK_PERIOD_MS);
//                    xQueueReset(messageQueue);
//                }
                break;
            case 2:  // Target message - "(273, 132)"
//                Serial.printf("target x:%i y:%i\n", tokens[0], tokens[1]);
                target_x = tokens[0];
                target_y = tokens[1];
                break;
            default:
                Serial.println("Invalid message");
                Serial.println(message);
                break;
        }
    }
}

// tokenize digits from message
vector<int> tokenizeMessage(const string& message) {
    vector<int> tokens;
    string intermediate;
    string numbers = regex_replace(message, regex("[^0-9- ]"), "");  // strip non digits and spaces
    numbers = regex_replace(numbers, regex("\\s{2,}"), " ");  // convert 2+ spaces into single
    numbers = regex_replace(numbers, regex("^\\s+|\\s+$"), "");  // left and right trim
    stringstream numberStream(numbers);
    while(getline(numberStream, intermediate,' ')) {
        tokens.push_back(stoi(intermediate));
    }
    return tokens;
}

double getDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double getAngle(double x1, double y1, double x2, double y2) {
    double angle = atan2(x2 - x1, y1 - y2);
    return angle * 180 / M_PI;
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
    motorDriver.setDirection(Direction::Forward);

    while(true) {
        char message[1000];
        if (xQueueReceive(motorQueue, (void *)message, 0)) {
            if (strcmp(message, "STOP") == 0) {
                motorDriver.setSpeed(Speed::Stop);
            }
            else {
                int angle = stoi(message);
                Serial.printf("turn angle: %i\n", angle);
                motorDriver.setSpeed(Speed::Stop);
                vTaskDelay(300 / portTICK_PERIOD_MS);
                motorDriver.setDirection(angle >= 0 ? Direction::Right : Direction::Left);
                motorDriver.setSpeed(Speed::Medium);
                vTaskDelay(((angle >= 0 ? angle : -angle) * TURN_MULTIPLIER) / portTICK_PERIOD_MS);
                motorDriver.setSpeed(Speed::Stop);
                vTaskDelay(300 / portTICK_PERIOD_MS);
                motorDriver.setDirection(Direction::Forward);
                motorDriver.setSpeed(Speed::Slow);
                xQueueReset(motorQueue);
            }
        }
    }
}

// read the sensors
void task3(void * parameter) {
    SensorDriver sensorDriver(parameter);
    sensorDriver.loop();
}

// listen for AWS messages
void task4(void * parameter) {
    aws aws(parameter);
    aws.connectAWS();
    while(true) {
        aws.stayConnected();
        vTaskDelay(100 / portTICK_PERIOD_MS);  // 0 crashes, 1000 times out
    }
}
