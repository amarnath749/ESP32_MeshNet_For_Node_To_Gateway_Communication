#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
// #include "lib/sensors.h"
// uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xED, 0x30, 0xD8};
const char* ssid     = "802.11";
const char* password = "12345678p";
uint8_t wifiChannel = 6;



typedef struct Data {
  uint8_t UID;
  // char someChar[50];
  int x ;
  int y;
} Data;

// Create struct same as Sender
Data Message;

// Create struct for different boards
Data from_board_1;
Data from_board_2;

// Pack it together
Data whole_data[2]={from_board_1,from_board_2};

void OnReceive(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&Message, incomingData, sizeof(Message));
  Serial.printf("Board ID %u: %u bytes\n", Message.UID, len);
  // Update the structures with the new incoming data
  whole_data[Message.UID-1].x = Message.x;
  whole_data[Message.UID-1].y = Message.y;
  // Serial.printf("x value: %d \n", whole_data[Message.UID-1].x);
  // Serial.printf("y value: %d \n", boardsStruct[Message.UID-1].y);
  // Serial.println();
}
void setup(){
Serial.begin(9600);

// Ignore this and get Contant Reboot :)
 WiFi.mode(WIFI_AP_STA);
  
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(wifiChannel, WIFI_SECOND_CHAN_NONE); 
  esp_wifi_set_promiscuous(false); 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive));
}
void loop(){}
