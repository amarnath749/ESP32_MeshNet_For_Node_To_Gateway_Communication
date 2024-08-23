#include <WiFi.h>
void setup(){
Serial.begin(9600);
WiFi.mode(WIFI_STA);
WiFi.STA.begin();
Serial.println();
}

void loop(){
Serial.print("ESP Board MAC Address: ");
Serial.println(WiFi.macAddress());
delay(500);

}
