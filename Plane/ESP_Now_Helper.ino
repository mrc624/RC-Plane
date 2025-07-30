#include "Commands.h"

// Controller's MAC Address
uint8_t broadcastAddress[6] = {0xA0, 0xDD, 0x6C, 0xB2, 0x33, 0x9C};

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Print_Color(status == ESP_NOW_SEND_SUCCESS ? COLOR_Green : COLOR_Red, status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while receiving data
  controller_message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X-Axis: ");
  Serial.println(msg.x_axis);
  Serial.print("Y-Axis: ");
  Serial.println(msg.y_axis);
  Serial.print("Yaw: ");
  Serial.println(msg.yaw);
  Serial.print("Thrust: ");
  Serial.println(msg.thrust);
  Serial.print("Reverse: ");
  Serial.println(msg.reverse);
  Serial.print("Data Response: ");
  Serial.println(msg.data_response);
  digitalWrite(LED_PIN, LOW);

  if (msg.data_response)
  {
    plane_message message;
    sprintf(message.message, "Some info about plane, possibly a fatal flaw");

    Send_Data(message);
  }
}

hw_timer_t * Retry_Init_Timer = NULL;

void IRAM_ATTR onTimerRetry() {
  Serial.println("Trying to initialize ESP Now");
  ESP_Now_Init();
}

void Init_Timer()
{
  Retry_Init_Timer = timerBegin(1);
}

bool ESP_Now_Init()
{
  if (Retry_Init_Timer == NULL)
  {
    pinMode(LED_PIN, OUTPUT);
    Init_Timer();
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Print_Color(COLOR_Red, "Failed initializing ESP Now, trying again in 1 second");
    timerAttachInterrupt(Retry_Init_Timer, &onTimerRetry); // Run the timer to try again
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
    timerAttachInterrupt(Retry_Init_Timer, &onTimerRetry);; // Run the timer to try again
    Print_Color(COLOR_Red, "Failed adding peer, trying again in 1 second");
    return false;
  }
  Serial.println("ESP Now Initialized");
  ESP_Now_Initialized = true;
  timerDetachInterrupt(Retry_Init_Timer);
  return true;
}

bool Send_Data(plane_message message)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while sending data
  // Send the message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
  
  digitalWrite(LED_PIN, LOW);
  if (result == ESP_OK)
  {
    Print_Color(COLOR_Green, "Sent with success");
    return true;
  }
  Print_Color(COLOR_Red, "Error sending the data");
  return false;
}
