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

class aws {
  public:
    explicit aws(void * parameter);
    void connectAWS();  // Initialize and connect to AWS
    void stayConnected();  // Maintain the connection
  private:
    QueueHandle_t messageQueue;
};

#endif
