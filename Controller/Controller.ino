  #include "Commands.h"
  #include "ESP_Now_Helper.h"
  #include "Joystick_Helper.h"

  #define THIS_MAC "a0:dd:6c:b2:33:9c"
  #define SEND_DATA_TIMES_PER_SECOND 1
  #define TIMER_FREQ 1000000

  hw_timer_t * SendMessageTimer = NULL;

  volatile bool send_data = false;

  void ARDUINO_ISR_ATTR  SendMessage() {
    send_data = true;
  }

  void InitMessageTimer()
  {
    SendMessageTimer = timerBegin(TIMER_FREQ);
    timerAttachInterrupt(SendMessageTimer, &SendMessage);
    timerAlarm(SendMessageTimer, TIMER_FREQ / SEND_DATA_TIMES_PER_SECOND, true, 0);
    //timerStart(SendMessageTimer);
    Serial_Println("Timer Initialized", DBG_MAIN);
  }

  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("===== RC Plane Booting =====");
    //ESP_Now_Init();
    Joystick_Init();
  #ifndef USE_DEFAULT_CENTER
    Set_Center_Values();
  #endif
    InitMessageTimer();
  }

  bool data_response = false;

  void loop() {
    Handle_Commands();

    if (send_data)
    {
      Serial_Println("Message", DBG_MAIN);
      send_data = false;
    }
  }