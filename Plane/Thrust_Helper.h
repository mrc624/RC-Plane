#ifndef THRUST_HELPER_H
#define THRUST_HELPER_H 

#define THRUST_PIN 21
#define THRUST_PWM_FREQUENCY 1000
#define DEFAULT_DUTY_CYCLE 0
#define THRUST_PWM_CHANNEL 0
#define THRUST_PWM_RESOLUTION 8

void Init_Thrust();
void Set_Thrust_With_Tick(int tick);

#endif