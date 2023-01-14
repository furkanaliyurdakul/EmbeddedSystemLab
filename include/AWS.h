/**
 * @file
 * 
 * ESP32 AWS Library
 * 
 * Functions to get the crawler coordinates from the Camera over AWS IoT
 * 
 * Authors: Vipul Deshpande, Jaime Burbano
 */

#ifndef aws_h
#define aws_h

#include <MQTTClient.h>

class myawsclass {
  public:
    explicit myawsclass(void * parameter);

    void connectAWS();                            /* Initialize and connect to AWS */
    // void publishMessage(int16_t sensorValue);     /* Publish the values of the sensors */
    void stayConnected();                         /* Maintain the connection */
  private:
    QueueHandle_t messageQueue;
    void messageHandler(String &topic, String &payload);
};

// extern myawsclass awsobject;

#endif
