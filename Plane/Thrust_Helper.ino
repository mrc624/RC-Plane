void Init_Thrust()
{
  pinMode(THRUST_PIN, OUTPUT);
}

void Set_Thrust_With_Tick(int tick)
{
  if (tick >= MAX_TICK)
  {
    Set_Thrust_Duty(255);
  }
  else if (tick <= 0)
  {
    Set_Thrust_Duty(0);
  }
  else
  {
    Set_Thrust_Duty((255 / MAX_TICK) * tick);
  }
}

void Set_Thrust_Duty(int duty)
{
  duty = 255 - duty; // invert because of p channel
  analogWrite(THRUST_PIN, duty);
}