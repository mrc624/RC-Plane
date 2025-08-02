#include "Commands.h"

// Controller's MAC Address
uint8_t broadcastAddress[6] = {0xC0, 0x5D, 0x89, 0xB0, 0xAD, 0xC0};

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial_Print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail", DBG_ESP_NOW ,status == ESP_NOW_SEND_SUCCESS ? COLOR_Green : COLOR_Red);
}

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while receiving data
  plane_message msg;
  memcpy(&msg, incomingData, sizeof(msg));
  Serial_Print("Bytes received: " + (String)len, DBG_ESP_NOW);
  Serial_Print("Plane Message: " + (String)msg.message, DBG_ESP_NOW);
  digitalWrite(LED_PIN, LOW);
}

hw_timer_t * Retry_Init_Timer = NULL;

void IRAM_ATTR onTimerRetry() {
  Serial_Print("Trying to initialize ESP Now", DBG_ESP_NOW);
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
    Serial_Print("Failed initializing ESP Now, trying again in 1 second",DBG_ESP_NOW ,COLOR_Red);
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
    Serial_Print("Failed adding peer, trying again in 1 second", DBG_ESP_NOW, COLOR_Red);
    return false;
  }
  Serial_Print("ESP Now Initialized", DBG_ESP_NOW);
  ESP_Now_Initialized = true;
  timerDetachInterrupt(Retry_Init_Timer);
  return true;
}

bool Send_Data(controller_message message)
{
  digitalWrite(LED_PIN, HIGH); // Turn on LED while sending data
  // Send the message
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
  
  digitalWrite(LED_PIN, LOW);
  if (result == ESP_OK)
  {
    Serial_Print("Sent with success",DBG_ESP_NOW ,COLOR_Green);
    return true;
  }
  Serial_Print("Error sending the data",DBG_ESP_NOW ,COLOR_Red);
  return false;
}
