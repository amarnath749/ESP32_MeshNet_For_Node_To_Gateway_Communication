#include <WiFi.h>
#include "painlessMesh.h"

#define   MESH_PREFIX     "GCEK_ESP_MESH"
#define   MESH_PASSWORD   "linuxisnotunix"
#define   MESH_PORT       5555

const char* ssid = "802.11";
const char* password = "12345678p";
Scheduler userScheduler; 
painlessMesh  mesh;
void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println("This ID " + String(mesh.getNodeId()));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  mesh.setDebugMsgTypes(   ERROR | STARTUP);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT,WIFI_STA ); // 6 -> wifi channel
  Serial.println("");
  Serial.println("WiFi connected.");



}
void loop(){
  mesh.update();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
delay(1000);
}
