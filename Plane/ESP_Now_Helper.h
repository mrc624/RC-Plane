#ifndef ESP_NPW_HELPER_H
#define ESP_NPW_HELPER_H 
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define LED_PIN 2
#define MAX_MESSAGE_LIMIT 50

typedef struct controller_message
{
  int x_axis;
  int y_axis;
  int yaw;
  int thrust;
  bool reverse;
  bool data_response;
} controller_message;

typedef struct plane_message
{
  char message[MAX_MESSAGE_LIMIT];
} plane_message;

bool ESP_Now_Initialized = false;

bool ESP_Now_Init();
bool Is_ESP_Now_Initialized();
bool Send_Data(plane_message message);

#endif