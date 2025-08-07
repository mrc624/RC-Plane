  #include "Commands.h"
  #include "ESP_Now_Helper.h"
  #include "Joystick_Helper.h"


  #define THIS_MAC "a0:dd:6c:b2:33:9c"

  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("===== RC Plane Booting =====");
    ESP_Now_Init();
    Joystick_Init();
  #ifndef USE_DEFAULT_CENTER
    Set_Center_Values();
  #endif
  }

  bool data_response = false;

  void loop() {
    Handle_Commands();

    Controller_Message* message = Write_Message_With_Joystick_Data();
    message->data_response = true;
    message->reverse = false;
    message->data_response = SENDING_DEFAULT_NONE;
    Print_Message(message);
    delay(2000);
  }