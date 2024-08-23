// Read This Carefully 
// Need to Set ID For each Node or Sender we can call that UID -> Unique Identifier

#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

uint8_t wifiChannel = 6;

uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xED, 0x30, 0xD8};

typedef struct Data {
  uint8_t UID;
  char someChar[50];
  int x;
  int y;
} Data;

Data Message;
 
esp_now_peer_info_t peerInfo;

// Function that executes when data sends
void sendStatus(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}

void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE); 
  esp_wifi_set_promiscuous(false);
  // Start ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Faild Starting");
    return;
  }
  // Register the callback Function -> executes when data sends
  esp_now_register_send_cb(sendStatus);

  // Register the peer
    // Register the peer
  memcpy(peerInfo.peer_addr, broadcastAddress,6); // 6 -> Number of Bytes sizeof(MAC) = 6
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop(){
Message.UID = 1;
strcpy(Message.someChar ,"This is a Message from Node 1");
 esp_err_t result =
      esp_now_send(broadcastAddress, (uint8_t *)&Message, sizeof(Message));
  // Check If the Tx was successful
  if (result == ESP_OK) {
    Serial.println("Success");
  }
  else {
    Serial.println("Failed");
    }

delay(1000);

}

