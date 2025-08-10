#include "Commands.h"

// Plane's MAC Address
uint8_t broadcastAddress[6] = {0x3C, 0x8A, 0x1F, 0x9D, 0xBD, 0xB8};

esp_now_peer_info_t peerInfo;

uint message_attempt_count = 0;
uint message_delivered_count = 0;

// Callback when data is sent
void OnDataSent(const wifi_tx_info_t *wifi_info, esp_now_send_status_t status)
{
  Serial_Println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail", DBG_ESP_NOW ,status == ESP_NOW_SEND_SUCCESS ? COLOR_Green : COLOR_Red);
}

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while receiving data
  Plane_Message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  Serial_Println("Bytes received: " + (String)len, DBG_ESP_NOW);
  Serial_Print("Plane Message: ", DBG_ESP_NOW);
  Serial_Println(msg.message, DBG_ESP_NOW);
  digitalWrite(LED_PIN, LOW);
}

bool ESP_Now_Init()
{
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial_Println("Failed initializing ESP Now",DBG_ESP_NOW, COLOR_Red);
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

bool Send_Data(Controller_Message* message)
{
  // Send the message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) message, sizeof(Controller_Message));

  if (Debug_Enabled(DBG_ESP_NOW))
  {
    Print_Message(message);
  }
  message_attempt_count++;
  if (result == ESP_OK)
  {
    Serial_Println("Sent with success",DBG_ESP_NOW ,COLOR_Green);
    message_delivered_count++;
    return true;
  }
  Serial_Println("Error sending the data",DBG_ESP_NOW ,COLOR_Red);
  return false;
}

void Print_Message(Controller_Message* message)
{
  Serial.print("X axis: ");
  Serial.println(message->x_axis);
  Serial.print("Y axis: ");
  Serial.println(message->y_axis);
  Serial.print("Thrust: ");
  Serial.println(message->thrust);
  Serial.print("Yaw: ");
  Serial.println(message->yaw);
  Serial.print("Response: ");
  Serial.println(message->data_response);
}
