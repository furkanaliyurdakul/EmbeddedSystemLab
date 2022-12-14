/**
 * ESP32 Motor Library
 * 
 * Functions to set motor speed
 * 
 * Authors: Vipul Deshpande, Jaime Burbano
 */

#include "Arduino.h"
#include "motorDriver.h"

static uint8_t ADIR = 19;           /* Pin for Motor 0 */
static uint8_t APWM = 5;           /* PWM for Motor 0 */
static uint8_t BDIR = 17;           /* Pin for Motor 1 */
static uint8_t BPWM = 18;           /* PWM for Motor 1 */
static uint8_t HEN = 22; //High Enable Pin
static int freq = 20000;
static uint8_t Channel1 = 0;
static uint8_t Channel2 = 1;
static uint8_t resolution = 8;


mclass::mclass() {
}

void mclass::SETUP() {
  pinMode(ADIR, OUTPUT);            /* Initializing pins as Outputs */
  //pinMode(APWM, OUTPUT);            /* Initializing pins as Outputs */
  pinMode(BDIR, OUTPUT);            /* Initializing pins as Outputs */
  //pinMode(BPWM, OUTPUT);            /* Initializing pins as Outputs */
  pinMode(HEN, OUTPUT);            /* Initializing pins as Outputs */
  digitalWrite(HEN, HIGH);
  
  ledcSetup(Channel1, freq, resolution);
  ledcAttachPin(APWM, Channel1);

  ledcSetup(Channel2, freq, resolution);
  ledcAttachPin(BPWM, Channel2);


}


void mclass::SPEED(int motor_speed) {
  
}

void mclass::motor_direction(Motors motor_ch, Direction dir) {
  
  if (motor_ch == MotorA)
  {
    if (dir == Forward)
    {
      digitalWrite(ADIR,LOW);  //uncomment to use ESP custom
    }
    else 
    {
      digitalWrite(ADIR,HIGH);   //uncomment to use ESP custom
    }
  }

  else
  {
    if (dir == Forward)
    {
      digitalWrite(BDIR,LOW);   //uncomment to use ESP custom
    }
    else
    {
      digitalWrite(BDIR,HIGH);  //uncomment to use ESP custom
    }
  }
}

void mclass::set_speed(Motors motor_ch, Direction dir, int new_speed) {
  motor_direction(motor_ch, dir);   /* polarize the motors before setting the speed */
  
  if (new_speed < 0)
  {
    new_speed = 0;
  }

  else if (new_speed > 255)
  {
    new_speed = 255;
  }

  else if (new_speed >= 0 && new_speed <= 255)
  {
    new_speed = new_speed;
    //Serial.println("setting new speed");

  }

  else
  {
    /* Do nothing */
  }

  if (motor_ch == MotorA)
  {
    ledcWrite(Channel1, new_speed);
    //Serial.println("setting new speed");
  }
  else
  {
    ledcWrite(Channel2, new_speed);
  }
}

mclass motorobject = mclass();  /* creating an object of class motor */
