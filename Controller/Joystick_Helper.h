#ifndef JOYSTICK_HELPER_H
#define JOYSTICK_HELPER_H 

#define CONTROL_X_PIN 34
#define CONTROL_Y_PIN 35
#define CONTROL_SELECT_PIN 26

#define THRUST_X_PIN 32
#define THRUST_Y_PIN 33
#define THRUST_SELECT_PIN 27

#define JOYSTICK_MAX_VAL 4095
#define JOYSTICK_MIN_VAL 0

typedef struct Joystick_Data
{
  int x;
  int y;
  bool select;
};

void Joystick_Init();
Joystick_Data* ReadControlJoystick();
Joystick_Data* ReadThrustJoystick();
Controller_Message* Write_Message_With_Joystick_Data();
void Print_Joystick_Data(Joystick_Data* data);

#endif