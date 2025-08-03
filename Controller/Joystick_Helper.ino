#include "Joystick_Helper.h"
#include "ESP_Now_Helper.h"

void Joystick_Init()
{
  analogSetPinAttenuation(CONTROL_X_PIN, ADC_11db);
  analogSetPinAttenuation(CONTROL_Y_PIN, ADC_11db);
  pinMode(CONTROL_SELECT_PIN, INPUT_PULLUP);
  analogSetPinAttenuation(THRUST_X_PIN, ADC_11db);
  analogSetPinAttenuation(THRUST_Y_PIN, ADC_11db);
  pinMode(THRUST_SELECT_PIN, INPUT_PULLUP);
}

Joystick_Data* ReadControlJoystick()
{
  static Joystick_Data data;
  data.x = analogRead(CONTROL_X_PIN);
  data.y = analogRead(CONTROL_Y_PIN);
  data.select = digitalRead(CONTROL_SELECT_PIN) == LOW; // switch so it is true when it is pushed
  return &data;
}

Joystick_Data* ReadThrustJoystick()
{
  static Joystick_Data data;
  data.x = analogRead(THRUST_X_PIN);
  data.y = analogRead(THRUST_Y_PIN);
  data.select = digitalRead(THRUST_SELECT_PIN) == LOW; // switch so it is true when it is pushed
  return &data;
}

void Print_Joystick_Data(Joystick_Data* data)
{
  Serial.print("X axis: ");
  Serial.println(data->x);
  Serial.print("Y axis: ");
  Serial.println(data->y);
  Serial.print("Select: ");
  Serial.println(data->select);
}

Controller_Message* Write_Message_With_Joystick_Data()
{
  Joystick_Data* control = ReadControlJoystick();
  Joystick_Data* thrust = ReadThrustJoystick();
  static Controller_Message message;
  message.x_axis = control->x;
  message.y_axis = control->y;
  message.thrust = thrust->y;
  message.yaw = thrust->x;
  return &message;
}