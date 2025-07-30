  #include "Commands.h"
  #include "ESP_Now_Helper.h"


  #define THIS_MAC "a0:dd:6c:b2:33:9c"

  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("===== RC Plane Booting =====");
    ESP_Now_Init();
  }

  void loop() {
    Handle_Commands();

    controller_message message;
    message.x_axis = 1;
    message.y_axis = 2; 
    message.yaw = 3;
    message.thrust = 4;
    message.reverse = false;
    message.data_response = true;

    Send_Data(message);
    delay(2000);
  }