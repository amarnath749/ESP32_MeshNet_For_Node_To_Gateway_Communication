#include <WiFi.h>


const char* ssid = "802.11";
const char* password = "12345678p";

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");



}
void loop(){

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
delay(1000);
}
