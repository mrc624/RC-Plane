#include <ESP32Servo.h>
#include "Servo_Helper.h"
#include "ESP_Now_Helper.h"

Servo Main_Right;
Servo Main_Left;
Servo Back_Right;
Servo Back_Left;
Servo Servo_Yaw;

void Init_Servos()
{
  Main_Right.attach(SERVO_MAIN_RIGHT);
  Main_Left.attach(SERVO_MAIN_LEFT);
  Back_Right.attach(SERVO_BACK_RIGHT);
  Back_Left.attach(SERVO_BACK_LEFT);
  Servo_Yaw.attach(SERVO_YAW);
}

void Handle_Servo(Servos servos, int degree)
{
  switch(servos)
  {
    case SERVO_MAIN_RIGHT:
      Main_Right.write(degree);
      break;
    case SERVO_MAIN_LEFT:
      Main_Left.write(degree);
      break;
    case SERVO_BACK_RIGHT:
      Back_Right.write(degree);
      break;
    case SERVO_BACK_LEFT:
      Back_Left.write(degree);
      break;
    case SERVO_YAW:
      Servo_Yaw.write(degree);
      break; 
  }
}

void Set_Servo_With_Tick(Servos servos, int tick)
{
  if (tick >= MAX_TICK)
  {
    Handle_Servo(servos, MIN_DEGREE);
  }
  else if (tick <= -MAX_TICK)
  {
    Handle_Servo(servos, MAX_DEGREE);
  }
  else if (tick > 0)
  {
    Handle_Servo(servos, (MIDDLE_DEGREE / MAX_TICK) * tick + MIDDLE_DEGREE);
  }
  else if (tick < 0)
  {
    Handle_Servo(servos, (MIDDLE_DEGREE / MAX_TICK) * tick + MIDDLE_DEGREE);
  }
  else
  {
    Handle_Servo(servos, MIDDLE_DEGREE);
  }
}