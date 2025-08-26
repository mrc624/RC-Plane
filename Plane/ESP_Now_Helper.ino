#include "Commands.h"
#include "Thrust_Helper.h"
#include "Servo_Helper.h"

// Controller's MAC Address
uint8_t broadcastAddress[6] = {0xA0, 0xDD, 0x6C, 0xB2, 0x33, 0x9C};

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const wifi_tx_info_t *wifi_info, esp_now_send_status_t status)
{
  Serial_Println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail", DBG_ESP_NOW ,status == ESP_NOW_SEND_SUCCESS ? COLOR_Green : COLOR_Red);
}

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while receiving data
  Controller_Message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  Print_Message(&msg);
  if (msg.data_response)
  {
    Plane_Message message;
    sprintf(message.message, "Some info about the plane, possibly a fatal flaw");
    Send_Data(&message);
  }
  Set_Thrust_With_Tick(-msg.thrust);
  Set_Servo_With_Tick(SERVO_MAIN_RIGHT, msg.x_axis);
  Set_Servo_With_Tick(SERVO_MAIN_LEFT, -msg.x_axis);
  Set_Servo_With_Tick(SERVO_BACK_RIGHT, msg.y_axis);
  Set_Servo_With_Tick(SERVO_BACK_LEFT, msg.y_axis);
  Set_Servo_With_Tick(SERVO_YAW, msg.yaw);
  digitalWrite(LED_PIN, LOW);
}

bool ESP_Now_Init()
{
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial_Println("Failed initializing ESP Now", DBG_ESP_NOW, COLOR_Red);
    return false;
  }
  
  // Set call back function for data received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Set call back function for data setn
  esp_now_register_send_cb(OnDataSent);
  
  // Set the peer into peer info
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
       
  // Add the peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial_Println("Failed adding peer", DBG_ESP_NOW, COLOR_Red);
    return false;
  }
  Serial_Println("ESP Now Initialized", DBG_ESP_NOW);
  ESP_Now_Initialized = true;
  return true;
}

bool Send_Data(Plane_Message* message)
{
  // Send the message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) message, sizeof(Plane_Message));

  if (result == ESP_OK)
  {
    Serial_Println("Sent with success",DBG_ESP_NOW ,COLOR_Green);
    return true;
  }
  Serial_Println("Error sending the data",DBG_ESP_NOW ,COLOR_Red);
  return false;
}

void Print_Message(Controller_Message* message)
{
  if (Debug_Enabled(DBG_ESP_NOW))
  {
    Serial_Print("X-Axis: ", DBG_ESP_NOW);
    Serial_Println(String(message->x_axis), DBG_ESP_NOW);
    Serial_Print("Y-Axis: ", DBG_ESP_NOW);
    Serial_Println(String(message->y_axis), DBG_ESP_NOW);
    Serial_Print("Thrust: ", DBG_ESP_NOW);
    Serial_Println(String(message->thrust), DBG_ESP_NOW);
    Serial_Print("Yaw: ", DBG_ESP_NOW);
    Serial_Println(String(message->yaw), DBG_ESP_NOW);
    Serial_Print("Rsponse: ", DBG_ESP_NOW);
    Serial_Println(String(message->data_response), DBG_ESP_NOW);
  }
}
