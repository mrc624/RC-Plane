#include "Joystick_Helper.h"

void Joystick_Init()
{
  analogSetPinAttenuation(JOYSTICK_X_PIN, ADC_11db);
  analogSetPinAttenuation(JOYSTICK_Y_PIN, ADC_11db);
  pinMode(JOYSTICK_SELECT_PIN, INPUT);
}

Joystick_Data ReadJoystick()
{
  Joystick_Data data;
  data.x = analogRead(JOYSTICK_X_PIN);
  data.y = analogRead(JOYSTICK_Y_PIN);
  data.select = !digitalRead(JOYSTICK_SELECT_PIN); // switch so it is true when it is pushed
  return data;
}