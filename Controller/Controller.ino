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
  }

  bool data_response = false;

  void loop() {
    Handle_Commands();

    controller_message message;
    Joystick_Data joystick_data = ReadControlJoystick();
    message.x_axis = joystick_data.x;
    message.y_axis = joystick_data.y; 
    message.yaw = 3;
    message.thrust = 4;
    message.reverse = joystick_data.select;
    message.data_response = data_response;
    data_response = !data_response;
    Send_Data(message);

    Serial.println("\nControl Data:");
    Serial.print("Control X axis: ");
    Serial.println(joystick_data.x);
    Serial.print("Control Y axis: ");
    Serial.println(joystick_data.y);
    Serial.print("Control Select: ");
    Serial.println(joystick_data.select);

    joystick_data = ReadThrustJoystick();
    Serial.println("Thrust Data:");
    Serial.print("Thrust X axis: ");
    Serial.println(joystick_data.x);
    Serial.print("Thrust Y axis: ");
    Serial.println(joystick_data.y);
    Serial.print("Thrust Select: ");
    Serial.println(joystick_data.select);

    delay(2000);
  }