#ifndef JOYSTICK_HELPER_H
#define JOYSTICK_HELPER_H 

#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 35
#define JOYSTICK_SELECT_PIN 32

#define JOYSTICK_MAX_VAL 4095
#define JOYSTICK_MIN_VAL 0

typedef struct Joystick_Data
{
  int x;
  int y;
  bool select;
};

void Joystick_Init();
Joystick_Data ReadJoystick();

#endif