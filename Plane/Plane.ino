  #include "Commands.h"
  #include "ESP_Now_Helper.h"
  #include "Thrust_Helper.h"

  #define THIS_MAC "3c:8a:1f:9d:bd:b8"

  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("===== RC Plane Booting =====");
    Init_Thrust();
    ESP_Now_Init();
  }

  void loop() {
    Handle_Commands();
  }