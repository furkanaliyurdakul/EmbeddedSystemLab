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

/* The MQTT topics that this device should publish/subscribe to */
#define AWS_IOT_SUBSCRIBE_TOPIC1 "esp32/rover"
#define AWS_IOT_SUBSCRIBE_TOPIC2 "esp32/target"
#define AWS_IOT_PUBLISH_TOPIC   "esp32/serial"

class aws {
  public:
    explicit aws(void * parameter);
    void connectAWS();  // Initialize and connect to AWS
    void stayConnected();  // Maintain the connection
    void publish(const char *message);
  private:
    QueueHandle_t messageQueue;
};

#endif
