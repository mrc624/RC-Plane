#include "Joystick_Helper.h"
#include "ESP_Now_Helper.h"
#include "Commands.h"

Joystick_Center Joystick_Centers[4] = {
  {DEFAULT_CENTER_MIN_VAL, DEFAULT_CENTER_MAX_VAL},  // CONTROL_X
  {DEFAULT_CENTER_MIN_VAL, DEFAULT_CENTER_MAX_VAL},  // CONTROL_Y
  {DEFAULT_CENTER_MIN_VAL, DEFAULT_CENTER_MAX_VAL},  // THRUST_X
  {DEFAULT_CENTER_MIN_VAL, DEFAULT_CENTER_MAX_VAL}   // THRUST_Y
};

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
  Serial_Print("X axis: ", DBG_JOYSTICK);
  Serial_Println(String(data->x), DBG_JOYSTICK);
  Serial_Print("Y axis: ", DBG_JOYSTICK);
  Serial_Println(String(data->y), DBG_JOYSTICK);
  Serial_Print("Select: ", DBG_JOYSTICK);
  Serial_Println(String(data->select), DBG_JOYSTICK);
}

Controller_Message* Write_Message_With_Joystick_Data()
{
  Joystick_Data* control = ReadControlJoystick();
  Joystick_Data* thrust = ReadThrustJoystick();
  static Controller_Message message;
  message.x_axis = Get_Tick(control->x, Joystick_Centers[CENTERING_CONTROL_X_IND]);
  message.y_axis = Get_Tick(control->y, Joystick_Centers[CENTERING_CONTROL_Y_IND]);
  message.thrust = Get_Tick(thrust->y, Joystick_Centers[CENTERING_THRUST_Y_IND]);
  message.yaw = Get_Tick(thrust->x, Joystick_Centers[CENTERING_THRUST_X_IND]);
  return &message;
}

int Get_Tick(int axis_value, Joystick_Center center)
{
  if (axis_value < center.min)
  {
    int tick = 0;
    for (int i = axis_value; i < center.min; i += TICK_VAL)
    {
      tick--;
    }
    return tick;
  }
  else if (axis_value > center.max)
  {
    int tick = 0;
    for (int i = axis_value; i > center.max; i -= TICK_VAL)
    {
      tick++;
    }
    return tick;
  }
  else
  { // we are centered
    return CENTER_TICK;
  }
}

void Adjust_Center_Value(int index, int new_value)
{
  Joystick_Center center = Joystick_Centers[index]; 
  if (new_value > center.max || center.max == NOT_YET_CENTERED)
  {
    Joystick_Centers[index].max = new_value;
  }
  if (new_value < center.min || center.min == NOT_YET_CENTERED)
  {
    Joystick_Centers[index].min = new_value;
  }
}

void Print_Center(int index)
{
  Serial_Print("Min: ", DBG_JOYSTICK);
  Serial_Print(String(Joystick_Centers[index].min), DBG_JOYSTICK);
  Serial_Print(" ", DBG_JOYSTICK);
  Serial_Print("Max: ", DBG_JOYSTICK);
  Serial_Println(String(Joystick_Centers[index].max), DBG_JOYSTICK);
}

void Set_Center_Values()
{
  Serial_Println("Setting Joystick Center Values...", DBG_JOYSTICK);

  Joystick_Centers[CENTERING_CONTROL_X_IND] = {NOT_YET_CENTERED, NOT_YET_CENTERED}; // CONTROL_X
  Joystick_Centers[CENTERING_CONTROL_Y_IND] = {NOT_YET_CENTERED, NOT_YET_CENTERED}; // CONTROL_Y
  Joystick_Centers[CENTERING_THRUST_X_IND] = {NOT_YET_CENTERED, NOT_YET_CENTERED}; // THRUST_X
  Joystick_Centers[CENTERING_THRUST_Y_IND] = {NOT_YET_CENTERED, NOT_YET_CENTERED}; // THRUST_Y

  int end_millis = millis() + CENTERING_TIME_MILLIS;

  while (millis() < end_millis)
  {
    Joystick_Data* control_data = ReadControlJoystick();
    Joystick_Data* thrust_data = ReadThrustJoystick();
    Adjust_Center_Value(CENTERING_CONTROL_X_IND, control_data->x);
    Adjust_Center_Value(CENTERING_CONTROL_Y_IND, control_data->y);
    Adjust_Center_Value(CENTERING_THRUST_X_IND, thrust_data->x);
    Adjust_Center_Value(CENTERING_THRUST_Y_IND, thrust_data->y);
    delay(CENTERING_READING_EVERY_MILLIS);
  }

  // Set all with a 10 offset
  Adjust_Center_Value(CENTERING_CONTROL_X_IND, Joystick_Centers[CENTERING_CONTROL_X_IND].max + CENTERING_SAFETY_SET);
  Adjust_Center_Value(CENTERING_CONTROL_X_IND, Joystick_Centers[CENTERING_CONTROL_X_IND].min - CENTERING_SAFETY_SET);

  Adjust_Center_Value(CENTERING_CONTROL_Y_IND, Joystick_Centers[CENTERING_CONTROL_Y_IND].max + CENTERING_SAFETY_SET);
  Adjust_Center_Value(CENTERING_CONTROL_Y_IND, Joystick_Centers[CENTERING_CONTROL_Y_IND].min - CENTERING_SAFETY_SET);

  Adjust_Center_Value(CENTERING_THRUST_X_IND, Joystick_Centers[CENTERING_THRUST_X_IND].max + CENTERING_SAFETY_SET);
  Adjust_Center_Value(CENTERING_THRUST_X_IND, Joystick_Centers[CENTERING_THRUST_X_IND].min - CENTERING_SAFETY_SET);

  Adjust_Center_Value(CENTERING_THRUST_Y_IND, Joystick_Centers[CENTERING_THRUST_Y_IND].max + CENTERING_SAFETY_SET);
  Adjust_Center_Value(CENTERING_THRUST_Y_IND, Joystick_Centers[CENTERING_THRUST_Y_IND].min - CENTERING_SAFETY_SET);

  Serial_Print("Control X ", DBG_JOYSTICK);
  Print_Center(CENTERING_CONTROL_X_IND);
  Serial_Print("Control Y ", DBG_JOYSTICK);
  Print_Center(CENTERING_CONTROL_Y_IND);
  Serial_Print("Thrust X ", DBG_JOYSTICK);
  Print_Center(CENTERING_THRUST_X_IND);
  Serial_Print("Thrust Y ", DBG_JOYSTICK);
  Print_Center(CENTERING_THRUST_Y_IND);
}