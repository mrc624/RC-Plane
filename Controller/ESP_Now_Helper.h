#ifndef ESP_NPW_HELPER_H
#define ESP_NPW_HELPER_H 
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define LED_PIN 2
#define MAX_MESSAGE_LIMIT 50

typedef struct Controller_Message
{
  int x_axis;
  int y_axis;
  int yaw;
  int thrust;
  bool reverse;
  bool data_response;
};

typedef struct Plane_Message
{
  char message[MAX_MESSAGE_LIMIT];
};

bool ESP_Now_Initialized = false;

bool ESP_Now_Init();
bool Is_ESP_Now_Initialized();
bool Send_Data(Controller_Message* message);
void Print_Message(Controller_Message* message);

#endif