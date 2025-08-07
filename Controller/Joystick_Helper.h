#ifndef JOYSTICK_HELPER_H
#define JOYSTICK_HELPER_H 

// negative values in top left and positive in bottom right
#define CONTROL_X_PIN 34
#define CONTROL_Y_PIN 35 // keep in mind UP on the joystick is negative and down is positive on the Y
#define CONTROL_SELECT_PIN 26

#define THRUST_X_PIN 32
#define THRUST_Y_PIN 33 // keep in mind UP on the joystick is negative and down is positive on the Y
#define THRUST_SELECT_PIN 27

#define JOYSTICK_MIN_VAL 0
#define JOYSTICK_MAX_VAL 4095

#define USE_DEFAULT_CENTER 1
#define DEFAULT_CENTER_MIN_VAL 1850
#define DEFAULT_CENTER_MAX_VAL 1950
#define NOT_YET_CENTERED -1
#define CENTERING_TIME_MILLIS 10000
#define CENTERING_READING_EVERY_MILLIS 100
#define CENTERING_SAFETY_SET 25
#define CENTERING_CONTROL_X_IND 0
#define CENTERING_CONTROL_Y_IND 1
#define CENTERING_THRUST_X_IND 2
#define CENTERING_THRUST_Y_IND 3

#define TICK_VAL 50
#define CENTER_TICK 0
#define MAX_TICK(center_max_val) ((int)(((JOYSTICK_MAX_VAL) - (center_max_val)) / (TICK_VAL) + 1))
#define MIN_TICK(center_min_val) (-(int)((((center_min_val) - (JOYSTICK_MIN_VAL)) / (TICK_VAL)) + 1))

typedef struct Joystick_Data
{
  int x;
  int y;
  bool select;
};

typedef struct Joystick_Center
{
  int min;
  int max;
};

void Joystick_Init();
Joystick_Data* ReadControlJoystick();
Joystick_Data* ReadThrustJoystick();
Controller_Message* Write_Message_With_Joystick_Data();
void Print_Joystick_Data(Joystick_Data* data);
int Get_Tick(int axis_value, Joystick_Center center);
#ifndef USE_DEFAULT_CENTER
void Set_Center_Values();
#endif

#endif