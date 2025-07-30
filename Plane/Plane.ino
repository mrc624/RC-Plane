  #include "Commands.h"
  #include "ESP_Now_Helper.h"

  #define THIS_MAC "c0:5d:89:b0:ad:c0"

  void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("===== RC Plane Booting =====");
    ESP_Now_Init();
  }

  void loop() {
    Handle_Commands();
  }