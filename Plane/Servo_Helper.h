#ifndef SERVO_HELPER_H
#define SERVO_HELPER_H 

#define MAX_DEGREE 0
#define MIN_DEGREE 180
#define MIDDLE_DEGREE 90

typedef enum Servos
{
  SERVO_MAIN_RIGHT = 26,
  SERVO_MAIN_LEFT = 27,
  SERVO_BACK_RIGHT = 14,
  SERVO_BACK_LEFT = 12,
  SERVO_YAW = 13
};

void Init_Servos();
void Set_Servo_With_Tick(Servos servos, int tick);

#endif