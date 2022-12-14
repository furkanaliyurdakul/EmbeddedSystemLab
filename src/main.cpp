#include <Arduino.h>
#include <motorDriver.h>
#include <sensorDriver.h>

void taskOne( void * parameter);
void taskTwo( void * parameter);
#define LED_BOARD 2 //change here the pin of the board to V2
mclass motors;
sclass sensors;

void setup(){
  pinMode(LED_BOARD, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  xTaskCreate(
                    taskOne,          /* Task function. */
                    "TaskOne",        /* String with name of task. */
                    4096,              /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

  xTaskCreate(
                    taskTwo,          /* Task function. */
                    "TaskTwo",        /* String with name of task. */
                    4096,              /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */

  motors.SETUP();
  sensors.SETUP();

}

void loop(){
delay(1000);
}

void taskOne( void * parameter )
{
    //example of a task that executes for some time and then is deleted
    for( int i = 0; i < 2; i++ )
    {
      
      Serial.println("Hello from task: 1");
      //Switch on the LED
      digitalWrite(LED_BOARD, HIGH); 
      // Pause the task for 1000ms
      //delay(1000); //This delay doesn't give a chance to the other tasks to execute
      vTaskDelay(1000 / portTICK_PERIOD_MS); //this pauses the task, so others can execute
      // Switch off the LED
      digitalWrite(LED_BOARD, LOW);
      // Pause the task again for 500ms
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    Serial.println("Ending task: 1");
    vTaskDelete( NULL );
}
 
void taskTwo( void * parameter)
{
    uint16_t j = 120;
    //create an endless loop so the task executes forever
    for( ;; )
    {
      if(j <250)
        j += 3;
      else
        j = 120;

      //motors.set_speed(MotorA,Backward,j);
      //vTaskDelay(250 / portTICK_PERIOD_MS);

      //motors.set_speed(MotorB,Backward,j);

      //Switch on the LED
      //digitalWrite(LED_BOARD, HIGH); 
      // Pause the task for 1000ms
      //delay(1000); //This delay doesn't give a chance to the other tasks to execute
      
      //vTaskDelay(500 / portTICK_PERIOD_MS);

      // Switch off the LED
      //digitalWrite(LED_BOARD, LOW);
      motors.set_speed(MotorA,Backward,j);
      vTaskDelay(50 / portTICK_PERIOD_MS);

      motors.set_speed(MotorB,Backward,j);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      sensors.readSomething();
      Serial.printf("speed: %d\n", j);
      //static int16_t *testValue = sensors.reading();
      //Serial.printf("[%08x]\n", testValue[1]);

    }
    Serial.println("Ending task 2"); //should not reach this point but in case...
    vTaskDelete( NULL );
}
