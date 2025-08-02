#include "Joystick_Helper.h"

void Joystick_Init()
{
  analogSetPinAttenuation(CONTROL_X_PIN, ADC_11db);
  analogSetPinAttenuation(CONTROL_Y_PIN, ADC_11db);
  pinMode(CONTROL_SELECT_PIN, INPUT_PULLUP);
  analogSetPinAttenuation(THRUST_X_PIN, ADC_11db);
  analogSetPinAttenuation(THRUST_Y_PIN, ADC_11db);
  pinMode(THRUST_SELECT_PIN, INPUT_PULLUP);
}

Joystick_Data ReadControlJoystick()
{
  Joystick_Data data;
  data.x = analogRead(CONTROL_X_PIN);
  data.y = analogRead(CONTROL_Y_PIN);
  data.select = digitalRead(CONTROL_SELECT_PIN) == LOW; // switch so it is true when it is pushed
  return data;
}

Joystick_Data ReadThrustJoystick()
{
  Joystick_Data data;
  data.x = analogRead(THRUST_X_PIN);
  data.y = analogRead(THRUST_Y_PIN);
  data.select = digitalRead(THRUST_SELECT_PIN) == LOW; // switch so it is true when it is pushed
  return data;
}