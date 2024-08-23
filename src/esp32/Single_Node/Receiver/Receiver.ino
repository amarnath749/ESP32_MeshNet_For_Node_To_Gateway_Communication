#include <WiFi.h>
#include <esp_now.h>

// Receiver's MAC
uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xED, 0x30, 0xD8};

typedef struct Data {
  char someChar[50];
} Data;

Data Message;

void OnReceive(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Message, incomingData, sizeof(Message));
  Serial.println("Message " + String(Message.someChar));
}

void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  // Start ESP-NOW
  if (esp_now_init() != ESP_OK) {
  Serial.println("Error initializing ESP-NOW");
  return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive));
}
void loop(){
  delay(40);
}